#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "driver.h"

namespace driver
{
#define DECL_RTTI_NAME( name ) \
	virtual const char* GetRTTIName() const\
	{\
	static const char* s_local_name = (""#name"");\
	return s_local_name;\
};\

	/**
	Ϊ���е����ṩһ�������Ļ���
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