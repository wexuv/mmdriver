#ifndef _TIME_H_
#define _TIME_H_

#include "define.h"
#include "types.h"
#include "lib.h"

namespace driver 
{
	/*
	��λ(����)
	*/
	void _Sleep(tuint32 nMilSec);

	/*
	*/
	void _localtime(const time_t &t1, tm &t2);

	/*
	1970�����ڵ�ʱ��(��λ:��)
	*/
	time_t _ansitime(void);

	/*
	windows��Ϊϵͳ����������ʱ��,linuxΪ1970�����ڵ�ʱ��(��λ:����)
	*/
	tuint64 ticktime();

	tchar* get_string_date_time(const time_t time, tchar* pStr,tint32 nSize);

	tchar* get_string_current_date(tchar* pStr,tint32 nSize);

	tchar* get_string_current_date_time(tchar* pStr,tint32 nSize);

	//��FF(1-59),СʱFF(1-23),��FF(0-31),��F(1-12),����F(0-6)
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