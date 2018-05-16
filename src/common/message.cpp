#include "message.h"
#include "define.h"
#include "exception.h"

namespace driver
{

	MessageHead::MessageHead()
	{
		m_nSize	= 0;
		m_usMessageID = 0;
		m_nSrcServiceType = -1;
		m_nScrServiceID = -1;
		m_nDstServiceType = -1;
		m_nDstServiceID = -1;
	}
	MessageHead::~MessageHead()
	{

	}
	bool MessageHead::Encode(char* pBuff, tuint16& nBufSize) const
	{
		if(nBufSize < (tint32)sizeof(MessageHead))
			return false;

		memcpy(pBuff,&m_nSize,sizeof(m_nSize));
		pBuff += sizeof(m_nSize);
		memcpy(pBuff,&m_usMessageID,sizeof(m_usMessageID));
		pBuff += sizeof(m_usMessageID);
		memcpy(pBuff,&m_nSrcServiceType,sizeof(m_nSrcServiceType));
		pBuff += sizeof(m_nSrcServiceType);
		memcpy(pBuff,&m_nScrServiceID,sizeof(m_nScrServiceID));
		pBuff += sizeof(m_nScrServiceID);
		memcpy(pBuff,&m_nDstServiceType,sizeof(m_nDstServiceType));
		pBuff += sizeof(m_nDstServiceType);
		memcpy(pBuff,&m_nDstServiceID,sizeof(m_nDstServiceID));
		return true;
	}
	bool MessageHead::Decode(const char* pBuff, tint32 nSize)
	{
		if(nSize < (tint32)sizeof(m_nSize) + (tint32)sizeof(m_usMessageID))
			return false;

		memcpy(&m_nSize,pBuff,sizeof(m_nSize));
		pBuff += sizeof(m_nSize);
		memcpy(&m_usMessageID,pBuff,sizeof(m_usMessageID));
		pBuff += sizeof(m_usMessageID);
		memcpy(&m_nSrcServiceType,pBuff,sizeof(m_nSrcServiceType));
		pBuff += sizeof(m_nSrcServiceType);
		memcpy(&m_nScrServiceID,pBuff,sizeof(m_nScrServiceID));
		pBuff += sizeof(m_nScrServiceID);
		memcpy(&m_nDstServiceType,pBuff,sizeof(m_nDstServiceType));
		pBuff += sizeof(m_nDstServiceType);
		memcpy(&m_nDstServiceID,pBuff,sizeof(m_nDstServiceID));
		return true;
	}

	Message::Message()
	{

	}

	Message::~Message()
	{

	}

	bool MessageEncoder::Encode(const Message* pkMessage, MessageHead& rkHead)
	{
		if(pkMessage == null_ptr)
			return false;

		tuint16 nMessageLen = MESSAGE_BUF_SIZE - sizeof(MessageHead);
		if(!pkMessage->Encode(m_szMsgBuf+sizeof(MessageHead),nMessageLen))
			return false;

		rkHead.m_usMessageID = pkMessage->GetMessageID();
		rkHead.m_nSize = nMessageLen;

		tuint16 usHeadLen = sizeof(MessageHead);
		if(!rkHead.Encode(m_szMsgBuf,usHeadLen))
			return false;

		m_usMsgSize = usHeadLen + nMessageLen;
		return true;
	}
}