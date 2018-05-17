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
			m_pPacketDispatcher[i] = &NetService::HandlePacketDefault;
		}
		m_pPacketDispatcher[PACKET_CG_LOGIN] = &NetService::HandlePacketUserLogin;

		for(tint32 i = MESSAGE_ID_MIN; i < MESSAGE_ID_MAX; ++ i)
		{
			m_pMessageDispatcher[i] = &NetService::HandleMsgDefault;
		}
		m_pMessageDispatcher[MSG_HN_RET_LOGIN] = &NetService::HandleMsgUserLoginRet;

		m_uIDGenerator = 1;
		return true;
	}

	void NetService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);

		TickNetwork(rkTimeData);

		TickHttpMsg(rkTimeData);
	}

	void NetService::TickNetwork(const TimeData& rkTimeData)
	{
		if (!m_ServerSocket.valid())
		{
			luaobject* pIp = g_Config.GetLuaObject("ServerSettings.Server1.Ip");
			if(pIp == null_ptr)
			{
				m_stLogEngine.log(log_mask_info, "[ServerSettings::%s] ip config error\n", __FUNCTION_NAME__);
				return;
			}
			luaobject* pPort = g_Config.GetLuaObject("ServerSettings.Server1.Port");
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
			m_ServerSocket.recv();
		}
	}

	void NetService::TickHttpMsg(const TimeData& rkTimeData)
	{
		tchar buf[2048] = {0};
		tuint16 bufSize = 2048;

		tint32 nMsgCount = 0;
		while(m_kMCLogin2Http.RecvMessasgeInput(buf,bufSize))
		{
			MessageHead kMessageHead;
			kMessageHead.Decode(buf,bufSize);

			char* pBodyBuff = buf + sizeof(MessageHead);
			tint32 nBufSize = bufSize - sizeof(MessageHead);

			if(kMessageHead.m_nSize != nBufSize)
				return;

			MessageID msgID = kMessageHead.m_usMessageID;
			if(msgID >= 0 && msgID < MESSAGE_ID_MAX)
			{
				if(m_pPacketDispatcher[msgID] != null_ptr)
				{
					(this->*m_pMessageDispatcher[msgID])(kMessageHead,pBodyBuff);
				}
				else
				{
					m_stLogEngine.log(log_mask_info, "[NetService::%s] unhandler message %d\n", __FUNCTION_NAME__,msgID);
				}
			}
			else
			{
				m_stLogEngine.log(log_mask_info, "[NetService::%s] unknown message %d\n", __FUNCTION_NAME__,msgID);
			}

			++nMsgCount;

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
			pkConnectInfo->m_nUID = m_uIDGenerator ++;

			m_kOnlineUser.insert(std::map<tuint32,Connection*>::value_type(pkConnectInfo->m_nUID,pkConnectInfo));
		}

		return pkConnectInfo;
	}

	void NetService::OnDisConnect(ClientSocket* pkClientSocket)
	{
		if(pkClientSocket == null_ptr)
			return;

		Connection* pCon = static_cast<Connection*>(pkClientSocket);
		if(null_ptr != pCon)
		{
			tuint32 nUID = pCon->m_nUID;
			std::map<tuint32,Connection*>::iterator iter = m_kOnlineUser.find(nUID);
			if(iter != m_kOnlineUser.end())
			{
				m_kOnlineUser.erase(iter);
			}
			else
			{
				AssertEx(false,"can't find online user when disconnect");
			}
		}

		Connection* pConnect = static_cast<Connection*>(pkClientSocket);
		if(pConnect != null_ptr)
		{
			pConnect->SetStatus(Connection::UNUSED);
		}
	}

	bool NetService::IPCheck(tuint32 uIP)
	{
		//内网地址
		//A类：10.0.0.0 ~ 10.255.255.255
		//B类：172.16.0.0 ~ 172.31.255.255
		//C类：192.168.0.0 ~ 192.168.255.255
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

		if(null_ptr == pkClientSocket)
			return 0;

		const tchar* pReadBuf = pInBuf;
		tint32 nLeftBufLen = (tint32)nInBufLen;

		PacketHead kPacketHead;
		if(!kPacketHead.Decode(pReadBuf,nLeftBufLen))
			return 0;

		pReadBuf += sizeof(PacketHead);
		nLeftBufLen -= sizeof(PacketHead);

		if(kPacketHead.m_usPacketSize > nLeftBufLen)
			return 0;

		//DecryptBuf(pBuff);

		PacketID nPacketID = kPacketHead.m_nPacketID;
		if(nPacketID >= 0 && nPacketID < PACKET_ID_MAX)
		{
			Connection* pConnect = static_cast<Connection*>(pkClientSocket);
			if(pConnect == null_ptr)
				return 0;

			if(m_pPacketDispatcher[nPacketID] != null_ptr)
			{
				(this->*m_pPacketDispatcher[nPacketID])(pConnect,kPacketHead,pReadBuf);
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

		return sizeof(PacketHead) + kPacketHead.m_usPacketSize;
	}

	void NetService::HandlePacketUserLogin(Connection* pConnect,const PacketHead& rkPacketHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		if(pConnect == null_ptr)
			return;		
		
		P_Login ptLogin;
		if(!ptLogin.Decode(pBuff,rkPacketHead.m_usPacketSize))
			return;

		printf("Server recv:%s\n",ptLogin.m_PacketData.account().c_str());

		pConnect->SetStatus(Connection::VERIFYING);
		pConnect->m_sAccount = ptLogin.m_PacketData.account();

		//去认证啊
		M_REQ_Login msgReqLogin;
		msgReqLogin.m_MessageData.set_uid(pConnect->m_nUID);
		msgReqLogin.m_MessageData.set_account(ptLogin.m_PacketData.account());
		msgReqLogin.m_MessageData.set_validateinfo(ptLogin.m_PacketData.validateinfo());
		SendMsgToHttp(&msgReqLogin);

		__LEAVE_FUNCTION
	}
	void NetService::HandlePacketDefault(Connection* pConnect,const PacketHead& rkPacketHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[NetService::%s] undefind packet id:%d,len:%d\n", __FUNCTION_NAME__,rkPacketHead.m_nPacketID,rkPacketHead.m_usPacketSize);

		__LEAVE_FUNCTION
	}

	bool NetService::SendMsgToHttp(const Message* pkMessage)
	{
		__ENTER_FUNCTION

		m_kMsgHead.m_nSrcServiceType = GetServiceType();
		m_kMsgHead.m_nScrServiceID = GetServiceID();
		m_kMsgHead.m_nDstServiceType = ServiceType::HTTP;
		m_kMsgHead.m_nDstServiceID = -1;

		if(!m_kMsgEncoder.Encode(pkMessage,m_kMsgHead))
			return false;

		m_kMCLogin2Http.SendMessageOutput(m_kMsgEncoder.GetBuff(),m_kMsgEncoder.GetSize());
		return true;

		__LEAVE_FUNCTION

		return false;
	}

	void NetService::HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[NetService::%s] undefind message id:%d,len:%d,info(%d,%d,%d,%d)\n", __FUNCTION_NAME__
			,rkMsgHead.m_usMessageID,rkMsgHead.m_nSize
			,rkMsgHead.m_nSrcServiceType,rkMsgHead.m_nScrServiceID,rkMsgHead.m_nDstServiceType,rkMsgHead.m_nDstServiceID);

		__LEAVE_FUNCTION
	}

	void NetService::HandleMsgUserLoginRet(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_RET_Login msgRetLogin;
		if(!msgRetLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("User login ret:%d,%s,%d\n",msgRetLogin.m_MessageData.uid(),msgRetLogin.m_MessageData.account().c_str(),msgRetLogin.m_MessageData.result());

		Connection* pUser = GetUser(msgRetLogin.m_MessageData.uid());
		if(pUser == null_ptr)
		{
			m_stLogEngine.log(log_mask_info, "[NetService::%s] uid:%d acc:%s disconnect\n", __FUNCTION_NAME__
				,msgRetLogin.m_MessageData.uid(),msgRetLogin.m_MessageData.account().c_str());
			return;
		}

		P_LoginRet kRet;
		kRet.m_PacketData.set_result(msgRetLogin.m_MessageData.result());

		PacketEncoder encoder;
		if(encoder.Encode(kRet))
		{
			pUser->send_data(encoder.GetBuff(),encoder.GetSize());
		}

		__LEAVE_FUNCTION
	}

	Connection* NetService::GetUser(tuint32 uID)
	{
		__ENTER_FUNCTION

		std::map<tuint32,Connection*>::iterator iter = m_kOnlineUser.find(uID);
		if(iter != m_kOnlineUser.end())
		{
			return iter->second;
		}

		return null_ptr;

		__LEAVE_FUNCTION

		return null_ptr;
	}
}
