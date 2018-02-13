#ifndef __MESSAGE_EQUEUE_H__
#define __MESSAGE_EQUEUE_H__

#include "types.h"
#include "lib.h"
#include "share_memory.h"

namespace driver
{
	struct _msg_queue_head_ 
	{
		size_t mem_size_;
		size_t queue_size_;
		volatile size_t read_;
		volatile size_t write_;
	};

	class Msg_Queue
	{
	public:

		enum
		{
			queue_message_head_length	= sizeof(size_t),					//用size_t大小的字节数来表示放入的内容长度
			min_message_chunk_size		= queue_message_head_length + 1,	//最小的消息长度为头部+1
			message_queue_reverse_length= 2*queue_message_head_length,		//用来防止read和write重叠
		};

	private:
		_msg_queue_head_* head_;
		char* msg_base_;

	public:
		size_t alloc_size (size_t queue_size)
		{
			return (sizeof(_msg_queue_head_) + queue_size + message_queue_reverse_length);
		}

		bool init (char* pmem, size_t mem_size, size_t queue_size, Enm_Sharemem_Mode mem_mode = ESM_INIT);
		
		bool append (const char* pmsg, size_t msg_length);
		bool pop (char* pmsg, size_t& msg_length);

		size_t size (void)
		{
			return head_->queue_size_ - message_queue_reverse_length;
		}

	private:
		size_t get_free_size (void)
		{
			return (head_->read_ > head_->write_) ? (head_->read_ - head_->write_ - message_queue_reverse_length) : (head_->queue_size_ - head_->write_ + head_->read_ - message_queue_reverse_length);
		}

		size_t get_used_size (void)
		{
			return (size () - get_free_size ());
		}

		size_t get_msg_length (void)
		{
			size_t msg_length = 0;
			char* pdst = reinterpret_cast<char*>(&msg_length);
			size_t read = head_->read_;
			char* psrc = msg_base_ + read;

			for (size_t i=0; i<queue_message_head_length; ++i)
			{
				*pdst = *psrc;
				++pdst;
				read = (read + 1) % head_->queue_size_;
				psrc = msg_base_ + read;
			}

			return msg_length;
		}

		bool is_full (void)
		{
			return (0 == get_free_size ());
		}

		bool is_empty (void)
		{
			return (size () == get_free_size ());
		}

		void clear (void)
		{
			head_->read_ = 0;
			head_->write_= 0;
		}

	};

}

#endif	//__MESSAGE_EQUEUE_H__
