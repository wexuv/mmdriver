
#include "ClientService.h"
#include "Message.pb.h"
#include "Config.h"

namespace driver
{
	ClientService::ClientService()
	{
		m_nLastTickTime = 0;
	}

	ClientService::~ClientService()
	{
	}

	bool ClientService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/ClientService_%d",g_Config.m_strLogPath.c_str());

		m_stLogEngine.init(0xFF, szLogFile);

		m_nLastTickTime = 0;

		return true;
	}

	void ClientService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);

		if (!m_ClientSocket.valid())
		{
			if(m_ClientSocket.get_fd() == INVALID_SOCKET)
			{
				if(!m_ClientSocket.open())
				{
					m_stLogEngine.log(log_mask_info, "[ClientService::%s] open failed\n", __FUNCTION_NAME__);
					return;
				}
			}

			luaobject* pIp = g_Config.GetLuaObject("ServerSettings.Ip");
			if(pIp == null_ptr)
			{
				m_stLogEngine.log(log_mask_info, "[ClientService::%s] ip config error\n", __FUNCTION_NAME__);
				return;
			}
			luaobject* pPort = g_Config.GetLuaObject("ServerSettings.Port");
			if(pPort == null_ptr)
			{
				m_stLogEngine.log(log_mask_info, "[ClientService::%s] port config error\n", __FUNCTION_NAME__);
				return;
			}

			tstring ip = pIp->ToString();
			const tint16 port = (tint16)pPort->ToInt();

			if(!m_ClientSocket.connect(ip.c_str(),port))
			{
				tint32 err = socketapi::net_error_no();

				m_stLogEngine.log(log_mask_info, "[ClientService::%s] connect %s:%d failed,err::%d\n", __FUNCTION_NAME__,ip.c_str(),port,err);
				return;
			}

			m_ClientSocket.set_buf_size();
			m_ClientSocket.SetOnDisConnectFun(&ClientService::OnDisConnect,this);
			m_ClientSocket.SetOnRecvMessageFun(&ClientService::OnRecvMessage,this);

			m_SocketBinder.bind(&m_ClientSocket);

			m_stLogEngine.log(log_mask_info, "[ClientService::%s] connect %s:%d success\n", __FUNCTION_NAME__,ip.c_str(),port);
		}
		else
		{
			m_SocketBinder.recv();

			HeartBeat();

			m_SocketBinder.send();
		}
	}

	tint32 ClientService::OnRecvMessage(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen )
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

	tuint32 ClientService::PeekPacket(ClientSocket* pkClientSocket,const tchar* pInBuf, size_t nInBufLen)
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

		switch(kPacketHead.nPacketID)
		{
		case PACKET_GC_LOGIN:
			{
				OnLoginRet(pkClientSocket,kPacketHead,pReadBuf);
				break;
			}
		default:
			//push input
			break;
		}

		return sizeof(PacketHead) + kPacketHead.usPacketSize;
	}

	void ClientService::OnDisConnect(ClientSocket* pkClientSocket)
	{
		m_SocketBinder.unbind(&m_ClientSocket);

		//µ×²ã¸ºÔðclose;
		//m_ClientSocket.close();
	}

	void ClientService::HeartBeat()
	{
		tuint64 curTickCount = ticktime();
		if(curTickCount - m_nLastTickTime > 5000)
		{
			m_nLastTickTime = curTickCount;

			char account[256] = {0};
			sprintf(account,"%d",GetServiceID());

			P_Login kLogin;
			kLogin.m_PacketData.set_account(account);
			kLogin.m_PacketData.set_deviceid("222");
			kLogin.m_PacketData.set_gameversion(123);

			_sendMessage(kLogin);
		}

		if(rand() % 1000 < 1)
		{
			printf("rand disconnect %d\n",GetServiceID());

			m_SocketBinder.unbind(&m_ClientSocket);
			m_ClientSocket.close();
		}
	}

	bool ClientService::_sendMessage(Packet& rkPacket)
	{
		static const tint32 BufSize = 64;
		char buf[BufSize];
		tint32 nSize = 64;
		if(!rkPacket.Encode(buf+sizeof(PacketHead),nSize))
			return false;

		PacketHead kHead;
		kHead.nPacketID = rkPacket.GetPacketID();
		kHead.usPacketSize = (tuint16)nSize;

		memcpy(buf,&kHead,sizeof(PacketHead));

		//EncryptBuf(buf);

		m_ClientSocket.send_data(buf,tuint16(sizeof(PacketHead)+nSize));

		return true;
	}

	void ClientService::OnLoginRet(ClientSocket* pkClientSocket,const PacketHead& rkPacketHead,const tchar* pBuff)
	{
		P_LoginRet ptLoginRet;
		if(!ptLoginRet.Decode(pBuff,rkPacketHead.usPacketSize))
			return;

		printf("Client recv:%d\n",ptLoginRet.m_nResult);
	}

}
