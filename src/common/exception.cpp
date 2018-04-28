#include "exception.h"
#include "time_ex.h"

#if defined(__WINDOWS__)
#include <Windows.h>
#endif 

namespace driver
{
	void _do_assert_(const tchar* szFile,tuint32 nline,const tchar* szFunc,const tchar* szExpr, const tchar* szMsg, bool bReThrow)
	{
		static bstMutex bstAssertMutex;
		bstMutexScopedLock LockGuard(bstAssertMutex);

		tchar szTemp[1024] = {0};
		tsnprintf(szTemp,sizeof(szTemp), "\n[%s]\n[%s][%d][%s][%s]\n[%s]",
			bReThrow ? "Assert" : "Verify",
			szFile,nline,szFunc,szExpr,szMsg);
		szTemp[sizeof(szTemp)-1] = '\0';

		tm ltm;
		_localtime(_ansitime(),ltm);

		tchar szTime[128] = {0};
		tsnprintf(szTime,sizeof(szTime), "(%.4d-%.2d-%.2d_%.2d:%.2d:%.2d)", ltm.tm_year+1900,ltm.tm_mon+1,ltm.tm_mday,ltm.tm_hour,ltm.tm_min,ltm.tm_sec);
		szTime[sizeof(szTime)-1] = '\0';

		tchar szFileName[256] = {0};
		tsnprintf(szFileName,sizeof(szFileName), "./assert.%.4d-%.2d-%.2d-%.2d.log", ltm.tm_year+1900,ltm.tm_mon+1,ltm.tm_mday,ltm.tm_hour);
		szFileName[sizeof(szFileName)-1] = '\0';

		try
		{
			FILE *f = fopen(szFileName,"a+");
			if(f)
			{
				fwrite(szTemp,1,strlen(szTemp),f);
				fwrite(szTime,1,strlen(szTime),f);
				fwrite("\n",1,1,f);
				fclose(f);
			}
		}
		catch (...)
		{
		}

#if defined(__WINDOWS__)
		::MessageBoxA(NULL,szTemp,bReThrow ? "Assert" : "Verify",MB_OK);
#elif defined(__LINUX__)
		printf("%s:%s\n",bReThrow ? "Assert" : "Verify",szTemp);
#endif

		if(bReThrow)
		{
			throw(1);
		}
	}
}