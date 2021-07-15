#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "NetEvent.h"
#include "EventLoop.h"
#include "NetClient.h"
#include "NetServer.h"
#include "Timer.h"
#include "NetConnect.h"
#include "NetPacket.h"

class INetEvent : public NetEvent
{
public:
	virtual void onConnect(NetConnect * conn, int argv){
		if (conn)
		{
			// 客户端通知连接成功
            std::cout << "connect to server success." << std::endl;
		}
	};
	virtual void onClose(NetConnect * conn){
		std::cout << "onClose" << std::endl;
	};

	virtual void onMsg(NetConnect * conn, int msgtype, NetPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("receive message from server: %s\n", s.c_str());
		conn->sendMsg(1, pack);
	}
};



int main()
{
	//初始化事件循环
	INetEvent eve;
	EventLoop::Instance()->init();


	NetClient client(EventLoop::Instance(), &eve);
	client.connect("127.0.0.1", 3001);

	//开启事件循环
	EventLoop::Instance()->run();

	return 0;
}