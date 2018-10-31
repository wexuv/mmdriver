#include "network/packet.h"
#include "common/define.h"
#include "common/exception.h"

namespace driver
{

	PacketHead::PacketHead()
	{
		m_usPacketSize = 0;
		m_nPacketID = -1;
	}
	PacketHead::~PacketHead()
	{

	}
	bool PacketHead::Encode(char* pBuff, tuint16& nBufSize) const
	{
		if(nBufSize < (tint32)sizeof(PacketHead))
			return false;

		memcpy(pBuff,&m_usPacketSize,sizeof(m_usPacketSize));
		pBuff += sizeof(m_usPacketSize);
		memcpy(pBuff,&m_nPacketID,sizeof(m_nPacketID));
		pBuff += sizeof(m_nPacketID);
		return true;
	}
	bool PacketHead::Decode(const char* pBuff, tint32 nSize)
	{
		if(nSize < (tint32)sizeof(m_usPacketSize) + (tint32)sizeof(m_nPacketID))
			return false;

		memcpy(&m_usPacketSize,pBuff,sizeof(m_usPacketSize));
		pBuff += sizeof(m_usPacketSize);
		memcpy(&m_nPacketID,pBuff,sizeof(m_nPacketID));
		pBuff += sizeof(m_nPacketID);
		return true;
	}

	bool PacketEncoder::Encode(const Packet& rkPacket)
	{
		tint32 nPacketLen = PACKET_BUF_SIZE - sizeof(PacketHead);
		if(!rkPacket.Encode(m_szBuf+sizeof(PacketHead),nPacketLen))
			return false;

		PacketHead kHead;
		kHead.m_nPacketID = rkPacket.GetPacketID();
		kHead.m_usPacketSize = nPacketLen;

		tuint16 usHeadLen = sizeof(PacketHead);
		if(!kHead.Encode(m_szBuf,usHeadLen))
			return false;

		m_usSize = usHeadLen + nPacketLen;
		return true;
	}
}