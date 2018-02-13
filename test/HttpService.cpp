
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
		sprintf_s(szLogFile,"%s/HttpService_%d",g_Config.m_LogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		InitMessageQueue(123);

		if(!InitCurl())
			return false;

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

		TestCurl();

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
		tuint16 unbufSize = 2048;

		tint32 nMsgCount = 0;
		while(PopMessage(buf,unbufSize))
		{
			MessageHead kPacketHead;
			kPacketHead.Decode(buf,unbufSize);

			char* pBodyBuff = buf + sizeof(MessageHead);
			tint32 nBufSize = unbufSize - sizeof(MessageHead);

			if(kPacketHead.m_nSize > unbufSize)
				return;

			HandleMsgUserLogin(kPacketHead,pBodyBuff);

			++nMsgCount;

		}
	}

	void HttpService::TestCurl()
	{
		__ENTER_FUNCTION

		std::string szURL = "http://www.baidu.com";
		std::string szPostData = "adfcasdf";

		curl_easy_setopt(m_pCurl,CURLOPT_URL,szURL.c_str());

		curl_slist *pHeaderList = null_ptr;

		pHeaderList = curl_slist_append(pHeaderList,"Content-Type:application/json;charset=UTF-8");
		pHeaderList = curl_slist_append(pHeaderList,"Content-Encoding:gzip");

		tchar szKeyBuf[128] = {0};
		sprintf_s(szKeyBuf,"AppKey:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szKeyBuf);

		tchar szOpCodeBuf[128] = {0};
		sprintf_s(szOpCodeBuf,"opcode:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szOpCodeBuf);

		tchar szSignatureBuf[128] = {0};
		sprintf_s(szSignatureBuf,"Signature:%s","aaa");
		pHeaderList = curl_slist_append(pHeaderList,szSignatureBuf);

		tchar szLengthBuf[128] = {0};
		sprintf_s(szLengthBuf,"Content-Length:%d",szPostData.length());
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
			return;
		}

		try
		{
			//解析m_szReceiveData
			std::istringstream iss(m_szReceiveData);
			boost::property_tree::ptree treeRoot;
			boost::property_tree::json_parser::read_json(iss,treeRoot);

			m_stLogEngine.log(log_mask_info,m_szReceiveData.c_str());
		}
		catch (...)
		{
			m_stLogEngine.log(log_mask_info,"json parser error:%s",m_szReceiveData.c_str());
		}

		__LEAVE_FUNCTION
	}

	void HttpService::HandleMsgUserLogin(const MessageHead& rkMsgHead,const tchar* pBuff)
	{
		__ENTER_FUNCTION

		M_REQ_Login msgReqLogin;
		if(!msgReqLogin.Decode(pBuff,rkMsgHead.m_nSize))
			return;

		printf("%s,%s\n",msgReqLogin.m_MessageData.account().c_str(),msgReqLogin.m_MessageData.validateinfo().c_str());

		__LEAVE_FUNCTION
	}
}
