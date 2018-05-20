#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "log.h"
#include "ProjectDefine.h"
#include "msg_channel.h"
#include "MessageTest.h"

namespace driver
{
	class HttpService : public Service
	{
		friend class HttpManagerService;
	public:
		HttpService();
		virtual ~HttpService();

	protected:
		tint32	GetServiceType() {return ServiceType::HTTP;};

		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

		bool		IsFree() {return m_bFree;}; 

	private:
		virtual bool InitCurl();
		virtual tint32 OnHttpReceive(tchar* pBuf, tint32 nSize);

		static size_t _OnHttpReceive(char* ptr,size_t size,size_t nmemb, void* userdata);

		void Verify(tuint32 uid, const tstring& strAccount,const tstring& strValidateInfo);

		bool SendMsgToNetServer(const Message* pkMessage);

	//message handler
	private:
		void HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff);
		void HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCHttp2Login;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;

		MessageHandler<HttpService>::Handler	m_pMessageDispatcher[MESSAGE_ID_MAX];

		CURL*		m_pCurl;

		std::string	m_szReceiveData;

		bool		m_bFree;
	};
}

#endif //_HTTP_SERVICE_H_