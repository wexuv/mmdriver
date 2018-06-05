#ifndef _HTTP_MANAGER_SERVICE_H_
#define _HTTP_MANAGER_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "log.h"
#include "ProjectDefine.h"
#include "msg_channel.h"
#include "HttpService.h"

namespace driver
{
	class HttpManagerService : public Service
	{
	public:
		HttpManagerService();
		virtual ~HttpManagerService();

	public:
		HttpService* NewHttpService();

	protected:
		tint32	GetServiceType() {return ServiceType::HTTPMANAGER;};

		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

		virtual bool IsStartOK();
		virtual bool IsShutdownOK();

	private:
		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMsgChannel;

		bsvector<HttpService*>	m_HttpServicePool;

		tint32			m_nInputMsgCount;	
		tint32			m_nOutputMsgCount;
	};
}

#endif //_HTTP_MANAGER_SERVICE_H_