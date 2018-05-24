#ifndef _DB_SERVICE_H_
#define _DB_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "log.h"
#include "ProjectDefine.h"
#include "msg_channel.h"
#include "MessageTest.h"
#include "mysql_handler.h"

namespace driver
{
	class DBService : public Service
	{
	public:
		DBService();
		virtual ~DBService();

	protected:
		tint32	GetServiceType() {return ServiceType::HTTP;};

		virtual bool Init();
		virtual bool Shutdown();

		virtual void Tick(const TimeData& rkTimeData);

		bool		IsFree() {return m_bFree;}; 

	private:
		bool SendMsgToNetServer(const Message* pkMessage);

		bool RecvMessasgeInput(tchar* pMsgBuff, tuint16& usMsgSize);
		bool SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize);

	//message handler
	private:
		void HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff);
		void HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		bool LoadCharInfo(tuint64 uCharGuid);
		bool UpdateCharInfo(tuint64 uCharGuid);

	private:
		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCHttp2Login;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;

		MessageHandler<DBService>::Handler	m_pMessageDispatcher[MESSAGE_ID_MAX];

		bool		m_bFree;

		Mysql_Handler m_kMysqlHandler;
	};
}

#endif //_DB_SERVICE_H_