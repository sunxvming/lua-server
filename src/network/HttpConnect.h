#pragma once
/************************************************************************
* @file      HttpConnect.h
* @brief     HttpConnect
* @author    jiangwang
* @data      2020-5-20
* @version   0.1
************************************************************************/

#include "TcpSocket.h"
#include "HttpEvent.h"
#include "PoolObject.h"
#include "HttpParser.h"
 #include <unistd.h>
 
enum http_content_type
{
	hct_text_html ,	//HTML��ʽ
	hct_text_plain,	//���ı���ʽ
	hct_text_xml,	//XML��ʽ
	hct_text_json,	//json��ʽ
	hct_image_gif,	//gifͼƬ��ʽ
	hct_image_jpeg,	//jpgͼƬ��ʽ
	hct_image_png,	//pngͼƬ��ʽ

};

class BasePacket;
class MessageBuffer;
class HttpConnect : public TcpSocket
{
public:
	HttpConnect();
	~HttpConnect();

	void zero();
	void setEvent(HttpEvent * e) { m_event = e; }
	void sendMsg(const char* msg, int32 len);
	// lua call
	void sendData(std::string_view sv);
	void autoMsg(std::string_view sv, enum http_content_type type = hct_text_html);
	static const char *getContentTypeStr(enum http_content_type type);
protected:
	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
	virtual void on_writecomplete();

private:
	void complete();

private:
	HttpEvent * m_event = NULL;
	BasePacket * m_readPacket;
	HttpParser  m_parser;
	const char * m_content;
	int m_residue;

	INCLUDE_POOL_OBJECT
};

