#ifndef _SERVICE_DEFINE_H_
#define _SERVICE_DEFINE_H_

#include "driver.h"

namespace driver
{
	class ServiceType
	{
	public:
		enum
		{
			INVALID = -1,
			LOGIN,	//登录
			HTTP,	//http
			MONITOR,//监控
			CLIENT,	//主动发起连接
			MAX,
		};
	};
}

#endif //_SERVICE_DEFINE_H_