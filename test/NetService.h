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
		typedef void (NetService::*MessageHandler)(const MessageHead& rkMsgHead,const tchar* pBuff); 

	public:
		NetService();
		virtual ~NetService();

	protected:
		tint32	GetServiceType() {return ServiceType::LOGIN;};
		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

		void TickNetwork(const TimeData& rkTimeData);
		void TickHttpMsg(const TimeData& rkTimeData);

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

	//packet handler
	private:
		void HandlePacketUserLogin(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff);
		void HandlePacketDefault(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff);

	//message handler
	private:
		void HandleMsgUserLoginRet(const MessageHead& rkMsgHead,const tchar* pBuff);
		void HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		bool SendMsgToHttp(const Message* pkMessage);

	private:
		ServerSocket		m_ServerSocket;
		SocketBinder		m_SocketBinder;
		ConnectionPool		m_ConnectionPool;
		PacketHandler		m_pPacketDispatcher[PACKET_ID_MAX];
		MessageHandler		m_pMessageDispatcher[MESSAGE_ID_MAX];

		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCLogin2Http;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;
	};
}

#endif //_NET_SERVICE_H_