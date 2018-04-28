#ifndef _MESSAGE_TEST_H_
#define _MESSAGE_TEST_H_

#include "message.h"
#include <string>
#include "Message.pb.h"

namespace driver
{
	enum MessageDefine
	{
		MESSAGE_ID_MIN = 0,
		
		//netservice <-> httpservice
		MSG_NH_REQ_LOGIN,
		MSG_HN_RET_LOGIN,
		//netservice <-> httpservice

		MESSAGE_ID_MAX,
	};

	class M_REQ_Login : public Message
	{
	public:
		M_REQ_Login();
		virtual ~M_REQ_Login();

	public:
		MessageID GetMessageID() const{ return MSG_NH_REQ_LOGIN; }

		virtual bool Encode(tchar* pBuff, tuint16& nBufSize) const;

		virtual bool Decode(const tchar* pBuff, tuint16 nBufSize);

	public:
		google::protobuf::NH_REQ_LOGIN	m_MessageData;
	};

	class M_RET_Login : public Message
	{
	public:
		M_RET_Login();
		virtual ~M_RET_Login();

	public:
		MessageID GetMessageID() const{ return MSG_HN_RET_LOGIN; }

		virtual bool Encode(tchar* pBuff, tuint16& nBufSize) const;

		virtual bool Decode(const tchar* pBuff, tuint16 nBufSize);

	public:
		google::protobuf::HN_RET_LOGIN	m_MessageData;
	};

}

#endif //_MESSAGE_TEST_H_