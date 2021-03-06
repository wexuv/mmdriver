#ifndef _CLIENT_SERVICE_H_
#define _CLIENT_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "common/log.h"
#include "network/serversocket.h"
#include "ConnectionPool.h"
#include "PacketTest.h"

namespace driver
{
	class ClientService : public Service
	{
	public:
		ClientService();
		virtual ~ClientService();

	protected:
		tint32	GetServiceType() {return ServiceType::CLIENT;};

		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

	public:
		//收到消息
		tint32 OnRecvMessage(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen );

		virtual tuint32 PeekPacket(ClientSocket* pkClientSocket,const tchar* pInBuf, size_t nInBufLen);

		//链接断开
		void OnDisConnect(ClientSocket* pkClientSocket);

	protected:
		void	HeartBeat();

		bool	_sendMessage(Packet& rkPacket);

	private:
		void OnLoginRet(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff);

	private:
		ClientSocket	m_ClientSocket;

		tuint64			m_nLastTickTime;

		Log_Engine		m_stLogEngine;
	};
}

#endif //_CLIENT_SERVICE_H_