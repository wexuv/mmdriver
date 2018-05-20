#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "types.h"
#include "lib.h"

namespace driver
{
	enum
	{
		MESSAGE_BUF_SIZE = (tuint16)0xFFFF,	//消息缓存区最大长度
	};

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
		virtual bool Encode(char* pBuff, tuint16& nBufSize) const;
		virtual bool Decode(const char* pBuff, tint32 nSize);
	};

	class Message
	{
	public:
		Message();
		virtual ~Message();

	public:
		virtual MessageID GetMessageID() const = 0;

		virtual bool Encode(char* pBuff, tuint16& nBufSize) const = 0;
		virtual bool Decode(const char* pBuff, tuint16 nBufSize) = 0;
	};

	typedef std::shared_ptr<const Message>	MessageConstPtr;

	class MessageEncoder
	{
	public:
		MessageEncoder()
		{
			Clear();
		}
		virtual ~MessageEncoder() {};

	public:
		inline void Clear()
		{
			m_szMsgBuf[0] = '\0';
			m_usMsgSize = 0;
		}
		inline tchar* GetBuff() 
		{ 
			return m_szMsgBuf; 
		}

		inline tuint16 GetSize() 
		{ 
			return m_usMsgSize; 
		}

		bool	Encode(const Message* pkMessage, MessageHead& rkHead);

	private:
		tchar	m_szMsgBuf[MESSAGE_BUF_SIZE];
		tuint16	m_usMsgSize;
	};

	template<class T>
	class MessageHandler
	{
	public:
		typedef void (T::*Handler)(const MessageHead& rkMsgHead,const tchar* pBuff); 
	};
}

#endif //_MESSAGE_H_