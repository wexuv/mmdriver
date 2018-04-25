#ifndef _CPUMEMSTAT_H_
#define _CPUMEMSTAT_H_

#include "types.h"
#include "define.h"
#include "lib.h"

namespace driver
{
	//��ȡָ�����̵�CPUռ��ʱ����ڴ�ʹ����
	//ԭ������/proc/[pid]/stat�ļ�

	class CpuMemStat
	{
	public:
		CpuMemStat();

	public:
		void		LogCpuMemStat();

	public:
		void		ReBuildCpu(void);
		tfloat32	GetCpuRate(void);

	public:
		void		ReBuildMem(void);
		tuint64		GetVmSize(void);
		tuint64		GetVmRSS(void);

#if defined(__WINDOWS__)
	public:
		//ת��Ϊ����ʱ��
		tint64 CompareFileTime( FILETIME time1, FILETIME time2);

	private:
		FILETIME m_kPreIdleTime;
		FILETIME m_kPreKernelTime;
		FILETIME m_kPreUserTime;

#elif defined(__LINUX__)
	public:
		enum
		{
			STAT_UTIME = 13,	//the number of jiffies that this process has been scheduled in user mode
			STAT_KTIME = 14,	//the number of jiffies that this process has been scheduled in kernel mode
			STAT_WCUTIME = 15,	//the number of jiffies that this process's waited-for children have been scheduled in user mode
			STAT_WCKTIME = 16,	//the number of jiffies that this process's waited-for children have been scheduled in kernel mode

			STAT_COUNT,			//Max Count
		};

	private:
		tuint32		m_pid;
		tuint64		m_utime;
		tuint64		m_ktime;
		tuint64		m_wcutime;
		tuint64		m_wcktime;
		tuint64		m_vmsize;
		tuint64		m_vmrss;
		tuint64		m_lastcputime;
#endif 
		time_t		m_lastcalltime;
	};
}

#endif //_CPUMEMSTAT_H_