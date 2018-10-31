#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common/driver.h"

namespace driver
{
#define DECL_RTTI_NAME( name ) \
	virtual const char* GetRTTIName() const\
	{\
	static const char* s_local_name = (""#name"");\
	return s_local_name;\
};\

	/**
	为所有的类提供一个公共的基类
	*/
	class Object
	{
	public:
		Object(){};
		virtual ~Object(){};
		DECL_RTTI_NAME( Object )
	};
}


#endif //_OBJECT_H_