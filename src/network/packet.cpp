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
	tint16 PacketHead::GetSize() const
	{
		return sizeof(m_usPacketSize) + sizeof(m_nPacketID);
	}
	bool PacketHead::Encode(char* pBuff, tuint16& nBufSize) const
	{
		if(nBufSize < GetSize())
			return false;

		pBuff[0] = (unsigned char)(m_usPacketSize >> 8);
		pBuff[1] = (unsigned char)m_usPacketSize;

		pBuff[2] = (unsigned char)(m_nPacketID >> 8);
		pBuff[3] = (unsigned char)m_nPacketID;

		//memcpy(pBuff,&m_usPacketSize,sizeof(m_usPacketSize));
		//pBuff += sizeof(m_usPacketSize);
		//memcpy(pBuff,&m_nPacketID,sizeof(m_nPacketID));
		//pBuff += sizeof(m_nPacketID);
		return true;
	}
	bool PacketHead::Decode(const char* pBuff, tint32 nSize)
	{
		if(nSize < GetSize())
			return false;

		m_usPacketSize = pBuff[1] | ((tint16)pBuff[0] << 8);
		m_nPacketID = pBuff[3] | ((tint16)pBuff[2] << 8);

		//memcpy(&m_usPacketSize,pBuff,sizeof(m_usPacketSize));
		//pBuff += sizeof(m_usPacketSize);
		//memcpy(&m_nPacketID,pBuff,sizeof(m_nPacketID));
		//pBuff += sizeof(m_nPacketID);
		return true;
	}

	bool PacketEncoder::Encode(const Packet& rkPacket)
	{
		PacketHead kHead;

		kHead.m_nPacketID = rkPacket.GetPacketID();
		kHead.m_usPacketSize = rkPacket.GetSize() + kHead.GetSize();

		tuint16 usHeadLen = kHead.GetSize();
		if(!kHead.Encode(m_szBuf,usHeadLen))
			return false;

		tint32 nPacketLen = PACKET_BUF_SIZE - kHead.GetSize();
		if(!rkPacket.Encode(m_szBuf+kHead.GetSize(),nPacketLen))
			return false;


		m_usSize = usHeadLen + nPacketLen;
		return true;
	}
}