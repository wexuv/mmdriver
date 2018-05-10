#include "NetService.h"
#include "PacketTest.h"
#include "Config.h"

namespace driver
{
	
	NetService::NetService()
	{
	}

	NetService::~NetService()
	{
	}

	bool NetService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/NetService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		luaobject* pChannelKey = g_Config.GetLuaObject("MessageChannel.LOGIN_HTTP.key");
		if(pChannelKey == null_ptr)
			return false;
		m_kMCLogin2Http.InitMessageQueue(pChannelKey->ToInt());

		m_ServerSocket.SetOnConnectFun(&NetService::OnNewConnec,this);

		for(tint32 i = PACKET_ID_MIN; i < PACKET_ID_MAX; ++ i)
		{
			m_pDispatcher[i] = &NetService::HandleDefault;
		}
		m_pDispatcher[PACKET_CG_LOGIN] = &NetService::HandleUserLogin;

		return true;
	}

	void NetService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);

		if (!m_ServerSocket.valid())
		{
			luaobject* pIp = g_Config.GetLuaObject("ServerSettings.Ip");
			if(pIp == null_ptr)
			{
				m_stLogEngine.log(log_mask_info, "[ServerSettings::%s] ip config error\n", __FUNCTION_NAME__);
				return;
			}
			luaobject* pPort = g_Config.GetLuaObject("ServerSettings.Port");
			if(pPort == null_ptr)
			{
				m_stLogEngine.log(log_mask_info, "[ServerSettings::%s] port config error\n", __FUNCTION_NAME__);
				return;
			}

			tstring ip = pIp->ToString();
			const tint16 port = (tint16)pPort->ToInt();

			if(!m_ServerSocket.open(port,ip.c_str()))
			{
				tint32 err = socketapi::net_error_no();

				m_stLogEngine.log(log_mask_info, "[NetService::%s] open %s:%d failed,err::%d\n", __FUNCTION_NAME__,ip.c_str(),port,err);
			}
			else
			{
				m_stLogEngine.log(log_mask_info, "[NetService::%s] open %s:%d success\n", __FUNCTION_NAME__,ip.c_str(),port);
			}
		}
		else
		{
			m_ServerSocket.accept_ex(10);
			m_SocketBinder.recv();

			m_SocketBinder.send();
		}
	}

	ClientSocket* NetService::OnNewConnec(tuint32 uClientIP,tuint16 uClientPort)
	{
		if(!IPCheck(uClientIP))
			return null_ptr;

		Connection* pkConnectInfo = m_ConnectionPool.get_idle_connect();
		if(pkConnectInfo != null_ptr)
		{
			pkConnectInfo->close();
			pkConnectInfo->set_buf_size();
			pkConnectInfo->SetOnDisConnectFun(&NetService::OnDisConnect,this);
			pkConnectInfo->SetOnRecvMessageFun(&NetService::OnRecvMessage,this);
			pkConnectInfo->SetStatus(Connection::CONNECTED);

			m_SocketBinder.bind(pkConnectInfo);
		}

		return pkConnectInfo;
	}

	void NetService::OnDisConnect(ClientSocket* pkClientSocket)
	{
		if(pkClientSocket == null_ptr)
			return;

		m_SocketBinder.unbind(pkClientSocket);

		Connection* pConnect = static_cast<Connection*>(pkClientSocket);
		if(pConnect != null_ptr)
		{
			pConnect->SetStatus(Connection::UNUSED);
		}
	}

	bool NetService::IPCheck(tuint32 uIP)
	{
		//������ַ
		//A�ࣺ10.0.0.0 ~ 10.255.255.255
		//B�ࣺ172.16.0.0 ~ 172.31.255.255
		//C�ࣺ192.168.0.0 ~ 192.168.255.255
		if ((uIP >= 0x0A000000 && uIP <= 0x0AFFFFFF) ||
			(uIP >= 0xAC100000 && uIP <= 0xAC1FFFFF) ||
			(uIP >= 0xC0A80000 && uIP <= 0xC0A8FFFF) ||
			(uIP >= 0x7F000001 && uIP <= 0x7F000001))
		{
			return true;
		}

		m_stLogEngine.log(log_mask_info, "[NetService::%s] IP:%d Check faild\n", __FUNCTION_NAME__,uIP);

		return false;
	}

	tint32 NetService::OnRecvMessage(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen )
	{
		tuint32 nReadTotal = 0;

		while(true)
		{
			tuint32 nRead = PeekPacket(pkClientSocket,pRecvBuf+nReadTotal,nRecvBufLen-nReadTotal);

			if(nRead > 0)
			{
				nReadTotal += nRead;
			}
			else
			{
				break;
			}
		}

		return nReadTotal;
	}

	tuint32 NetService::PeekPacket(ClientSocket* pkClientSocket,const tchar* pInBuf, size_t nInBufLen)
	{
		if(nInBufLen < sizeof(tuint16) + sizeof(PacketID))
			return 0;

		const tchar* pReadBuf = pInBuf;
		size_t nLeftBufLen = nInBufLen;

		PacketHead kPacketHead;

		memcpy(&kPacketHead,pReadBuf,sizeof(PacketHead));

		pReadBuf += sizeof(PacketHead);
		nLeftBufLen -= sizeof(PacketHead);

		if(kPacketHead.usPacketSize > nLeftBufLen)
			return 0;

		//DecryptBuf(pBuff);

		PacketID nPacketID = kPacketHead.nPacketID;
		if(nPacketID >= 0 && nPacketID < PACKET_ID_MAX)
		{
			if(m_pDispatcher[nPacketID] != null_ptr)
			{
				(this->*m_pDispatcher[nPacketID])(pkClientSocket,kPacketHead,pReadBuf);
			}
			else
			{
				m_stLogEngine.log(log_mask_info, "[NetService::%s] unhandler packet %d\n", __FUNCTION_NAME__,nPacketID);
			}
		}
		else
		{
			m_stLogEngine.log(log_mask_info, "[NetService::%s] unknown packet %d\n", __FUNCTION_NAME__,nPacketID);
		}

		return sizeof(PacketHead) + kPacketHead.usPacketSize;
	}

	void NetService::HandleUserLogin(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		P_Login ptLogin;
		if(!ptLogin.Decode(pBuff,rkPacketHead.usPacketSize))
			return;

		printf("Server recv:%s\n",ptLogin.m_PacketData.account().c_str());

		Connection* pConnect = static_cast<Connection*>(pkClientSocket);
		if(pConnect == null_ptr)
			return;

		pConnect->SetStatus(Connection::VERIFYING);
		pConnect->m_sAccount = ptLogin.m_PacketData.account();

		//ȥ��֤��

		P_LoginRet kRet;
		kRet.m_nResult = (tint32)pkClientSocket->get_fd();

		char buf[32];
		tint32 nSize = 32;
		if(!kRet.Encode(buf+sizeof(PacketHead),nSize))
		{
			pkClientSocket->close();
			return;
		}

		PacketHead kHead;
		kHead.nPacketID = kRet.GetPacketID();
		kHead.usPacketSize = nSize;
		memcpy(buf,&kHead,sizeof(kHead));

		pkClientSocket->send_data(buf,tuint16(sizeof(PacketHead)+nSize));

		M_REQ_Login msgReqLogin;
		msgReqLogin.m_MessageData.set_account(ptLogin.m_PacketData.account());
		msgReqLogin.m_MessageData.set_validateinfo(ptLogin.m_PacketData.validateinfo());
		SendMsgToHttp(&msgReqLogin);

		__LEAVE_FUNCTION
	}
	void NetService::HandleDefault(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[NetService::%s] undefind packet id:%d,len:%d\n", __FUNCTION_NAME__,rkPacketHead.nPacketID,rkPacketHead.usPacketSize);

		__LEAVE_FUNCTION
	}

	bool NetService::SendMsgToHttp(const Message* pkMessage)
	{
		__ENTER_FUNCTION

		if(!m_kMsgEncoder.Encode(pkMessage,m_kMsgHead))
			return false;

		m_kMCLogin2Http.SendMessageOutput(m_kMsgEncoder.GetBuff(),m_kMsgEncoder.GetSize());
		return true;

		__LEAVE_FUNCTION

		return false;
	}
}
