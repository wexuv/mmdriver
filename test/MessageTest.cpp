#include "MessageTest.h"

namespace driver
{
	M_REQ_Login::M_REQ_Login()
	{
	}
	M_REQ_Login::~M_REQ_Login()
	{

	}

	bool M_REQ_Login::Encode(tchar* pBuff, tuint16& nBufSize) const
	{
		if(!m_MessageData.SerializePartialToArray(pBuff,nBufSize))
		{
			return false;
		}
		nBufSize = m_MessageData.ByteSize();

		return true;
	}

	bool M_REQ_Login::Decode(const tchar* pBuff, tuint16 nBufSize) 
	{
		if(!m_MessageData.ParseFromArray(pBuff,nBufSize))
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

	bool M_RET_Login::Encode(tchar* pBuff, tuint16& nBufSize) const
	{ 
		if(!m_MessageData.SerializePartialToArray(pBuff,nBufSize))
		{
			return false;
		}
		nBufSize = m_MessageData.ByteSize();

		return true;
	}

	bool M_RET_Login::Decode(const tchar* pBuff, tuint16 nBufSize) 
	{
		if(!m_MessageData.ParseFromArray(pBuff,nBufSize))
		{
			return false;

		}
		return true;
	}
}
