#ifndef _PACKET_TEST_H_
#define _PACKET_TEST_H_

#include "ProjectDefine.h"
#include "packet.h"
#include "Packet.pb.h"

namespace driver
{
	enum PacketDefine
	{
		PACKET_ID_MIN = 0,
		
		PACKET_CG_LOGIN,
		PACKET_GC_LOGIN,

		PACKET_ID_MAX,
	};

	class P_Login : public Packet
	{
	public:
		P_Login();
		virtual ~P_Login();

	public:
		PacketID GetPacketID() const {return PACKET_CG_LOGIN;};

		virtual bool Encode(tchar* pBuff, tint32& nBufSize);

		virtual bool Decode(const tchar* pBuff, tint32 sSize);

	public:
		google::protobuf::CG_LOGIN	m_PacketData;
	};

	class P_LoginRet : public Packet
	{
	public:
		P_LoginRet();
		virtual ~P_LoginRet();

	public:
		PacketID GetPacketID() const {return PACKET_GC_LOGIN;};

		virtual bool Encode(tchar* pBuff, tint32& nBufSize);

		virtual bool Decode(const tchar* pBuff, tint32 sSize);

	public:
		tint32 m_nResult;
	};

}

#endif //_PACKET_TEST_H_