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
		//������
		ClientSocket* OnNewConnec(tuint32 uClientIP,tuint16 uClientPort);

		//ip��飬���ڴ˺���������IP�ڰ�����
		bool IPCheck(tuint32 uIP);

		//�յ���Ϣ
		tint32 OnRecvMessage(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen );

		//���ӶϿ�
		void OnDisConnect(ClientSocket* pkClientSocket);

	protected:
		/*
		˵����
				�����Զ���Э���ȡ����
		������
				pInBuf ����buf
				nInBufLen ����buf����
		����ֵ��
				pInBuf�϶�ȡ�ĳ���	
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