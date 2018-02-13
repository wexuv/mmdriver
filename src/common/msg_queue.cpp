#include "msg_queue.h"

namespace driver
{

	bool Msg_Queue::init (char* pmem, size_t mem_size, size_t queue_size, Enm_Sharemem_Mode mem_mode)
	{
		size_t real_mem_size   = alloc_size (queue_size);
		size_t real_queue_size = queue_size + message_queue_reverse_length;
		_msg_queue_head_* head = reinterpret_cast<_msg_queue_head_*>(pmem);
		if (ESM_RESUME == mem_mode)
		{
			if (head->mem_size_   != real_mem_size ||
				head->queue_size_ != real_queue_size)
			{
				return false;
			}
		}
		else
		{
			head->mem_size_   = real_mem_size;
			head->queue_size_ = real_queue_size;
		}

		if (head->mem_size_ != mem_size)
		{
			return false;
		}

		head_ = head;
		msg_base_ = pmem + sizeof(_msg_queue_head_);

		if(ESM_INIT == mem_mode)
		{
			this->clear ();
		}

		return true;
	}

	bool Msg_Queue::append (const char* pmsg, size_t msg_length)
	{
		size_t free_size = get_free_size ();
		//不足以放下一个包
		if (free_size < (msg_length + queue_message_head_length))
		{
			return false;
		}

		size_t write = head_->write_;
		char* pdst = msg_base_ + write;
		char* psrc = reinterpret_cast<char*>(&msg_length);
		for (size_t i=0; i<queue_message_head_length; ++i)
		{
			*pdst = *psrc;
			++psrc;
			write = (write + 1) % head_->queue_size_;
			pdst = msg_base_ + write;
		}

		head_->write_ = (head_->write_ + queue_message_head_length) % head_->queue_size_;

		// 如果绕圈
		if ((write + msg_length) > head_->queue_size_)
		{
			size_t first = head_->queue_size_ - head_->write_;
			size_t second= msg_length - first;

			memcpy (pdst, pmsg, first);
			memcpy (msg_base_, (pmsg + first), second);
			head_->write_ = second;
		}
		else
		{
			memcpy (pdst, pmsg, msg_length);
			head_->write_ += msg_length;
		}

		return true;
	}

	bool Msg_Queue::pop (char* pmsg, size_t& msg_length)
	{
		size_t used_size = get_used_size ();
		size_t body_size = get_msg_length ();
		// 都不足以组成一个包
		if (min_message_chunk_size > used_size || (queue_message_head_length + body_size) > used_size)
		{
			return false;
		}

		// 传入的buff不足以放下一个包
		if (body_size > msg_length)
		{
			return false;
		}

		msg_length = body_size;
		size_t read = head_->read_;

		// 先把read移动到真实数据区
		for (size_t i=0; i<queue_message_head_length; ++i)
		{
			read = (read + 1) % head_->queue_size_;
		}

		head_->read_ = (head_->read_ + queue_message_head_length) % head_->queue_size_;

		// 如果绕圈
		if ((read + msg_length) > head_->queue_size_)
		{
			size_t first = head_->queue_size_ - read;
			size_t second= msg_length - first;

			memcpy (pmsg, (msg_base_ + read), first);
			memcpy ((pmsg + first), msg_base_, second);
			head_->read_ = second;
		}
		else
		{
			memcpy (pmsg, (msg_base_ + read), msg_length);
			head_->read_ += msg_length;
		}

		return true;
	}

}


