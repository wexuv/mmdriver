
#include "DBService.h"
#include "Config.h"

#include "MessageTest.h"

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

		if(m_kMysqlHandler.init("192.168.137.129", "test", "123456", "mtlbbdb_2329") != success)
		{
			const char *szError = m_kMysqlHandler.get_last_error();
			m_stLogEngine.log(log_mask_info, "[DBService_::%s] mysql error:%d,%s\n", __FUNCTION_NAME__,m_kMysqlHandler.get_last_errno(),szError);
			return false;
		}

		LoadCharInfo(1);

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

	bool DBService::LoadCharInfo(tuint64 uCharGuid)
	{
		/*MySQL_Utility::_makeSQLString("select c_uid, c_type, c_name, c_career, c_weapon, c_level, c_exp, c_headpic, c_cash, c_yb, c_citizenship, c_reg_time, c_off_time, c_basic_info from t_char_basic where c_cid=%u and c_delete_flag=0", uiCharID);

		DUMP_LOG(log_mask_detail, "[DB_Role_Handler::%s]select from t_char_basic SQL: %s\n",__FUNCTION__, MySQL_Utility::_getSQLString());

		Mysql_Result kDBResult;
		u_int NumAffect = 0;
		int32_t iRet = MySQL_Utility::_executeSQL(NumAffect, kDBResult);
		if (success == iRet && NumAffect > 0 && kDBResult.fetch_next_row())
		{
		kDBResult >> rkCharInfo.m_uiUserID;
		kDBResult >> rkCharBaseInfo.m_byCharType;
		kDBResult.get_string(rkCharBaseInfo.m_szCharName, sizeof(rkCharBaseInfo.m_szCharName));
		kDBResult >> rkCharBaseInfo.m_byCareer;
		kDBResult >> rkCharBaseInfo.m_byWeapon;
		kDBResult >> rkCharBaseInfo.m_usCharLevel;
		kDBResult >> rkCharBaseInfo.m_uiCharExp;
		kDBResult >> rkCharBaseInfo.m_uiHeadPic;
		kDBResult >> rkCharBaseInfo.m_llUnBindCash;
		kDBResult >> rkCharBaseInfo.m_llYuanBaoPiao;
		kDBResult >> rkCharBaseInfo.m_iCitizenship;
		kDBResult >> rkCharBaseInfo.m_uiRegTime;
		kDBResult >> rkCharBaseInfo.m_uiLastOfflineTime;

		size_t tBlobSize = MAX_BLOB_INFO_LENGTH;
		kDBResult.get_blob(m_szBlobBuff, tBlobSize);
		if (tBlobSize > 0)
		{
		if (false == DecodeBlob(rkCharBaseInfo, m_szBlobBuff, tBlobSize))
		{
		return fail;
		}
		}

		return success;
		}
		*/
		return true;
	}
	bool DBService::UpdateCharInfo(tuint64 uCharGuid)
	{

		//uint16_t usBlobSize = MAX_BLOB_INFO_LENGTH;
		//if (false == EncodeBlob(rkCharInfo.m_kBaseInfo, m_szBlobBuff, usBlobSize))
		//{
		//	return fail;
		//}
		//MySQL_Utility::_makeRealEscapeString(m_szEscapedBlobBuff, m_szBlobBuff, usBlobSize);
		//MySQL_Utility::_makeSQLString("update t_char_basic set c_career=%d, c_level=%u, c_exp=%u, c_cash=%lld, c_yb=%lld, c_citizenship=%d, c_basic_info='%s' where c_cid=%u", 
		//	rkCharInfo.m_kBaseInfo.m_byCareer, rkCharInfo.m_kBaseInfo.m_usCharLevel, rkCharInfo.m_kBaseInfo.m_uiCharExp, rkCharInfo.m_kBaseInfo.m_llUnBindCash, rkCharInfo.m_kBaseInfo.m_llYuanBaoPiao, rkCharInfo.m_kBaseInfo.m_iCitizenship, m_szEscapedBlobBuff, uiCharID);
		//u_int NumAffect = 0;
		//u_int LastID = 0;
		//int32_t iRet = MySQL_Utility::_executeSQL(NumAffect, LastID);

		//DUMP_LOG(log_mask_detail, "[DB_Role_Handler::%s]update t_char_basic result : %d, SQL: %s\n",__FUNCTION__, iRet, MySQL_Utility::_getSQLString());

		return true;
	}
}
