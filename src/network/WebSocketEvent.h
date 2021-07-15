/************************************************************************
* @file      WebSocketEvent.h
* @brief     ����websocket�¼��� �ɼ̳и���
* @author    jiangwang
* @data      2019-12-23
* @version   0.1
************************************************************************/

#pragma once

class WebSocketConnect;
class WebSocketPacket;

class WebSocketEvent
{
public:
	WebSocketEvent();
	virtual ~WebSocketEvent();

	virtual WebSocketConnect * createConnect();
	virtual void destroyConnect(WebSocketConnect * conn);

	virtual void onHandshake(WebSocketConnect *){};  //����
	virtual void onAccept(WebSocketConnect * conn){};
	//virtual void onConnect(WebSocketConnect * conn){};
	virtual void onClose(WebSocketConnect * conn){};
	virtual void onMsg(WebSocketConnect * conn, WebSocketPacket * pack){};
};