
#include "MonitorService.h"
#include "time_ex.h"
#include "timer_ex.h"
#include "Config.h"

namespace driver
{
	MonitorService::MonitorService()
	{
	}

	MonitorService::~MonitorService()
	{
	}

	bool MonitorService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,MAX_FILE_NAME_LENGTH,"%s/MonitorService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);


		m_nLastTickTime = ticktime();

		TimerExPtr ptrTimerEx = TimerExPtr(new TimerEx());
		AssertEx(ptrTimerEx,"");

		//ptrTimerEx->SetInterval(1000);
		ptrTimerEx->SetAlarmHour(19);
		ptrTimerEx->SetAlarmMinute(7);
		//ptrTimerEx->SetCallBack(boost::bind(&MonitorService::Log,this,_1));
		ptrTimerEx->SetCallBack(&MonitorService::Log,this);

		AddTimer(ptrTimerEx);


		return true;
	}

	bool MonitorService::NeedSchedule()
	{
		tuint64 curTickCount = ticktime();
		if(curTickCount - m_nLastTickTime > 3000)
		{
			m_nLastTickTime = curTickCount;
			return Service::NeedSchedule();
		}
		return false;
	}

	void MonitorService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);
		//if(rkTimeData.m_nAnsiTime - m_nLastTickTime > 3)
		//{
		//	Log(rkTimeData);

		//	m_nLastTickTime = rkTimeData.m_nAnsiTime;
		//}
	}

	void MonitorService::Log(const TimeData& rkTimeData)
	{
		m_kCpuMemStat.ReBuildCpu();

		tfloat32 fCpuRate = m_kCpuMemStat.GetCpuRate();

		m_stLogEngine.log(log_mask_info, "[MonitorService::%s] CpuRate(%0.2f)\n", __FUNCTION_NAME__,fCpuRate);
	}
}
