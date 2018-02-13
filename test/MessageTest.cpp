#include "MessageTest.h"

namespace driver
{
	M_REQ_Login::M_REQ_Login()
	{
	}
	M_REQ_Login::~M_REQ_Login()
	{

	}

	bool M_REQ_Login::Encode(tchar* pBuff, tint32& nBufSize) 
	{
		if(!m_MessageData.SerializePartialToArray(pBuff,m_MessageData.ByteSize()))
		{
			return false;
		}
		nBufSize = m_MessageData.ByteSize();

		return true;
	}

	bool M_REQ_Login::Decode(const tchar* pBuff, tint32 sSize) 
	{
		if(!m_MessageData.ParseFromArray(pBuff,sSize))
		{
			return false;

		}
		return true;
	}

	M_RET_Login::M_RET_Login()
	{
	}
	M_RET_Login::~M_RET_Login()
	{

	}

	bool M_RET_Login::Encode(tchar* pBuff, tint32& nBufSize) 
	{ 
		if(!m_MessageData.SerializePartialToArray(pBuff,m_MessageData.ByteSize()))
		{
			return false;
		}
		nBufSize = m_MessageData.ByteSize();

		return true;
	}

	bool M_RET_Login::Decode(const tchar* pBuff, tint32 sSize) 
	{
		if(!m_MessageData.ParseFromArray(pBuff,sSize))
		{
			return false;

		}
		return true;
	}
}
