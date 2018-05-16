#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "log.h"
#include "ProjectDefine.h"
#include "msg_channel.h"

namespace driver
{
	class HttpService : public Service
	{
	public:
		HttpService();
		virtual ~HttpService();

	protected:
		tint32	GetServiceType() {return ServiceType::HTTP;};

		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

	private:
		virtual bool InitCurl();
		virtual tint32 OnHttpReceive(tchar* pBuf, tint32 nSize);

		static size_t _OnHttpReceive(char* ptr,size_t size,size_t nmemb, void* userdata);

		void TestCurl(tuint32 uid, const tstring& strAccount,const tstring& strValidateInfo);

		bool SendMsgToNetServer(const Message* pkMessage);

	//message handler
	private:
		void HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCHttp2Login;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;

		CURL*		m_pCurl;

		std::string	m_szReceiveData;
	};
}

#endif //_HTTP_SERVICE_H_