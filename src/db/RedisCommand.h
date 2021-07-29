#pragma once
/************************************************************************
* @file      RedisCommand.h
* @brief     redis command param
* @author    jiangwang
* @data      2020-4-2
* @version   0.1
************************************************************************/

class BasePacket;
class RedisCommand
{
public:
	RedisCommand(const char *cmd);
	// push
	void pushInt8(int8 value);
	void pushUint8(uint8 value);
	void pushInt16(int16 value);
	void pushUint16(uint16 value);
	void pushInt32(int32 value);
	void pushUint32(uint32 value);
	void pushInt64(int64 value);
	void pushUint64(uint64 value);
	void pushFloat(float value);
	void pushDouble(double value);
	void pushString(std::string value);
	void pushBlob(BasePacket * packet);
	void pushData(std::string_view sv); // lua call

	template<class T>
	void pushValue(T t)
	{
		append((const uint8 *)&t, sizeof(t));	
	}

	int length();
	const char ** argv(std::vector<char *> & vec);
	const size_t * argvlen();
	const char * tostr();

private:
	void append(const uint8 * p, int len);

private:
	std::vector<std::vector<uint8>> m_param;
	// void *redisCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
	// 因为redisCommandArgv最后一个参数是size_t类型的，所以m_len的类型为size_t,若为int类型的话在返回(size_t *)m_len.data()的时候由于
	// 类型转换的问题会导致redisCommandArgv()函数调用失败
	std::vector<size_t> m_len;   
};

