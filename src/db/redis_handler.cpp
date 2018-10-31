#include "common/driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "db/redis_handler.h"

namespace driver
{
	Redis_Handler::Redis_Handler (void):port_ (redis_default_port)
	{
		host_ip_[0]  = '\0';
		account_[0]  = '\0';
		password_[0] = '\0';
			
		m_pRedisContext = null_ptr;
	}

	Redis_Handler::~Redis_Handler (void)
	{
	}

	//打开一个Redis实例
	tint32 Redis_Handler::Open(const tchar* pszIP, tuint16 uPort)
	{
		strncpy (host_ip_,  pszIP, redis_string_length);
		port_ = uPort;

		//strncpy (account_,  user,    mysql_string_length);
		//strncpy (password_, pwd,     mysql_string_length);

		redisContext* c = redisConnect(pszIP, uPort);
		if(c && 0 == c->err )
		{
			m_pRedisContext = c;
			return success;
		}
		return fail;
	}
	//关闭
	void Redis_Handler::Close()
	{
		if(m_pRedisContext != null_ptr)
		{
			redisFree(m_pRedisContext);
		}
		m_pRedisContext = null_ptr;
	}

	//执一条Redis指令
	redisReply* Redis_Handler::ExecuteRedisCommand(const tchar *format, ...)
	{
		if(m_pRedisContext == null_ptr)
			return null_ptr;

		va_list ap;
		va_start(ap,format);

		redisReply *reply = NULL;

		while(1)
		{
			reply = (redisReply*)redisvCommand(m_pRedisContext,format,ap);
			if (NULL != reply)
			{
				break;
			}
			else
			{
				redisFree(m_pRedisContext);

				//TraceLog0("CRedisInstance::ExecuteRedisCommand 失败[IP:%s,Port:%d]\n",m_szIP,m_wPort);
			}
		}

		va_end(ap);

		return reply;
	}

	void Redis_Handler::FreeReply(redisReply*& reply)
	{
		if(NULL != reply)
		{
			freeReplyObject(reply);
			reply = NULL;
		}
	}

	redisReply* Redis_Handler::ExecuteCommand(const tchar* szCommand)
	{
		return ExecuteRedisCommand(szCommand);
	}
	redisReply* Redis_Handler::HSet(const tchar* szKey,const tchar* szField, const tchar* szValue, size_t nValueSize)
	{
		//此处严重注意,传入的二进制长度类型必须是size_t,否则在hiredis的转换中会出错
		//封装此函数，以免调用错误
		return ExecuteRedisCommand("HSET %s %s %b",szKey,szField,szValue, nValueSize);
	}
	redisReply* Redis_Handler::HGet(const tchar* szKey,const tchar* szField)
	{
		return ExecuteRedisCommand("HGET %s %s",szKey,szField);
	}
}




