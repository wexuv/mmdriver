#ifndef _MONITOR_SERVICE_H_
#define _MONITOR_SERVICE_H_

#include "ServiceDefine.h"
#include "service.h"
#include "common/cpumemstat.h"
#include "common/log.h"

namespace driver
{
	class MonitorService : public Service
	{
	public:
		MonitorService();
		virtual ~MonitorService();

	protected:
		virtual bool NeedSchedule();

		tint32	GetServiceType() {return ServiceType::MONITOR;};

		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

	public:
		void Log(const TimeData& rkTimeData);

	private:
		CpuMemStat m_kCpuMemStat;

		tuint64	m_nLastTickTime;

		Log_Engine	m_stLogEngine;
	};
}

#endif //_MONITOR_SERVICE_H_