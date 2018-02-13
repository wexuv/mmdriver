#include "lib.h"


//////////////////////////////////////
/*
包含C运行时库
*/


//////////////////////////////////////
/*
boost
*/
#include "libs/system/src/error_code.cpp"

#if defined(__WINDOWS__)
namespace boost
{
	void tss_cleanup_implemented() {}
}
#include "libs/thread/src/win32/thread.cpp"
#include "libs/thread/src/win32/tss_dll.cpp"
#include "libs/thread/src/win32/tss_pe.cpp"
#elif defined(__LINUX__)
#include "libs/thread/src/pthread/thread.cpp"
#include "libs/thread/src/pthread/once.cpp"
#else
#error windows or linux is required.
#endif

