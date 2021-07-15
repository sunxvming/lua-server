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
	virtual void onAccept(NetConnect * conn){
		if (conn)
		{
			// 服务器通知有客户端连入
            std::cout << "accept client connect" << std::endl;
			conn->sendMsg(1, "12345", 5);
		}
	};


	virtual void onClose(NetConnect * conn){
		std::cout << "onClose" << std::endl;
	};

	virtual void onMsg(NetConnect * conn, int msgtype, NetPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("receive message from client: %s\n", s.c_str());
	}
};



int main()
{
	//初始化事件循环
	INetEvent eve;
	EventLoop::Instance()->init();


	NetServer server(EventLoop::Instance(), &eve);
	server.listen("127.0.0.1", 3001);


	//开启事件循环
	EventLoop::Instance()->run();

	return 0;
}