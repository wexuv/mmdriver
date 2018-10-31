#include "common/time_ex.h"

namespace driver 
{
	void _Sleep(tuint32 nMilSec)
	{
#if defined(__WINDOWS__)
		Sleep(nMilSec);
#elif defined(__LINUX__)
		usleep(nMilSec * 1000);
#else
#error windows or linux is required.
#endif
	}

	void _localtime(const time_t &t1,tm &t2)
	{
#if defined(__WINDOWS__)
		tm ltm;
		localtime_s(&ltm,&t1);
		memcpy(&t2,&ltm,sizeof(tm));
#elif defined(__LINUX__)
		tm ltm;
		localtime_r(&t1,&ltm);
		memcpy(&t2,&ltm,sizeof(tm));
#else
#error windows or linux is required.
#endif
	}

	time_t _ansitime(void)
	{
		time_t t;
		time(&t);
		return t;
	}

	tuint64 ticktime()
	{
#if defined(__WINDOWS__)
		return GetTickCount();
#elif defined(__LINUX__)
		struct timeval _tcurent;
		struct timezone _tz;
		gettimeofday(&_tcurent,&_tz);
		return (tuint64)((tfloat64)_tcurent.tv_sec*1000 + (tfloat64)_tcurent.tv_usec/1000);
#else
#error windows or linux is required.
#endif
	}

	tchar* get_string_date_time(const time_t time, tchar* pStr,tint32 nSize)
	{
		if (NULL == pStr)
		{
			return NULL;
		}

		struct tm result;
		_localtime(time, result);

		tsnprintf(pStr, nSize, "%04d-%02d-%02d %02d:%02d:%02d",
			result.tm_year + 1900, result.tm_mon + 1, result.tm_mday,
			result.tm_hour, result.tm_min, result.tm_sec);

		return pStr;
	}

	tchar* get_string_current_date(tchar* pStr,tint32 nSize)
	{
		if (null_ptr == pStr)
		{
			return null_ptr;
		}

		struct tm result;
		_localtime(_ansitime(),result);

		tsnprintf(pStr, nSize, "%04d-%02d-%02d", result.tm_year + 1900, result.tm_mon + 1, result.tm_mday);
		return pStr;
	}

	tchar* get_string_current_date_time(tchar* pStr,tint32 nSize)
	{
		return get_string_date_time(_ansitime(), pStr, nSize);
	}

	bool get_time_info(TimeData& out)
	{
		struct tm result;

		out.m_nAnsiTime = _ansitime();
		out.m_nMiliSec = ticktime();

		_localtime(out.m_nAnsiTime, result);

		out.m_nYear	= result.tm_year + 1900;
		out.m_nMon	= result.tm_mon + 1;
		out.m_nDay	= result.tm_mday;
		out.m_nHour	= result.tm_hour;
		out.m_nMin	= result.tm_min;
		out.m_nSec	= result.tm_sec;
		out.m_nWeekDay = result.tm_wday;

		return true;
	}
}