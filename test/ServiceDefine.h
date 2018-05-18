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
			LOGIN,			//��¼
			HTTP,			//http
			HTTPMANAGER,	//HTTPService���ȷ���
			MONITOR,		//���
			CLIENT,			//������������
			MAX,
		};
	};
}

#endif //_SERVICE_DEFINE_H_