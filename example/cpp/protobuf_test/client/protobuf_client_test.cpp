#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "NetEvent.h"
#include "EventLoop.h"
#include "NetClient.h"
#include "NetConnect.h"
#include "NetPacket.h"
#include "ProtoPacket.h"
#include "msgtest.pb.h"
#include <google/protobuf/util/json_util.h>


class INetEvent : public NetEvent
{
public:
	virtual void onConnect(NetConnect * conn, int argv)
	{
		if (!conn) return;

		Family family;
		family.set_name("i family");
		family.set_lv(80);

		Role* role1 = family.add_roles();
		role1->set_id(1000);
		role1->set_name("zhangsan");
		role1->set_money(50);
		role1->add_tasks(100);
		role1->add_tasks(101);

		Role* role2 = family.add_roles();
		role2->set_id(1001);
		role2->set_name("lisi");
		role2->set_money(90);
		role2->add_tasks(200);
		role2->add_tasks(201);
		role2->add_tasks(202);

		NetPacket pack;
		writeProto(&pack, family);
		conn->sendMsg(111, &pack);
	};
	virtual void onClose(NetConnect * conn){
		
	};

	virtual void onMsg(NetConnect * conn, int msgtype, NetPacket * pack)
	{
		Family family;
		readProto<NetPacket, Family>(pack, family);

        google::protobuf::util::JsonPrintOptions options;
        options.add_whitespace = true;
        options.always_print_primitive_fields = true;
        options.preserve_proto_field_names = true;

        std::string out_str;
        google::protobuf::util::MessageToJsonString(family, &out_str, options);	
        std::cout << out_str << std::endl;
		
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