#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "types.h"
#include "lib.h"

namespace driver
{
	typedef tuint16	MessageID;

	class MessageHead
	{
	public:
		tint32 m_nSize;
		MessageID m_usMessageID;
		tint16 m_nSrcServiceType;
		tint16 m_nScrServiceID;
		tint16 m_nDstServiceType;
		tint16 m_nDstServiceID;

	public:
		MessageHead(void);

		virtual ~MessageHead(void) ;

	public:
		virtual bool Encode(char* pBuff, tint32& sSize);
		virtual bool Decode(const char* pBuff, tint32 sSize);
	};

	class Message
	{
	public:
		Message();
		virtual ~Message();

	public:
		virtual MessageID GetMessageID() const = 0;

		//virtual bool Encode(char* pBuff, tuint16& sSize);
		//virtual bool Decode(const char* pBuff, tuint16 sSize);
	};

	typedef std::shared_ptr<const Message>	MessageConstPtr;

	class MessageEncode
	{
	public:
		MessageEncode();
		virtual ~MessageEncode();

	public:
		virtual bool Encode(char* pBuff, tuint16& sSize);

	private:
		//char m_MsgBuff[max_package_size];
		tuint16 m_usMsgSize;
	};

	class MessageDecoder
	{
	public:
		MessageDecoder();
		virtual ~MessageDecoder();

	public:
		bool RegistPacket(Message* pkMessage);

		virtual bool Decode(const char* pBuff, tuint16 sSize);

	};
}

#endif //_MESSAGE_H_