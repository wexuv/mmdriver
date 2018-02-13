#ifndef __TIMER_EX_H__
#define __TIMER_EX_H__

#include "lib.h"
#include "time_ex.h"

namespace driver 
{
	class TimerEx
	{
		typedef boost::function<void(const TimeData& rkTimeData)> TimerCallback; 

	public:
		TimerEx();
		~TimerEx();

	public:
		virtual void Tick(const TimeData& rkTimeData);

	public:
		void SetAlarmTime(tint32 nMonth,tint32 nDay,tint32 nWeek,tint32 nHour,tint32 nMinute);

		//month [1,12]
		void SetAlarmMonth(tint32 nMonth);	
		//week:[0,6]
		void SetAlarmWeek(tint32 nWeek);
		//day:[1,31]
		void SetAlarmDay(tint32 nDay);
		//hour:[0,23]
		void SetAlarmHour(tint32 nHour);
		//minute:[0,59]
		void SetAlarmMinute(tint32 nMinute);

		tint32 GetAlarmMonth();
		tint32 GetAlarmWeek();
		tint32 GetAlarmDay();
		tint32 GetAlarmHour();
		tint32 GetAlarmMinute();

		//∫¡√Î
		void SetInterval(tint32 nMiliSec);	

		tint32 GetInterval();

		void SetCallBack(TimerCallback kFunCallBack);

		template<class T>
		void SetCallBack(void (T::*func_)(const TimeData& rkTimeData),T* pIns)
		{
			m_funCallback = boost::bind(func_,pIns,_1);
		}

	public:
		TimerCallback	m_funCallback;	
		TimeData		m_kLastTrigger;		
		tuint32			m_nClock;			
		tint32			m_nInterval;
	};

	typedef std::shared_ptr<TimerEx>	TimerExPtr;

}

#endif	// __TIMER_EX_H__

