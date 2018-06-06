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
		//redis的IP地址
		char host_ip_[redis_string_length+1];
		//redis的端口号
		u_short port_;
		//redis的用户名
		char account_[redis_string_length+1];
		//redis的密码
		char password_[redis_string_length+1];

		//redis上下文
		redisContext* m_pRedisContext;

	public:
		Redis_Handler (void);
		~Redis_Handler (void);

		//打开一个Redis实例
		tint32 Open(const tchar* pszIP, tuint16 uPort);
		//关闭
		void Close();
		//释放上下文
		void FreeReply(redisReply*&);

	public:
		redisReply* ExecuteCommand(const tchar* szCommand);
		redisReply* HSet(const tchar* szKey,const tchar* szField, const tchar* szValue, size_t nValueSize);
		redisReply* HGet(const tchar* szKey,const tchar* szField);

	protected:
		//执一条Redis指令
		redisReply* ExecuteRedisCommand(const tchar *format, ...);
	};
}



#endif //_REDIS_HANDLER_H_




