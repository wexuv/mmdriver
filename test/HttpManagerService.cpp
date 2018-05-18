#include "HttpManagerService.h"
#include "Config.h"

#include "MessageTest.h"

namespace driver
{
	HttpManagerService::HttpManagerService()
	{
	}

	HttpManagerService::~HttpManagerService()
	{
		bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
		for( ; iter != m_HttpServicePool.end(); ++ iter)
		{
			SAFE_DELETE(*iter);
		}
		m_HttpServicePool.clear();
	}

	bool HttpManagerService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/HttpManagerService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		luaobject* pChannelKey = g_Config.GetLuaObject("MessageChannel.LOGIN_HTTP.key");
		if(pChannelKey == null_ptr)
			return false;

		m_kMCHttp2Login.InitMessageQueue(pChannelKey->ToInt());

		return true;
	}

	bool HttpManagerService::IsShutdownOK()
	{
		bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
		for( ; iter != m_HttpServicePool.end(); ++ iter)
		{
			if((*iter)->NeedSchedule())
				return false;
		}
		return true;
	}

	void HttpManagerService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);

		tchar buf[2048] = {0};
		tuint16 bufSize = 2048;

		tint32 nMsgCount = 0;
		//while(m_kMCHttp2Login.RecvMessageOutput(buf,bufSize))
		//{
		//	MessageHead kMessageHead;
		//	kMessageHead.Decode(buf,bufSize);

		//	char* pBodyBuff = buf + sizeof(MessageHead);
		//	tint32 nBufSize = bufSize - sizeof(MessageHead);

		//	if(kMessageHead.m_nSize != nBufSize)
		//		return;

		//	HandleMsgUserLogin(kMessageHead,pBodyBuff);

		//	++nMsgCount;

		//}
	}

	void HttpManagerService::HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_REQ_Login msgReqLogin;
		if(!msgReqLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("User Verification:%s,%s\n",msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		__LEAVE_FUNCTION
	}

	bool HttpManagerService::SendMsgToNetServer(const Message* pkMessage)
	{
		__ENTER_FUNCTION

		m_kMsgHead.m_nSrcServiceType = GetServiceType();
		m_kMsgHead.m_nScrServiceID = GetServiceID();
		m_kMsgHead.m_nDstServiceType = ServiceType::LOGIN;
		m_kMsgHead.m_nDstServiceID = -1;

		if(!m_kMsgEncoder.Encode(pkMessage,m_kMsgHead))
			return false;

		m_kMCHttp2Login.SendMessageInput(m_kMsgEncoder.GetBuff(),m_kMsgEncoder.GetSize());
		return true;

		__LEAVE_FUNCTION

		return false;
	}

	HttpService* HttpManagerService::NewHttpService()
	{
		__ENTER_FUNCTION

		HttpService* pService = new HttpService();
		if(pService == null_ptr)
			return null_ptr;
		m_HttpServicePool.push_back(pService);
		return pService;

		__LEAVE_FUNCTION
		return null_ptr;
	}
}
