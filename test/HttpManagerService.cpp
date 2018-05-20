#include "HttpManagerService.h"
#include "Config.h"

#include "MessageTest.h"

namespace driver
{
	HttpManagerService::HttpManagerService()
	{
		m_nInputMsgCount = 0;
		m_nOutputMsgCount = 0;
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

		luaobject* pChannelKey = g_Config.GetLuaObject("Service.HttpManager.msgchannel.channel1.key");
		if(pChannelKey == null_ptr)
			return false;

		m_kMsgChannel.InitMessageQueue(pChannelKey->ToInt());

		m_nInputMsgCount = 0;
		m_nOutputMsgCount = 0;

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

		//�����ⲿ��Ϣ�����䵽httpservice
		while(m_kMsgChannel.RecvMessageOutput(buf,bufSize))
		{
			MessageHead kMessageHead;
			kMessageHead.Decode(buf,bufSize);

			char* pBodyBuff = buf + sizeof(MessageHead);
			tint32 nBufSize = bufSize - sizeof(MessageHead);

			if(kMessageHead.m_nSize != nBufSize)
				return;

			//dispatch to http service
			bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
			for( ; iter != m_HttpServicePool.end(); ++ iter)
			{
				if(*iter == null_ptr)
					continue;
				if((*iter)->IsFree())
				{
					(*iter)->m_kMCHttp2Login.SendMessageOutput(buf,bufSize);
				}
			}

			++m_nInputMsgCount;
		}

		//��httpservice����Ϣ����ת����ȥ
		bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
		for( ; iter != m_HttpServicePool.end(); ++ iter)
		{
			if(*iter == null_ptr)
				continue;
			HttpService& rHttpService = *(*iter);
			while(rHttpService.m_kMCHttp2Login.RecvMessasgeInput(buf,bufSize))
			{
				MessageHead kMessageHead;
				kMessageHead.Decode(buf,bufSize);

				char* pBodyBuff = buf + sizeof(MessageHead);
				tint32 nBufSize = bufSize - sizeof(MessageHead);

				if(kMessageHead.m_nSize != nBufSize)
					return;

				m_kMsgChannel.SendMessageInput(buf,bufSize);

				++m_nOutputMsgCount;
			}
		}
	}

	void HttpManagerService::TranslateMsg(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_REQ_Login msgReqLogin;
		if(!msgReqLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("User Verification:%s,%s\n",msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		__LEAVE_FUNCTION
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
