#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "common/log.h"
#include "network/serversocket.h"
#include "ConnectionPool.h"
#include "network/packet.h"
#include "PacketTest.h"
#include "common/msg_channel.h"
#include "MessageTest.h"

namespace driver
{
	template<class T>
	class PacketHandler
	{
	public:
		typedef void (T::*Handler)(Connection* pConnect,const PacketHead& rkPacketHead,const tchar* pBuff); 
	};

	class NetService : public Service
	{
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

	//packet handler
	private:
		void HandlePacketUserLogin(Connection* pConnect,const PacketHead& rkPacketHead,const tchar* pBuff);
		void HandlePacketDefault(Connection* pConnect,const PacketHead& rkPacketHead,const tchar* pBuff);

	//message handler
	private:
		void HandleMsgUserLoginRet(const MessageHead& rkMsgHead,const tchar* pBuff);
		void HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		bool SendMsgToHttp(const Message* pkMessage);

		Connection*		GetUser(tuint32 uID);

	private:
		ServerSocket		m_ServerSocket;
		ConnectionPool		m_ConnectionPool;

		PacketHandler<NetService>::Handler	m_pPacketDispatcher[PACKET_ID_MAX];
		MessageHandler<NetService>::Handler	m_pMessageDispatcher[MESSAGE_ID_MAX];

		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCLogin2Http;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;

		tuint32			m_uIDGenerator;

		std::map<tuint32,Connection*>	m_kOnlineUser;
	};
}

#endif //_NET_SERVICE_H_