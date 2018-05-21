
#include "HttpService.h"
#include "Config.h"

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>

#include "MessageTest.h"

namespace driver
{
	HttpService::HttpService()
	{
		m_pCurl = null_ptr;
		m_bFree = true;
	}

	HttpService::~HttpService()
	{
		if(m_pCurl != null_ptr)
		{
			curl_easy_cleanup(m_pCurl);
			m_pCurl = null_ptr;
		}
	}

	bool HttpService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/HttpService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		luaobject* pChannelKey = g_Config.GetLuaObject("Service.Http.msgchannel.channel1.key");
		if(pChannelKey == null_ptr)
			return false;

		m_kMCHttp2Login.InitMessageQueue(GetServiceID()*10000 + pChannelKey->ToInt());

		if(!InitCurl())
			return false;

		for(tint32 i = MESSAGE_ID_MIN; i < MESSAGE_ID_MAX; ++ i)
		{
			m_pMessageDispatcher[i] = &HttpService::HandleMsgDefault;
		}
		m_pMessageDispatcher[MSG_NH_REQ_LOGIN] = &HttpService::HandleMsgUserLogin;

		m_bFree = true;

		return true;
	}

	bool HttpService::InitCurl()
	{
		__ENTER_FUNCTION

		AssertEx(m_pCurl == null_ptr,"");

		m_pCurl = curl_easy_init();
		AssertEx(m_pCurl != null_ptr,"");

		m_stLogEngine.log(log_mask_info,"[HttpService::%s] service(%d) curl init.\n",__FUNCTION_NAME__,GetServiceID());

		curl_version_info_data *pCurlVersion = curl_version_info(CURLVERSION_NOW);
		AssertEx(pCurlVersion != null_ptr,"")
		AssertEx(pCurlVersion->version != null_ptr,"");

		m_stLogEngine.log(log_mask_info,"[HttpService::%s] service(%d) curl version(%s).\n",__FUNCTION_NAME__,GetServiceID(),pCurlVersion->version);

		curl_easy_setopt(m_pCurl,CURLOPT_NOSIGNAL,1L);
		curl_easy_setopt(m_pCurl,CURLOPT_CONNECTTIMEOUT,10L);
		curl_easy_setopt(m_pCurl,CURLOPT_TIMEOUT,60L);
		curl_easy_setopt(m_pCurl,CURLOPT_WRITEFUNCTION,_OnHttpReceive);
		curl_easy_setopt(m_pCurl,CURLOPT_WRITEDATA,this);

		return true;
		__LEAVE_FUNCTION
		return false;
	}

	size_t HttpService::_OnHttpReceive(char* ptr,size_t size,size_t nmemb, void* userdata)
	{
		__ENTER_FUNCTION

		AssertEx(ptr != null_ptr,"");
		AssertEx(size >= 0,"");
		AssertEx(nmemb >= 0,"");
		AssertEx(userdata != null_ptr,"");

		tint32 nSize = static_cast<tint32>(size * nmemb);
		AssertEx(nSize >= 0, "");

		HttpService* pService = (HttpService*)userdata;
		AssertEx(pService != null_ptr, "");

		tint32 nRet = pService->OnHttpReceive(ptr,nSize);

		return static_cast<size_t>(nRet);

		__LEAVE_FUNCTION

		return 0;
	}

	tint32 HttpService::OnHttpReceive(tchar* pBuf, tint32 nSize)
	{
		__ENTER_FUNCTION

		AssertEx(pBuf != null_ptr, "");
		AssertEx(nSize >= 0, "");

		tint32 nSizeBefore = static_cast<tint32>(m_szReceiveData.length());
		m_szReceiveData.append(pBuf,nSize);
		tint32 nSizeAfter = static_cast<tint32>(m_szReceiveData.length());

		return nSizeAfter - nSizeBefore;

		__LEAVE_FUNCTION

		return 0;
	}

	void HttpService::Tick(const TimeData& rkTimeData)
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
					m_stLogEngine.log(log_mask_info, "[HttpService::%s] unhandler message %d\n", __FUNCTION_NAME__,msgID);
				}
			}
			else
			{
				m_stLogEngine.log(log_mask_info, "[HttpService::%s] unknown message %d\n", __FUNCTION_NAME__,msgID);
			}

			++nMsgCount;
		}
	}

	void HttpService::Verify(tuint32 uid, const tstring& strAccount,const tstring& strValidateInfo)
	{
		__ENTER_FUNCTION

		std::string szURL = "http://www.baidu.com";
		std::string szPostData = "adfcasdf";

		curl_easy_setopt(m_pCurl,CURLOPT_URL,szURL.c_str());

		curl_slist *pHeaderList = null_ptr;

		pHeaderList = curl_slist_append(pHeaderList,"Content-Type:application/json;charset=UTF-8");
		pHeaderList = curl_slist_append(pHeaderList,"Content-Encoding:gzip");

		tchar szKeyBuf[128] = {0};
		tsnprintf(szKeyBuf,sizeof(szKeyBuf),"AppKey:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szKeyBuf);

		tchar szOpCodeBuf[128] = {0};
		tsnprintf(szOpCodeBuf,sizeof(szOpCodeBuf),"opcode:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szOpCodeBuf);

		tchar szSignatureBuf[128] = {0};
		tsnprintf(szSignatureBuf,sizeof(szSignatureBuf),"Signature:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szSignatureBuf);

		tchar szLengthBuf[128] = {0};
		tsnprintf(szLengthBuf,sizeof(szLengthBuf),"Content-Length:%d",(tint32)szPostData.length());
		pHeaderList = curl_slist_append(pHeaderList,szLengthBuf);


		//此处应注意，如果没有pHeaderList,应设置null,而不是跳过设置
		//curl_easy_setopt(m_pCurl,CURLOPT_HTTPHEADER,null_ptr);
		curl_easy_setopt(m_pCurl,CURLOPT_HTTPHEADER,pHeaderList);

		curl_easy_setopt(m_pCurl,CURLOPT_POSTFIELDS,szPostData.c_str());

		m_szReceiveData.clear();

		tint32 nRet = curl_easy_perform(m_pCurl);

		curl_slist_free_all(pHeaderList);

		if(nRet != CURLE_OK)
		{
			m_stLogEngine.log(log_mask_info,"perform fail(%d)",nRet);

			//错误处理
			M_RET_Login msgRet;
			msgRet.m_MessageData.set_uid(uid);
			msgRet.m_MessageData.set_account(strAccount);
			msgRet.m_MessageData.set_result(0);

			SendMsgToNetServer(&msgRet);
			return;
		}

		try
		{
			//解析m_szReceiveData
			std::istringstream iss(m_szReceiveData);
			boost::property_tree::ptree treeRoot;
			boost::property_tree::json_parser::read_json(iss,treeRoot);

			m_stLogEngine.log(log_mask_info,m_szReceiveData.c_str());

			M_RET_Login msgRet;
			msgRet.m_MessageData.set_uid(uid);
			msgRet.m_MessageData.set_account("ddddd");
			msgRet.m_MessageData.set_result(1);

			SendMsgToNetServer(&msgRet);
		}
		catch (...)
		{
			m_stLogEngine.log(log_mask_info,"json parser error:%s",m_szReceiveData.c_str());

			M_RET_Login msgRet;
			msgRet.m_MessageData.set_uid(uid);
			msgRet.m_MessageData.set_account("ddddd");
			msgRet.m_MessageData.set_result(0);

			SendMsgToNetServer(&msgRet);
		}

		__LEAVE_FUNCTION
	}

	void HttpService::HandleMsgDefault(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

			m_stLogEngine.log(log_mask_info, "[HttpService::%s] undefind message id:%d,len:%d,info(%d,%d,%d,%d)\n", __FUNCTION_NAME__
			,rkMsgHead.m_usMessageID,rkMsgHead.m_nSize
			,rkMsgHead.m_nSrcServiceType,rkMsgHead.m_nScrServiceID,rkMsgHead.m_nDstServiceType,rkMsgHead.m_nDstServiceID);

		__LEAVE_FUNCTION
	}

	void HttpService::HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_REQ_Login msgReqLogin;
		if(!msgReqLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("User Verification:%s,%s\n",msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		Verify(msgReqLogin.m_MessageData.uid(),msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		__LEAVE_FUNCTION
	}

	bool HttpService::SendMsgToNetServer(const Message* pkMessage)
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

	bool HttpService::RecvMessasgeInput(tchar* pMsgBuff, tuint16& usMsgSize)
	{
		__ENTER_FUNCTION

		m_bFree = true;

		return m_kMCHttp2Login.RecvMessasgeInput(pMsgBuff,usMsgSize);
		__LEAVE_FUNCTION
		return false;
	}

	bool HttpService::SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize)
	{
		__ENTER_FUNCTION

		m_bFree = false;

		return m_kMCHttp2Login.SendMessageOutput(pMsgBuff,usMsgSize);
		__LEAVE_FUNCTION
		return false;
	}
}
