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
			LOGIN,			//登录
			HTTP,			//http
			HTTPMANAGER,	//HTTPService调度服务
			MONITOR,		//监控
			CLIENT,			//主动发起连接
			DBSERVICE = 5,	//DBService
			MAX,
		};
	};
}

#endif //_SERVICE_DEFINE_H_