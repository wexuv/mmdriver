#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "types.h"
#include "define.h"
#include "lib.h"
#include "exception.h"

namespace driver
{
	enum
	{
		fail	= (tint32)-1,
		success	= (tint32)0,
	};
}

#endif //_DRIVER_H_