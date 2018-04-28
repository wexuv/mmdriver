#ifndef _MESSAGE_CHANNEL_H_
#define _MESSAGE_CHANNEL_H_

#include "types.h"
#include "lib.h"
#include "message.h"
#include "msg_queue.h"

namespace driver
{

#define MSG_QUEUE_SIZE_DEFAULT		0x10000000		// 16M

	class MessageChannel
	{
	public:
		MessageChannel();
		virtual ~MessageChannel();

	public:
		bool InitMessageQueue(tuint32 unKey, tint32 nInputSize = MSG_QUEUE_SIZE_DEFAULT,tint32 nOutputSize = MSG_QUEUE_SIZE_DEFAULT);

		bool SendMessageInput(const tchar* pMsgBuff, tuint16 usMsgSize);
		bool RecvMessageOutput(tchar* pMsgBuff, tuint16& usMsgSize);
		bool SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize);
		bool RecvMessasgeInput (tchar* pMsgBuff, tuint16& usMsgSize);

	private:
		Share_Memory	m_kShareMemory;
		Msg_Queue		m_kMsgOutput;
		Msg_Queue		m_kMsgInput;
	};
}

#endif //_MESSAGE_CHANNEL_H_