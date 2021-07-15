#include "KcpClient.h"
#include "KcpServer.h"
#include "EventLoop.h"
#include "KcpEvent.h"
#include "KcpSession.h"
#include "UdpPacket.h"
#include <time.h>
#include <iostream>

class KNetEvent : public KcpEvent
{
public:
	virtual KcpSession * createConnect()
	{
		return new KcpSession(this, 1000);
	}

	virtual void onAccept(KcpSession * conn){
		if (conn)
		{
			// 通知服务器有客户端连入
            std::cout << "accept client connect" << std::endl;
		}
	};

	virtual void onClose(KcpSession * conn){
        std::cout << "onClose" << std::endl;
	};

	virtual void onMsg(KcpSession * conn, int msgtype, UdpPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("receive message from client: %s\n", s.c_str());
		conn->sendMsg(1, pack);
        std::cout << "send message to client:" << s << std::endl;
		
	}

	virtual void onUdpTimeout(KcpSession * s)
	{
		printf("timeout ..............\n");
		server->closeSession(s);
	}

	KcpServer * server;
};

//#define UDP_CLIENT_TEST

int main()
{
	srand((int)time(0));
	EventLoop::Instance()->init();

	//初始化事件循环
	KNetEvent eve;
	EventLoop::Instance()->init();

	KcpServer server(EventLoop::Instance(), &eve);
	server.start("127.0.0.1", 3001);
	eve.server = &server;

	EventLoop::Instance()->run();

	system("pause");
	return 0;
}