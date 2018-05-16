#include "PacketTest.h"

namespace driver
{

	P_Login::P_Login()
	{
	}
	P_Login::~P_Login()
	{

	}

	bool P_Login::Encode(tchar* pBuff, tint32& nBufSize) const
	{ 
		if(!m_PacketData.SerializePartialToArray(pBuff,m_PacketData.ByteSize()))
		{
			return false;
		}
		nBufSize = m_PacketData.ByteSize();

		return true;
	}

	bool P_Login::Decode(const tchar* pBuff, tint32 sSize) 
	{
		if(!m_PacketData.ParseFromArray(pBuff,sSize))
		{
			return false;
		
		}
		return true;
	}

	P_LoginRet::P_LoginRet()
	{
	}
	P_LoginRet::~P_LoginRet()
	{

	}

	bool P_LoginRet::Encode(tchar* pBuff, tint32& nBufSize) const
	{ 
		if(!m_PacketData.SerializePartialToArray(pBuff,m_PacketData.ByteSize()))
		{
			return false;
		}
		nBufSize = m_PacketData.ByteSize();

		return true;
	}

	bool P_LoginRet::Decode(const tchar* pBuff, tint32 sSize) 
	{
		if(!m_PacketData.ParseFromArray(pBuff,sSize))
		{
			return false;
		}
		return true;
	}
}
