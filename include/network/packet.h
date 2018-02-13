#ifndef _PACKET_H_
#define _PACKET_H_

#include "types.h"
#include "lib.h"

namespace driver
{
	typedef tuint16	PacketID;

	struct PacketHead
	{
		tuint16 usPacketSize;
		PacketID nPacketID;
	};

	class Packet
	{
	public:
		Packet() {};
		virtual ~Packet() {};

	public:
		virtual PacketID GetPacketID() const = 0;

		virtual bool Encode(tchar* pBuff, tint32& nBufSize) = 0;

		virtual bool Decode(const tchar* pBuff, tint32 sSize) = 0;
	};
}

#endif //_PACKET_H_