#ifndef _DB_SERVICE_H_
#define _DB_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "common/log.h"
#include "ProjectDefine.h"
#include "common/msg_channel.h"
#include "MessageTest.h"
#include "db/mysql_handler.h"
#include "DBStruct.h"
#include "db/redis_handler.h"

namespace driver
{
	class DBService : public Service
	{
	public:
		DBService();
		virtual ~DBService();

	protected:
		tint32	GetServiceType() {return ServiceType::DBSERVICE;};

		virtual bool Init();
		virtual bool Shutdown();

		virtual void	Tick(const TimeData& rkTimeData);
		void			TickConnect(const TimeData& rkTimeData);

		bool			IsFree() {return m_bFree;}; 

	private:
		bool SendMsgToNetServer(const Message* pkMessage);

		bool RecvMessasgeInput(tchar* pMsgBuff, tuint16& usMsgSize);
		bool SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize);

	//message handler
	private:
		void HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff);
		void HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff);

	private:
		uint32_t CreateChar (uint32_t uiUserID, const CharData& rkCharData);
		tint32 LoadCharInfo(tuint64 uCharGuid,CharData& rkCharData);
		tint32 UpdateCharInfo(const CharData& rkCharData);

	private:
		Log_Engine		m_stLogEngine;
		MessageChannel	m_kMCHttp2Login;

		MessageHead		m_kMsgHead;
		MessageEncoder	m_kMsgEncoder;

		MessageHandler<DBService>::Handler	m_pMessageDispatcher[MESSAGE_ID_MAX];

		bool		m_bFree;

		Mysql_Handler m_kMysqlHandler;

		char m_szBlobBuff[MAX_BLOB_INFO_LENGTH];
		char m_szEscapedBlobBuff[2*MAX_BLOB_INFO_LENGTH + 1];

		Redis_Handler m_kRedisHandler;
	};
}

#endif //_DB_SERVICE_H_