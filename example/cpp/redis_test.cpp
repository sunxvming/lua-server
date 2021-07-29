#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "XLog.h"
#include "RedisResult.h"
#include <vector>
#include "RedisCommand.h"
#include <functional>
#include "DBThreadPool.h"
#include "Tools.h"

const  char * HOST = "127.0.0.1";
unsigned int PORT = 6379;


void test_ping()
{
	DBInterfaceRedis redis(HOST, PORT);
	redis.connect();	
	redis.ping();	
}

void test_execute()
{
	DBInterfaceRedis redis(HOST, PORT);
	redis.connect();

	
	RedisResult result;
	redis.execute(&result, "set key1 aaa");

	while (result.fetch())
	{
		std::string res;
		result >> res;
		printf("res:%s\n", res.c_str());
	}
}


void test_RedisCommand()
{
	DBInterfaceRedis redis(HOST, PORT);
	redis.connect();

	RedisCommand command("lpush");
	command.pushString("t1");
	command.pushString("1");
	command.pushString("2");
	command.pushString("3");


	RedisResult result;
	redis.execute(&command, &result);

	while (result.fetch())
	{
		std::string res;
		result >> res;
		printf("res:%s\n", res.c_str());
	}	
}

void test_RedisCommand2()
{
	{		
		DBInterfaceRedis redis(HOST, PORT);
		redis.connect();

		RedisResult result;
		redis.execute(&result, "lrange t1 0 10");

		while (result.fetch())
		{
			std::string res;
			result >> res;
			printf("res:%s\n", res.c_str());
		}
	}	
	
	{
		redisContext* c = redisConnect(HOST, PORT);
		const char* command_argv[] = {"lrange", "t1", "0", "10"};
		std::vector<size_t> len = {6, 2, 1, 2};
		redisReply *pRedisReply = (redisReply *)redisCommandArgv(c, 4, command_argv, (size_t *)len.data());
		RedisResult result;
		result.setResult(pRedisReply);
		while (result.fetch())
		{
			std::string res;
			result >> res;
			printf("res2:%s\n", res.c_str());
		}
	}

	{
		DBInterfaceRedis redis(HOST, PORT);
		redis.connect();

		RedisCommand command("lrange");
		command.pushString("t1");
		command.pushString("0");
		command.pushString("10");


		RedisResult result;
		redis.execute(&command, &result);

		while (result.fetch())
		{
			std::string res;
			result >> res;
			printf("res3:%s\n", res.c_str());

		}
	}	
}

void test_RedisCommand3()
{
	{
		DBInterfaceRedis redis(HOST, PORT);
		redis.connect();

		RedisCommand command("lpush");
		command.pushString("t2");
		command.pushInt32(1);
		command.pushInt32(22);
		command.pushInt32(333);


		RedisResult result;
		redis.execute(&command, &result);
	}

	{
		DBInterfaceRedis redis(HOST, PORT);
		redis.connect();

		RedisCommand command("lrange");
		command.pushString("t2");
		command.pushString("0");
		command.pushString("10");


		RedisResult result;
		redis.execute(&command, &result);

		while (result.fetch())
		{
			int32 value;
			result >> value;
			printf("%d\n", value);

		}
	}	
}


void select(DBThreadPool * pool)
{
	std::shared_ptr<RedisCommand> command(new RedisCommand("lrange"));  //lrange就根据两个数字表示的闭区间范围来获取元素数组。
	command->pushString("t3");
	command->pushString("0");
	command->pushString("10");

	std::shared_ptr<RedisResult> result(new RedisResult);

	std::shared_ptr<DBRedisTask> task(new DBRedisTask(command, result));

	task->backfunc = [](int errno_, const char * err, std::shared_ptr<RedisResult> result) {

		while (result->fetch())
		{
			int32 value;
			*result >> value;
			printf("%d\n", value);
		}
	};

	pool->addTask(task);
}

void insert(DBThreadPool * pool)
{
	std::shared_ptr<RedisCommand> command(new RedisCommand("lpush"));   // list push,lpush命令时往列表的队头插入
	command->pushString("t3");
	command->pushInt32(1);
	command->pushInt32(22);
	command->pushInt32(333);

	std::shared_ptr<DBRedisTask> task(new DBRedisTask(command, std::make_shared<RedisResult>()));
	task->backfunc = [pool](int errno_, const char* err, std::shared_ptr<RedisResult> result) {
		select(pool);
	};

	pool->addTask(task);
}


void test_pool()
{
	DBConfig config;
	config.device = "redis";
	config.ip = "127.0.0.1";
	config.port = 6379;

	DBThreadPool pool(config);
	pool.create(1);

	insert(&pool);

	while (1)
	{
		Tools::sleep(10);
		pool.update();
	}
}

int main()
{
	std::cout << "test_ping============" << std::endl;
	test_ping();
	std::cout << "test_execute============" << std::endl;
	test_execute();
	std::cout << "test_RedisCommand============" << std::endl;
	test_RedisCommand();
	std::cout << "test_RedisCommand2============" << std::endl;
	test_RedisCommand2();
	std::cout << "test_RedisCommand3============" << std::endl;
	test_RedisCommand3();	
	std::cout << "test_pool============" << std::endl;
	test_pool();


	return 0;
}