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


	virtual void onConnect(KcpSession * conn){
		if (conn)
		{

			// 客户端通知连接成功
            std::cout << "connect to server success." << std::endl;

			int x = rand() % 100000;
			std::string s(std::to_string(x));
			conn->sendMsg(1, s.c_str(), s.length());
            std::cout << "send message to server:" << s << std::endl;
		}
	};
	virtual void onClose(KcpSession * conn){
        std::cout << "onClose" << std::endl;
	};

	virtual void onMsg(KcpSession * conn, int msgtype, UdpPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("receive message from server: %s\n", s.c_str());
		// conn->sendMsg(1, pack);
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


	KcpClient client(EventLoop::Instance(), &eve);
	client.createSession("127.0.0.1", 3001);

	EventLoop::Instance()->run();

	system("pause");
	return 0;
}