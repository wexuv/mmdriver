
#include "timer_ex.h"
#include "exception.h"

namespace driver 
{
	enum TimerExMask
	{
		TCM_MONTH_FLAG = 1 << 27,
		TCM_WEEK_FLAG = 1 << 26,
		TCM_DAY_FLAG = 1 << 25,
		TCM_HOUR_FLAG = 1 << 24,
		TCM_MINUTE_FLAG = 1 << 23,
		TCM_MONTH = 0x780000,	//4
		TCM_WEEK = 0x70000,		//3
		TCM_DAY = 0xF800,		//5
		TCM_HOUR = 0x7C0,		//5
		TCM_MINUTE = 0x3F,		//6
		TCM_ALL = TCM_MONTH_FLAG | TCM_WEEK_FLAG | TCM_DAY_FLAG | TCM_HOUR_FLAG | TCM_MINUTE_FLAG | TCM_MONTH | TCM_WEEK | TCM_DAY | TCM_HOUR | TCM_MINUTE,
	};

	TimerEx::TimerEx()
	{
		get_time_info(m_kLastTrigger);
		m_nClock = 0;
		m_nInterval = 0;

		m_funCallback.clear();
	}
	TimerEx::~TimerEx()
	{

	}

	void TimerEx::Tick(const TimeData& rkTimeData)
	{
		if(m_nInterval > 0)
		{
			if(rkTimeData.m_nMiliSec - m_kLastTrigger.m_nMiliSec >= m_nInterval)
			{
				m_funCallback(rkTimeData);
				m_kLastTrigger = rkTimeData;
			}
		}
		if(m_nClock > 0)
		{
			if(m_kLastTrigger.m_nMon == rkTimeData.m_nMon
				&& m_kLastTrigger.m_nWeekDay == rkTimeData.m_nWeekDay
				&& m_kLastTrigger.m_nDay == rkTimeData.m_nDay
				&& m_kLastTrigger.m_nHour == rkTimeData.m_nHour
				&& m_kLastTrigger.m_nMin == rkTimeData.m_nMin)
				return;

			if(m_nClock & TCM_MONTH_FLAG)
			{
				tint32 nMonth = GetAlarmMonth();
				if(nMonth != rkTimeData.m_nMon)
					return;
			}
			if(m_nClock & TCM_WEEK_FLAG)
			{
				tint32 nWeek = GetAlarmWeek();
				if(nWeek != rkTimeData.m_nWeekDay)
					return;
			}
			if(m_nClock & TCM_DAY_FLAG)
			{
				tint32 nDay = GetAlarmDay();
				if(nDay != rkTimeData.m_nDay)
					return;
			}
			if(m_nClock & TCM_HOUR_FLAG)
			{
				tint32 nHour = GetAlarmHour();
				if(nHour != rkTimeData.m_nHour)
					return;
			}
			if(m_nClock & TCM_MINUTE_FLAG)
			{
				tint32 nMinute = GetAlarmMinute();
				if(nMinute != rkTimeData.m_nMin)
					return;
			}
			
			m_funCallback(rkTimeData);

			m_kLastTrigger = rkTimeData;
		}
	}

	void TimerEx::SetAlarmTime(tint32 nMonth,tint32 nDay,tint32 nWeek,tint32 nHour,tint32 nMinute)
	{
		SetAlarmMonth(nMonth);	
		SetAlarmWeek(nWeek);
		SetAlarmDay(nDay);
		SetAlarmHour(nHour);
		SetAlarmMinute(nMinute);
	}

	void TimerEx::SetAlarmMonth(tint32 nMonth)
	{
		if(nMonth >= 1 && nMonth <= 12)
		{
			m_nClock = (m_nClock & (TCM_ALL & ~TCM_MONTH)) | (nMonth << 19);
			m_nClock = m_nClock | TCM_MONTH_FLAG;
		}
		else
		{
			m_nClock = m_nClock & (TCM_ALL & ~TCM_MONTH_FLAG);
		}
	}

	void TimerEx::SetAlarmWeek(tint32 nWeek)
	{
		if(nWeek >= 0 && nWeek <= 6)
		{
			m_nClock = (m_nClock & (TCM_ALL & ~TCM_WEEK)) | nWeek << 16;
			m_nClock = m_nClock | TCM_WEEK_FLAG;
		}
		else
		{
			m_nClock = m_nClock & (TCM_ALL & ~TCM_WEEK_FLAG);
		}
	}

	void TimerEx::SetAlarmDay(tint32 nDay)
	{
		if(nDay >= 1 && nDay <= 31)
		{
			m_nClock = (m_nClock & (TCM_ALL & ~TCM_DAY)) | nDay << 11;
			m_nClock = m_nClock | TCM_DAY_FLAG;
		}
		else
		{
			m_nClock = m_nClock & (TCM_ALL & ~TCM_DAY_FLAG);
		}
	}

	void TimerEx::SetAlarmHour(tint32 nHour)
	{
		if(nHour >= 0 && nHour <= 23)
		{
			m_nClock = (m_nClock & (TCM_ALL & ~TCM_HOUR)) | nHour << 6;
			m_nClock = m_nClock | TCM_HOUR_FLAG;
		}
		else
		{
			m_nClock = m_nClock & (TCM_ALL & ~TCM_HOUR_FLAG);
		}
	}

	void TimerEx::SetAlarmMinute(tint32 nMinute)
	{
		if(nMinute >= 0 && nMinute <= 59)
		{
			m_nClock = (m_nClock & (TCM_ALL & ~TCM_MINUTE)) | nMinute;
			m_nClock = m_nClock | TCM_MINUTE_FLAG;
		}
		else
		{
			m_nClock = m_nClock & (TCM_ALL & ~TCM_MINUTE_FLAG);
		}
	}

	tint32 TimerEx::GetAlarmMonth()
	{
		return (m_nClock & TCM_MONTH) >> 19;
	}

	tint32 TimerEx::GetAlarmWeek()
	{
		return (m_nClock & TCM_WEEK) >> 16;
	}

	tint32 TimerEx::GetAlarmDay()
	{
		return (m_nClock & TCM_DAY) >> 11;
	}

	tint32 TimerEx::GetAlarmHour()
	{
		return (m_nClock & TCM_HOUR) >> 6;
	}

	tint32 TimerEx::GetAlarmMinute()
	{
		return (m_nClock & TCM_MINUTE);
	}

	//ºÁÃë
	void TimerEx::SetInterval(tint32 nMiliSec)
	{
		if(nMiliSec > 0)
		{
			m_nInterval = nMiliSec;
		}
	}

	tint32 TimerEx::GetInterval()
	{
		return m_nInterval;
	}

	void TimerEx::SetCallBack(TimerCallback kFunCallBack)
	{
		m_funCallback = kFunCallBack;
	}
}

