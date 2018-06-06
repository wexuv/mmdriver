
#include "DBService.h"
#include "Config.h"

#include "MessageTest.h"
#include "mysql_result.h"
#include "User.h"
#include "SystemError.h"

namespace driver
{
	DBService::DBService()
	{
		m_bFree = true;
	}

	DBService::~DBService()
	{
	}

	bool DBService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/DBService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		if(m_kMysqlHandler.init("192.168.137.129", "test", "123456", "mm_test") != success)
		{
			const char *szError = m_kMysqlHandler.get_last_error();
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] mysql error:%d,%s\n", __FUNCTION_NAME__,m_kMysqlHandler.get_last_errno(),szError);
			return false;
		}

		if(m_kRedisHandler.Open("192.168.137.129",6379) != success)
		{
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] open redis error\n", __FUNCTION_NAME__);
			return false;
		}

		TimerExPtr ptrTimerEx = TimerExPtr(new TimerEx());
		AssertEx(ptrTimerEx,"");

		ptrTimerEx->SetInterval(60000);
		ptrTimerEx->SetCallBack(&DBService::TickConnect,this);

		AddTimer(ptrTimerEx);

		//mysql test
		CharData kCharData;
		kCharData.m_nID = 1;
		tsnprintf(kCharData.m_szCharName,sizeof(kCharData.m_szCharName),"%s","dadada");
		kCharData.m_TitleData.set_version(1);
		kCharData.m_TitleData.add_uid(1);
		kCharData.m_TitleData.add_title("aaaaa");
		kCharData.m_TitleData.add_uid(2);
		kCharData.m_TitleData.add_title("bbbbb");

		CreateChar(1,kCharData);

		LoadCharInfo(1,kCharData);

		UpdateCharInfo(kCharData);
		//mysql test

		//redis test
		//save
		tchar szKey[32] = {0};
		tsnprintf(szKey,32,"%d",kCharData.m_nID);
		
		tint32 usBlobSize = MAX_BLOB_INFO_LENGTH;
		if (false == EncodeBlob(kCharData.m_TitleData, m_szBlobBuff, usBlobSize))
			return false;

		redisReply* pRedisReply = m_kRedisHandler.HSet("USER",szKey,(tchar*)m_szBlobBuff, usBlobSize);
		if (NULL == pRedisReply)
			return false;

		if (REDIS_REPLY_ERROR == pRedisReply->type)
		{
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] redis error:%d,%s\n", __FUNCTION_NAME__,kCharData.m_nID,pRedisReply->str);
			m_kRedisHandler.FreeReply(pRedisReply);
			return false;
		}

		m_kRedisHandler.FreeReply(pRedisReply);

		//load
		pRedisReply = m_kRedisHandler.HGet("USER",szKey);

		if (NULL == pRedisReply)
		{
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] redis execute error:%d\n",__FUNCTION_NAME__,kCharData.m_nID);
			return false;
		}

		if (REDIS_REPLY_STRING != pRedisReply->type)
		{
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] redis get data type error:%d,len:%d\n",__FUNCTION_NAME__,kCharData.m_nID,pRedisReply->type);
			m_kRedisHandler.FreeReply(pRedisReply);
			return false;
		}

		//if(pRedisReply->len != sizeof(CharData))
		//{
		//	m_stLogEngine.log(log_mask_info, "[DBService_::%s] redis get data len error:%d,len:%d\n",__FUNCTION_NAME__,kCharData.m_nID,pRedisReply->len);
		//	m_kRedisHandler.FreeReply(pRedisReply);
		//	return false;
		//}

		CharData kLoadData;
		if (false == DecodeBlob(kLoadData.m_TitleData, pRedisReply->str, pRedisReply->len))
		{
			return false;
		}
		//释放
		m_kRedisHandler.FreeReply(pRedisReply);
		//redis test

		luaobject* pChannelKey = g_Config.GetLuaObject("Service.DBService.msgchannel.channel1.key");
		if(pChannelKey == null_ptr)
			return false;

		m_kMCHttp2Login.InitMessageQueue(GetServiceID()*10000 + pChannelKey->ToInt());

		for(tint32 i = MESSAGE_ID_MIN; i < MESSAGE_ID_MAX; ++ i)
		{
			m_pMessageDispatcher[i] = &DBService::HandleMsgDefault;
		}
		m_pMessageDispatcher[MSG_NH_REQ_LOGIN] = &DBService::HandleMsgUserLogin;

		m_bFree = true;



		return true;
	}

	bool DBService::Shutdown()
	{
		m_kMysqlHandler.fini();

		m_kRedisHandler.Close();

		return Service::Shutdown();
	}

	void DBService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);

		tchar buf[2048] = {0};
		tuint16 bufSize = 2048;

		tint32 nMsgCount = 0;
		while(m_kMCHttp2Login.RecvMessageOutput(buf,bufSize))
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
				if(m_pMessageDispatcher[msgID] != null_ptr)
				{
					(this->*m_pMessageDispatcher[msgID])(kMessageHead,pBodyBuff);
				}
				else
				{
					m_stLogEngine.log(log_mask_info, "[DBService_::%s] unhandler message %d\n", __FUNCTION_NAME__,msgID);
				}
			}
			else
			{
				m_stLogEngine.log(log_mask_info, "[DBService_::%s] unknown message %d\n", __FUNCTION_NAME__,msgID);
			}

			++nMsgCount;
		}
	}

	void DBService::TickConnect(const TimeData& rkTimeData)
	{
		redisReply *pRedisReply = m_kRedisHandler.ExecuteCommand("TIME");
		if(pRedisReply)
		{
			m_kRedisHandler.FreeReply(pRedisReply);
		}
		else
		{
			m_kRedisHandler.Close();
			m_kRedisHandler.Open("192.168.137.129",6379);
		}
	}

	void DBService::HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

			m_stLogEngine.log(log_mask_info, "[DBService_::%s] undefind message id:%d,len:%d,info(%d,%d,%d,%d)\n", __FUNCTION_NAME__
			,rkMsgHead.m_usMessageID,rkMsgHead.m_nSize
			,rkMsgHead.m_nSrcServiceType,rkMsgHead.m_nScrServiceID,rkMsgHead.m_nDstServiceType,rkMsgHead.m_nDstServiceID);

		__LEAVE_FUNCTION
	}

	void DBService::HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_REQ_Login msgReqLogin;
		if(!msgReqLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("User Verification:%s,%s\n",msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		__LEAVE_FUNCTION
	}

	bool DBService::SendMsgToNetServer(const Message* pkMessage)
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

	bool DBService::RecvMessasgeInput(tchar* pMsgBuff, tuint16& usMsgSize)
	{
		__ENTER_FUNCTION

		m_bFree = true;

		return m_kMCHttp2Login.RecvMessasgeInput(pMsgBuff,usMsgSize);
		__LEAVE_FUNCTION
		return false;
	}

	bool DBService::SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize)
	{
		__ENTER_FUNCTION

		m_bFree = false;

		return m_kMCHttp2Login.SendMessageOutput(pMsgBuff,usMsgSize);
		__LEAVE_FUNCTION
		return false;
	}

	uint32_t DBService::CreateChar (uint32_t uiUserID, const CharData& rkCharData)
	{
		tchar m_szEscapeCharName[2*CHAR_NAME_LENGTH+1]={0};
		m_kMysqlHandler.make_real_escape_string(m_szEscapeCharName, rkCharData.m_szCharName, (tint32)strlen(rkCharData.m_szCharName));

		tint32 usBlobSize = MAX_BLOB_INFO_LENGTH;
		EncodeBlob(rkCharData.m_TitleData, m_szBlobBuff, usBlobSize);

		m_kMysqlHandler.make_real_escape_string(m_szEscapedBlobBuff, m_szBlobBuff, usBlobSize);

		// 基础信息
		m_kMysqlHandler.make_sql_string("insert into t_char (charguid, accname, charname, hp, mp, title, isvalid) values (%lu, '%s', '%s', %d, %d, '%s', 1)", 
			uiUserID, m_szEscapeCharName, m_szEscapeCharName, 100,100, m_szEscapedBlobBuff);

		m_stLogEngine.log(log_mask_detail, "[DBService_::%s]insert into t_char SQL: %s\n", __FUNCTION_NAME__, m_kMysqlHandler.get_sql_string());

		u_int NumAffect = 0;
		u_int LastID = 0;
		u_int Errno = 0;
		int32_t iRet = m_kMysqlHandler.executeSQL(NumAffect, LastID);
		if (success != iRet)
		{
			Errno = m_kMysqlHandler.get_last_errno();
			m_stLogEngine.log(log_mask_warning, "[DBService_::%s]insert into t_char fail, char name : %s, MySQL err(%d):%s.\n", __FUNCTION_NAME__, 
				rkCharData.m_szCharName, Errno,m_kMysqlHandler.get_last_error());

			if (ER_DUP_ENTRY == Errno)
			{
				return RE_LOGIN_NAME_ALREADY_USED;
			}
			else
			{
				return RE_LOGIN_CREATE_CHAR_FAIL;
			}
		}

		// 技能信息
		m_kMysqlHandler.make_sql_string("insert into t_char_skill (charguid, isvalid) values (%lu, 1)", uiUserID);
		m_kMysqlHandler.executeSQL(NumAffect, LastID);

		return RE_LOGIN_CREATE_CHAR_SUCCESS;
	}

	tint32 DBService::LoadCharInfo(tuint64 uCharGuid,CharData& rkCharData)
	{
		m_kMysqlHandler.make_sql_string("select charname,title from t_char where charguid=%lu and isvalid=1", uCharGuid);

		m_stLogEngine.log(log_mask_detail, "[DBService_::%s] select from t_char SQL: %s\n", __FUNCTION_NAME__,m_kMysqlHandler.get_sql_string());

		Mysql_Result kDBResult;
		u_int NumAffect = 0;
		int32_t iRet = m_kMysqlHandler.executeSQL(NumAffect, kDBResult);
		if (success == iRet && NumAffect > 0 && kDBResult.fetch_next_row())
		{
			//kDBResult >> rkCharData.m_nID;
			kDBResult.get_string(rkCharData.m_szCharName, sizeof(rkCharData.m_szCharName));

			size_t tBlobSize = MAX_BLOB_INFO_LENGTH;
			kDBResult.get_blob(m_szBlobBuff, tBlobSize);
			if (tBlobSize > 0)
			{
				if (false == DecodeBlob(rkCharData.m_TitleData, m_szBlobBuff, (tint32)tBlobSize))
				{
					return fail;
				}
			}
			return success;
		}

		return fail;
	}

	tint32 DBService::UpdateCharInfo(const CharData& rkCharData)
	{
		tint32 usBlobSize = MAX_BLOB_INFO_LENGTH;
		if (false == EncodeBlob(rkCharData.m_TitleData, m_szBlobBuff, usBlobSize))
		{
			return fail;
		}
		m_kMysqlHandler.make_real_escape_string(m_szEscapedBlobBuff, m_szBlobBuff, usBlobSize);
		m_kMysqlHandler.make_sql_string("update t_char set title='%s' where charguid=%lu", m_szEscapedBlobBuff, rkCharData.m_nID);
		u_int NumAffect = 0;
		u_int LastID = 0;
		int32_t iRet = m_kMysqlHandler.executeSQL(NumAffect, LastID);

		m_stLogEngine.log(log_mask_detail, "[DB_Role_Handler::%s]update t_char result : %d, SQL: %s\n",__FUNCTION_NAME__, iRet, m_kMysqlHandler.get_sql_string());

		return true;
	}
}
