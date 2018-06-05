#ifndef _DEFINES_H_
#define _DEFINES_H_

namespace driver
{

#define MAX_FILE_NAME_LENGTH	256
#define MAX_TIME_STRING_LENGTH	32
#define	MAX_IP_LENGTH			64

#define null_ptr (0)
#define invalid_id (-1)

#define _PI		3.1415926f
#define _2PI	6.2831852f

#define _MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define _MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define _ABS(a)		(((a) > (b)) ? (a) : (b))

#define _45FLOAT2INT(val) ((val) >= 0.0f ? (static_cast<int>((val)+0.5f)) : (static_cast<int>((val)-0.5f)))

#define SAFE_DELETE(x)	\
	if((x) != null_ptr)	\
	{	\
	delete (x);	\
	(x) = null_ptr;	\
}
#define SAFE_DELETE_ARRAY(x)	\
	if((x) != null_ptr)	\
	{	\
	delete[] (x);	\
	(x) = null_ptr;	\
}

#define STRNCPY(dst, src, size)			\
	do									\
	{									\
	strncpy(dst, src, (size - 1));	\
	dst[(size - 1)] = '\0';			\
}									\
	while(0)

#if defined(__WINDOWS__)

#define __FUNCTION_NAME__	__FUNCTION__

#define tvsnprintf	_vsnprintf
#define tstricmp	_stricmp
#define tsnprintf	_snprintf
#define tatoll		_atoi64
#define access		_access

#elif defined(__LINUX__)
#define __FUNCTION_NAME__ __PRETTY_FUNCTION__

#define tvsnprintf	vsnprintf
#define tstricmp	strcasecmp
#define tsnprintf	snprintf
#define tatoll		atoll
#define access		access

#else
#error windows or linux is required.	
#endif

}

#endif //_DEFINES_H_