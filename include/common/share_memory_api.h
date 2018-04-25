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

		/*����ShareMem�ڴ�
		������	key  ShareMem��ֵ
		size �����ڴ��С
		����ֵ�������ڴ��ʶ��
		*/
		SMHandle	CreateShareMem(tuint32 uiKey,size_t uiSize);

		/*�����е�ShareMem�ڴ�
		������	key  ShareMem��ֵ
		size �����ڴ��С
		����ֵ�������ڴ��ʶ��
		*/
		SMHandle	OpenShareMem(tuint32 uiKey,size_t uiSize);

		/*ӳ�䵽ShareMem�ڴ�
		������	handle  ShareMem��ʶ��
		����ֵ�������ڴ��ַ
		*/
		char*		MapShareMem(SMHandle handle);

		/*ȡ��ӳ��ShareMem�ڴ�
		������	�����ڴ��ַ
		*/
		void		UnMapShareMem(char* pMemPtr);

		/*�ر�ShareMem�ڴ�
		������	handle  ShareMem��ʶ��
		*/
		void		CloseShareMem(SMHandle handle);
	}
}

#endif // __SHARE_MEMEORY__API_H__
