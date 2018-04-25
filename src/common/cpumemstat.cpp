#include "cpumemstat.h"
#include "log.h"
#include "exception.h"
#include "time_ex.h"

#if defined(__LINUX__)
#include <unistd.h>
#include <sys/param.h>
#endif

namespace driver
{
	CpuMemStat::CpuMemStat()
	{
#if defined(__WINDOWS__)

		GetSystemTimes( &m_kPreIdleTime, &m_kPreKernelTime, &m_kPreUserTime );

#elif defined(__LINUX__)
		m_pid = getpid();
		m_utime = 0;
		m_ktime = 0;
		m_wcutime = 0;
		m_wcktime = 0;
		m_vmsize = 0;
		m_vmrss = 0;
		m_lastcputime = m_utime + m_ktime + m_wcutime + m_wcktime;
#endif
		m_lastcalltime = _ansitime();
	}

	void CpuMemStat::ReBuildCpu(void)
	{
		__ENTER_PROJECT

#if defined(__LINUX__)
		
		tchar szFileName[128] = {0};
		tsnprintf(szFileName,sizeof(szFileName),"proc/%d/stat",m_pid);
		FILE* fp = fopen(szFileName,"r");
		AssertEx(fp != null_ptr,szFileName);

		tchar szBuff[4096] = {0};
		tint32 nReadLength = fread(szBuff,sizeof(tchar),sizeof(szBuff)-1,fp);
		fclose(fp);
		AssertEx(nReadLength > 0 && nReadLength < (tint32)sizeof(szBuff),"");

		for(tint32 nIndex = 0, nFileldIndex = 0; nIndex < nReadLength && nFileldIndex < STAT_COUNT;)
		{
			for( ; nIndex < nReadLength && isspace(szBuff[nIndex]); nIndex ++)
			{
				if(!(nIndex < nReadLength))
					break;

				switch(nFileldIndex++)
				{
				case STAT_UTIME:
					m_utime = strtoul(&szBuff[nIndex],null_ptr,0);
					break;
				case STAT_KTIME:
					m_ktime = strtoul(&szBuff[nIndex],null_ptr,0);
					break;
				case STAT_WCUTIME:
					m_wcutime = strtoul(&szBuff[nIndex],null_ptr,0);
					break;
				case STAT_WCKTIME:
					m_wcktime = strtoul(&szBuff[nIndex],null_ptr,0);
					break;
				default:
					break;
				}
			}
			for( ; nIndex < nReadLength && !isspace(szBuff[nIndex]); nIndex ++)
			{
				if(!(nIndex < nReadLength))
					break;
			}
		}
#endif
		__LEAVE_PROJECT
	}

	tfloat32 CpuMemStat::GetCpuRate(void)
	{
		__ENTER_PROJECT

#if defined(__WINDOWS__)

		//windows实现有问题，要求输出进程的cpu占有率，不是整体cpu使用率
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		GetSystemTimes( &idleTime, &kernelTime, &userTime );

		tint64 idle = CompareFileTime( m_kPreIdleTime,idleTime);
		tint64 kernel = CompareFileTime( m_kPreKernelTime, kernelTime);
		tint64 user = CompareFileTime(m_kPreUserTime, userTime);

		tfloat32 fCpuRate = (tfloat32)((kernel +user - idle) *100.0/(kernel+user));
		tfloat32 fCpuIdle = (tfloat32)((idle) *100.0/(kernel+user));

		m_kPreIdleTime = idleTime;
		m_kPreKernelTime = kernelTime;
		m_kPreUserTime = userTime ;

		return fCpuRate;

#elif defined(__LINUX__)

		tuint64 uCpuTime = m_utime + m_ktime + m_wcutime + m_wcktime;
		time_t uCallTime = _ansitime();

		tfloat32 fRet = 0.0f;
		tfloat32 fCpuIntelval = (tfloat32)(uCpuTime - m_lastcputime) / (tfloat32)HZ;	//HZ:linux系统的帧率(jiffers/s)不可能为 0
		tint32 nTimeInterval = uCallTime - m_lastcalltime;
		if(nTimeInterval > 0)
		{
			fRet = fCpuIntelval / (tfloat32)nTimeInterval;
		}

		m_lastcputime = uCpuTime;
		m_lastcalltime = uCallTime;
		return fRet;

#endif

		__LEAVE_PROJECT

		return 0.0f;
	}

	void CpuMemStat::ReBuildMem(void)
	{
		__ENTER_PROJECT

#if defined(__LINUX__)
		
		tchar szFileName[128] = {0};
		tsnprintf(szFileName,sizeof(szFileName),"/proc/%d/status",m_pid);
		FILE* fp = fopen(szFileName,"r");
		AssertEx(fp != null_ptr,szFileName);

		tchar szBuff[4096] = {0};
		tint32 nReadLength = fread(szBuff,sizeof(tchar),sizeof(szBuff) - 1, fp);
		fclose(fp);
		AssertEx(nReadLength >= 0 && nReadLength < (tint32)sizeof(szBuff), "");

		const tchar* pvmsize = strstr(szBuff,"VmSize:");
		if(pvmsize != null_ptr)
		{
			pvmsize += strlen("VmSize:");
			for( ; pvmsize < &(szBuff[sizeof(szBuff)-1]) && isspace(*pvmsize); pvmsize ++);
			if( pvmsize < &(szBuff[sizeof(szBuff)-1]))
			{
				m_vmsize = strtoul(pvmsize,null_ptr,0);
			}
		}

		const tchar* pvmrss = strstr(szBuff,"VmRSS:");
		if(pvmrss != null_ptr)
		{
			pvmrss += strlen("VmRSS:");
			for( ; pvmrss < &(szBuff[sizeof(szBuff)-1]) && isspace(*pvmrss); pvmrss ++);
			if( pvmrss < &(szBuff[sizeof(szBuff)-1]))
			{
				m_vmrss = strtoul(pvmrss,null_ptr,0);
			}
		}
#endif
	
		__LEAVE_PROJECT
	}

	tuint64 CpuMemStat::GetVmSize(void)
	{
#if defined(__LINUX__)
		return m_vmsize;
#endif
		return 0;
	}

	tuint64 CpuMemStat::GetVmRSS(void)
	{
#if defined(__LINUX__)
		return m_vmrss;
#endif
		return 0;
	}

#if defined(__WINDOWS__)
	tint64 CpuMemStat::CompareFileTime ( FILETIME time1, FILETIME time2 )//转换为整数时间
	{
		ULARGE_INTEGER ui1;
		ui1.LowPart = time1.dwLowDateTime;
		ui1.HighPart = time1.dwHighDateTime;

		ULARGE_INTEGER ui2;
		ui2.LowPart = time2.dwLowDateTime;
		ui2.HighPart = time2.dwHighDateTime;

		return ui2.QuadPart - ui1.QuadPart;
	}
#endif
}