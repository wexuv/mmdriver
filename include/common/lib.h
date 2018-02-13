#ifndef _LIB_H_
#define _LIB_H_

//////////////////////////////////////
/*
包含C运行时库
*/
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#if defined(__WINDOWS__)
#include <direct.h>
#include <io.h>
#elif defined(__LINUX__)
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <unistd.h>
#else
#error windows or linux is required.
#endif

//////////////////////////////////////


//////////////////////////////////////
/*
包含标准模板库
*/
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <exception>
#include <algorithm>
#include <sstream>
//////////////////////////////////////


//////////////////////////////////////
/*
编译期断言
*/
#define STATIC_ASSERT( B )	static_assert(B,#B)
//////////////////////////////////////

//////////////////////////////////////
/*
boost相关
*/
#define BOOST_ALL_NO_LIB


//线程相关
#include "boost/thread.hpp"
//////////////////////////////////////


#endif //_LIB_H_