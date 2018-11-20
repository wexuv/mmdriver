#ifndef _PACKET_H_
#define _PACKET_H_

#include "common/types.h"
#include "common/lib.h"

namespace driver
{
	enum
	{
		PACKET_BUF_SIZE = (tuint16)0xFFFF,	//消息缓存区最大长度
	};

	typedef tuint16	PacketID;

	struct PacketHead
	{
		tuint16		m_usPacketSize;
		PacketID	m_nPacketID;

	public:
		PacketHead(void);
		virtual ~PacketHead(void) ;

	public:
		virtual tint16 GetSize() const;
		virtual bool Encode(char* pBuff, tuint16& nBufSize) const;
		virtual bool Decode(const char* pBuff, tint32 nSize);
	};

	class Packet
	{
	public:
		Packet() {};
		virtual ~Packet() {};

	public:
		virtual PacketID GetPacketID() const = 0;

		virtual tint32 GetSize() const = 0;

		virtual bool Encode(tchar* pBuff, tint32& nBufSize) const = 0;

		virtual bool Decode(const tchar* pBuff, tint32 sSize) = 0;
	};

	class PacketEncoder
	{
	public:
		PacketEncoder()
		{
			Clear();
		}
		virtual ~PacketEncoder() {};

	public:
		inline void Clear()
		{
			m_szBuf[0] = '\0';
			m_usSize = 0;
		}
		inline tchar* GetBuff() 
		{ 
			return m_szBuf; 
		}

		inline tuint16 GetSize() 
		{ 
			return m_usSize; 
		}

		bool	Encode(const Packet& rkPacket);

	private:
		tchar	m_szBuf[PACKET_BUF_SIZE];
		tuint16	m_usSize;
	};
}

#endif //_PACKET_H_