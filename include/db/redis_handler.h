#ifndef _REDIS_HANDLER_H_
#define _REDIS_HANDLER_H_

#include "driver.h"
#include <string>
#include "socketapi.h"
#include "hiredis.h"

namespace driver
{
	class Redis_Handler
	{
	public:
		enum
		{
			redis_success		= 0,
			redis_default_port	= 6379,
			redis_string_length	= 128,
		};

	private:
		//redis��IP��ַ
		char host_ip_[redis_string_length+1];
		//redis�Ķ˿ں�
		u_short port_;
		//redis���û���
		char account_[redis_string_length+1];
		//redis������
		char password_[redis_string_length+1];

		//redis������
		redisContext* m_pRedisContext;

	public:
		Redis_Handler (void);
		~Redis_Handler (void);

		//��һ��Redisʵ��
		tint32 Open(const tchar* pszIP, tuint16 uPort);
		//�ر�
		void Close();

		//ִһ��Redisָ��
		redisReply* ExecuteRedisCommand(const tchar *format, ...);

		//�ͷ�������
		void FreeReply(redisReply*&);
	};
}



#endif //_REDIS_HANDLER_H_



