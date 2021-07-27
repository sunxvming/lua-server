#ifndef DB_INTERFACE_MYSQL_H
#define DB_INTERFACE_MYSQL_H

#include "DB_Interface.h"

class MysqlResult;
class SqlPrepare;
class DBInterfaceMysql : public DB_Interface
{
public:
	DBInterfaceMysql(const char * host, const char * dbname, const char * user, const char * pswd = "", unsigned int port = 3306);
	~DBInterfaceMysql();
	
	virtual bool connect();
	virtual bool detach();
	virtual int execute(DBResult * result, const char * cmd, int len = 0);

	virtual const char * getError();
	virtual int getErrno();
	virtual bool ping();

	MYSQL * mysql();

protected:

	std::string m_dbname;
	std::string m_user;
	std::string m_pswd;
	//MYSQL mMysql_; mMysql_字段如果不放在最后的位置的话，当BDInterfaceMysql构造时调用mysql_init(&mMysql_)的时候，会破坏mMysql_字段下面的其他字段的内存，从而造成段错误
	// 目前将MYSQL字段放到了最后的位置，不知道还会有其他的隐患没有。后来知道了，析构的时候会报段错误。
	//后来改成MYSQL * mMysql_;这种指针的形式了，由mysql的库在堆上分配内存，上面出现的段错误就不会再发生了。
	MYSQL * mMysql_;    
};

namespace MySQL
{
	int threadSafe();
	void libraryInit();
	void libraryEnd();
	char const* getLibraryVersion();
};

#endif