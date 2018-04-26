#ifndef _TYPES_H_
#define _TYPES_H_

#include <string>
#include <iostream>

namespace driver 
{
	typedef char				tchar;
	typedef unsigned char		tbyte;

	typedef char				tint8;
	typedef unsigned char		tuint8;

	typedef short				tint16;
	typedef unsigned short		tuint16;

	typedef int					tint32;
	typedef unsigned int		tuint32;

	typedef long long			tint64;
	typedef unsigned long long	tuint64;

	typedef float				tfloat32;
	typedef double				tfloat64;

#if defined (__LP64__) || defined (__64BIT) || defined (_LP64) || (__WORDSIZE == 64) || defined (_WIN64)
	typedef tint64				tintptr;
	typedef tfloat64			tfloatptr;
#else
	typedef tint32				tintptr;
	typedef tfloat32			tfloatptr;
#endif

	typedef std::string			tstring;
	typedef std::stringstream	tstringstream;
}

#endif //_TYPES_H_