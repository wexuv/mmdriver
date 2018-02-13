#include "share_memory_api.h"
#include "exception.h"

#if defined(__WINDOWS__)
#include <windows.h>
#elif defined(__LINUX__)
#include <sys/ipc.h>
#include <sys/shm.h>
#else
#error windows or linux is required.
#endif


namespace driver {

	namespace sharememory_api{

		SMHandle	CreateShareMem(tuint32 uiKey,size_t uiSize)
		{
			__ENTER_FUNCTION

#if defined(__WINDOWS__)

			char keybuf[64];
			memset(keybuf,0,64);
			sprintf_s(keybuf,"%u",uiKey);
			return CreateFileMapping( (HANDLE)0xFFFFFFFFFFFFFFFF,NULL,PAGE_READWRITE,0,(DWORD)uiSize,keybuf);

#elif defined(__LINUX__)

			SMHandle hd = shmget(uikey,uiSize,IPC_CREAT|IPC_EXCL|0666);
			return hd;
#else
#error windows or linux is required.
#endif

			__LEAVE_FUNCTION

			return INVALID_SM_HANDLE;
		}

		SMHandle	OpenShareMem(tuint32 uiKey,size_t uiSize)
		{
			__ENTER_FUNCTION

#if defined(__WINDOWS__)

			char keybuf[64];
			memset(keybuf,0,64);
			sprintf_s(keybuf,"%u",uiKey);
			return OpenFileMapping( FILE_MAP_ALL_ACCESS ,TRUE ,keybuf);

#elif defined(__LINUX__)

			SMHandle hd = shmget(uikey,uiSize,0666);
			return hd;
#else
#error windows or linux is required.
#endif

			__LEAVE_FUNCTION

			return INVALID_SM_HANDLE;
		}

		char*		MapShareMem(SMHandle handle)
		{
			__ENTER_FUNCTION

#if defined(__WINDOWS__)

			return (char*)MapViewOfFile(handle,FILE_MAP_ALL_ACCESS,0,0,0);

#elif defined(__LINUX__)

			return (char*)shmat(handle,0,0);
#else
#error windows or linux is required.
#endif

			__LEAVE_FUNCTION

			return 0;
		}

		void		UnMapShareMem(char* pMemPtr)
		{
			__ENTER_FUNCTION

#if defined(__WINDOWS__)

			UnmapViewOfFile(pMemPtr);

#elif defined(__LINUX__)

			shmdt(pMemPtr);
#else
#error windows or linux is required.
#endif

			__LEAVE_FUNCTION
		}

		void		CloseShareMem(SMHandle handle)
		{
			__ENTER_FUNCTION

#if defined(__WINDOWS__)

			CloseHandle(handle);

#elif defined(__LINUX__)

			shmctl(handle,IPC_RMID,0);
#else
#error windows or linux is required.
#endif

			__LEAVE_FUNCTION
		}
	}
}
