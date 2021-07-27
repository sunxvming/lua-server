#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>

#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"


const  char * HOST = "192.168.1.120";
const  char * DB = "test_db";
const  char * USER = "root";
const  char * PWD = "123456";


void sql_query_1()
{
	DBInterfaceMysql mysql(HOST, DB, USER, PWD);
	mysql.connect();

	
	MysqlResult result;
	mysql.execute(&result, "select * from test");

	while (result.fetch())
	{

		int id;
		int num;
		std::string name;
		result >> id;
		result >> num;
		result >> name;

		printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
	}
}

#include "SqlResultSet.h"

void sql_query_2()
{
	DBInterfaceMysql mysql(HOST, DB, USER, PWD);
	mysql.connect();


	SqlPrepare pre("select * from test where id = ?");
	pre.pushInt32(1);
	pre.prepare(mysql.mysql());

	SqlResultSet result;
	pre.execute(&result);

	while (result.fetch())
	{
		int id = result.getInt32();
		int num = result.getInt32();
		std::string name = result.getString();

		printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
	}
}

#include <functional>
#include "DBThreadPool.h"
void sql_query_3()
{
	DBConfig config;
	config.dbname = DB;
	config.pswd = PWD;
	config.ip = HOST;
	DBThreadPool pool(config);
	pool.create(1);

	{
		std::shared_ptr<SqlPrepare> pre(new SqlPrepare("select * from test where id = ?"));
		pre->pushInt32(1);

		std::shared_ptr<DBSqlTask> task(new DBSqlTask(pre, std::make_shared<SqlResultSet>()));
		task->backfunc = [](int errno_, const char* error, std::shared_ptr<SqlResultSet> result) {

			if (errno_ != 0)
			{
				printf("%s\n", error);
			}
			else
			{
				while (result->fetch())
				{
					int id = result->getInt32();
					int num = result->getInt32();
					std::string name = result->getString();
					printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
				}
			}
		};

		pool.addTask(task);
	}

	
	std::this_thread::sleep_for(std::chrono::seconds(3));
	pool.update();
	
}

#include <iostream>
int main()
{
	MySQL::libraryInit();
	MySQL::threadSafe();

	std::cout << "sql_query_1****************************" << std::endl;
	sql_query_1();
	std::cout << "sql_query_2****************************" << std::endl;
	sql_query_2();
	std::cout << "sql_query_3****************************" << std::endl;
	sql_query_3();

	MySQL::libraryEnd();

	return 0;
}