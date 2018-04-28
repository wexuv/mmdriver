#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "log.h"
#include "serversocket.h"
#include "socketbinder.h"
#include "ConnectionPool.h"
#include "packet.h"
#include "PacketTest.h"
#include "msg_channel.h"
#include "MessageTest.h"

namespace driver
{
	class NetService : public Service
	{
		typedef void (NetService::*PacketHandler)(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff); 

	public:
		NetService();
		virtual ~NetService();

	protected:
		tint32	GetServiceType() {return ServiceType::LOGIN;};
		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

	public:
		//新链接
		ClientSocket* OnNewConnec(tuint32 uClientIP,tuint16 uClientPort);

		//ip检查，可在此函数内设置IP黑白名单
		bool IPCheck(tuint32 uIP);

		//收到消息
		tint32 OnRecvMessage(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen );

		//链接断开
		void OnDisConnect(ClientSocket* pkClientSocket);

	protected:
		/*
		说明：
				重载自定义协议捡取函数
		参数：
				pInBuf 输入buf
				nInBufLen 输入buf长度
		返回值：
				pInBuf上读取的长度	
		*/
		virtual tuint32 PeekPacket(ClientSocket* pkClientSocket,const tchar* pInBuf, size_t nInBufLen);

	private:
		void HandleUserLogin(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff);
		void HandleDefault(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff);

		bool SendMsgToHttp(const Message* pkMessage);

	private:
		ServerSocket		m_ServerSocket;
		SocketBinder		m_SocketBinder;
		ConnectionPool		m_ConnectionPool;
		PacketHandler		m_pDispatcher[PACKET_ID_MAX];

		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCLogin2Http;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;
	};
}

#endif //_NET_SERVICE_H_