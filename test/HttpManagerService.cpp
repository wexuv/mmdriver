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

		//接受外部消息，分配到httpservice
		while(m_kMsgChannel.RecvMessageOutput(buf,bufSize))
		{
			MessageHead kMessageHead;
			kMessageHead.Decode(buf,bufSize);

			tint32 nBufSize = bufSize - sizeof(MessageHead);
			if(kMessageHead.m_nSize != nBufSize)
				return;

			//dispatch to http service
			bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
			for( ; iter != m_HttpServicePool.end(); ++ iter)
			{
				if(*iter == null_ptr)
					continue;
				//注意此处多线程IsFree的判定
				if((*iter)->IsFree())
				{
					(*iter)->SendMessageOutput(buf,bufSize);
					break;
				}
			}

			++m_nInputMsgCount;
		}

		//从httpservice收消息，再转发出去
		bsvector<HttpService*>::iterator iter = m_HttpServicePool.begin();
		for( ; iter != m_HttpServicePool.end(); ++ iter)
		{
			if(*iter == null_ptr)
				continue;
			HttpService& rHttpService = *(*iter);
			while(rHttpService.RecvMessasgeInput(buf,bufSize))
			{
				MessageHead kMessageHead;
				kMessageHead.Decode(buf,bufSize);

				tint32 nBufSize = bufSize - sizeof(MessageHead);

				if(kMessageHead.m_nSize != nBufSize)
					return;

				m_kMsgChannel.SendMessageInput(buf,bufSize);

				++m_nOutputMsgCount;
			}
		}
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
