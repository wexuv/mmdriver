#include "msg_channel.h"

namespace driver
{
	MessageChannel::MessageChannel()
	{
	}

	MessageChannel::~MessageChannel()
	{
		m_kShareMemory.detach_share_memory();
	}

	bool MessageChannel::InitMessageQueue(tuint32 unKey, tint32 nInputSize,tint32 nOutputSize)
	{
		size_t tAllocInputSize = m_kMsgInput.alloc_size(nInputSize);
		size_t tAllocOutputSize = m_kMsgOutput.alloc_size(nOutputSize);

		m_kShareMemory.attach_share_memory(unKey,tAllocInputSize + tAllocOutputSize);

		char* pInputMem = m_kShareMemory.create_segment(tAllocInputSize);
		if (!m_kMsgInput.init((char*)pInputMem, tAllocInputSize, nInputSize, m_kShareMemory.get_mode()))
		{
			return false;
		}

		char* pOutputMem = m_kShareMemory.create_segment(tAllocOutputSize);
		if (!m_kMsgOutput.init((char*)pOutputMem, tAllocOutputSize, nOutputSize, m_kShareMemory.get_mode()))
		{
			return false;
		}
		return true;
	}

	bool MessageChannel::SendMessageInput(const tchar* pMsgBuff, tuint16 usMsgSize)
	{
		return m_kMsgInput.append(pMsgBuff, usMsgSize);
	}
	bool MessageChannel::RecvMessageOutput(tchar* pMsgBuff, tuint16& usMsgSize)
	{
		size_t tTmpSize = usMsgSize;

		if(m_kMsgOutput.pop(pMsgBuff, tTmpSize))
		{
			usMsgSize = (tuint16)tTmpSize;
			return true;
		}

		return false;
	}

	bool MessageChannel::SendMessageOutput(const tchar* pMsgBuff, tuint16 usMsgSize)
	{
		return m_kMsgOutput.append(pMsgBuff, usMsgSize);
	}

	bool MessageChannel::RecvMessasgeInput (tchar* pMsgBuff, tuint16& usMsgSize)
	{
		size_t tTmpSize = usMsgSize;

		if(m_kMsgInput.pop(pMsgBuff, tTmpSize))
		{
			usMsgSize = (tuint16)tTmpSize;
			return true;
		}

		return false;
	}
}