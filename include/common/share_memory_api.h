#ifndef __SHARE_MEMEORY__API_H__
#define __SHARE_MEMEORY__API_H__

#include "types.h"
#include "lib.h"

namespace driver 
{

#if defined(__WINDOWS__)
	typedef	void*	SMHandle;
#define INVALID_SM_HANDLE  ((void*)0)
#elif defined(__LINUX__)
	typedef	tint32	SMHandle;
#define INVALID_SM_HANDLE	-1
#else
#error windows or linux is required.
#endif

	namespace sharememory_api{

		/*创建ShareMem内存
		参数：	key  ShareMem键值
		size 创建内存大小
		返回值：共享内存标识符
		*/
		SMHandle	CreateShareMem(tuint32 uiKey,size_t uiSize);

		/*打开已有的ShareMem内存
		参数：	key  ShareMem键值
		size 创建内存大小
		返回值：共享内存标识符
		*/
		SMHandle	OpenShareMem(tuint32 uiKey,size_t uiSize);

		/*映射到ShareMem内存
		参数：	handle  ShareMem标识符
		返回值：共享内存地址
		*/
		char*		MapShareMem(SMHandle handle);

		/*取消映射ShareMem内存
		参数：	共享内存地址
		*/
		void		UnMapShareMem(char* pMemPtr);

		/*关闭ShareMem内存
		参数：	handle  ShareMem标识符
		*/
		void		CloseShareMem(SMHandle handle);
	}
}

#endif // __SHARE_MEMEORY__API_H__
