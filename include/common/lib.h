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
#define WIN32_LEAN_AND_MEAN
#include <direct.h>
#include <io.h>
#include <windows.h>
#elif defined(__LINUX__)
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
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
typedef boost::mutex	bstMutex;
typedef boost::mutex::scoped_lock	bstMutexScopedLock;

//////////////////////////////////////


#endif //_LIB_H_