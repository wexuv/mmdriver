#ifndef _TIME_H_
#define _TIME_H_

#include "define.h"
#include "types.h"
#include "lib.h"

namespace driver 
{
	/*
	单位(毫秒)
	*/
	void _Sleep(tuint32 nMilSec);

	/*
	*/
	void _localtime(const time_t &t1, tm &t2);

	/*
	1970到现在的时间(单位:秒)
	*/
	time_t _ansitime(void);

	/*
	windows下为系统启动到现在时间,linux为1970到现在的时间(单位:毫秒)
	*/
	tuint64 ticktime();

	tchar* get_string_date_time(const time_t time, tchar* pStr,tint32 nSize);

	tchar* get_string_current_date(tchar* pStr,tint32 nSize);

	tchar* get_string_current_date_time(tchar* pStr,tint32 nSize);

	//分FF(1-59),小时FF(1-23),日FF(0-31),月F(1-12),星期F(0-6)
	struct TimeData
	{
		tint32 m_nYear;		
		tint32 m_nMon;		//1-12
		tint32 m_nDay;		//1-31
		tint32 m_nHour;		//0-23
		tint32 m_nMin;		//0-59
		tint32 m_nSec;		//0-59
		tint32 m_nWeekDay;	//0-6
		time_t m_nAnsiTime;
		tuint64 m_nMiliSec;	
	};

	bool get_time_info(TimeData& out);
}

#endif //_TIME_H_