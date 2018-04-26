#include "servicemanager.h"
#include "log.h"
#include "misc.h"
#include "cpumemstat.h"
#include "time_ex.h"

namespace driver
{
	ServiceManager gServiceManager;

	ServiceManager::ServiceManager()
	{
		m_bShutdown = false;
	}

	ServiceManager::~ServiceManager()
	{
		m_ServicePtrVec.clear();
	}

	bool ServiceManager::Create(tint32 nThreadCount)
	{
		__ENTER_FUNCTION

		if (!m_stLogEngine.init(0xFF, "ServiceManager"))
			return false;

		AssertEx(!m_pkThreadPool,"");
		m_pkThreadPool = ThreadPoolPtr(new ThreadPool(nThreadCount));

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] create thread pool(%d) ok\n", __FUNCTION_NAME__, nThreadCount);

		return true;
		__LEAVE_FUNCTION

		return false;
	}

	void ServiceManager::Register(Service* pService)
	{
		__ENTER_FUNCTION

		AssertEx(pService != null_ptr,"");

		ServicePtr ptr(pService);

		AssertEx(ptr,"");

		ptr->m_nServiceID = (tint32)m_ServicePtrVec.size() + 1;

		m_ServicePtrVec.push_back(ptr);

		__LEAVE_FUNCTION
	}

	void ServiceManager::Run()
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunOpenup begin\n", __FUNCTION_NAME__);
		RunLogic(SS_START);
		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunOpenup end\n", __FUNCTION_NAME__);

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunMainLogic begin\n", __FUNCTION_NAME__);
		RunLogic(SS_RUN);
		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunMainLogic end\n", __FUNCTION_NAME__);

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunShutdown begin\n", __FUNCTION_NAME__);
		RunLogic(SS_SHUTDOWN);
		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] run service manager RunShutdown end\n", __FUNCTION_NAME__);

		__LEAVE_FUNCTION
	}

	void ServiceManager::RunLogic(const ServiceState enmRunStatus)
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] Run Logic Status = %d\n", __FUNCTION_NAME__,enmRunStatus);

		SetAllServiceStatus(enmRunStatus);

		TimeData kTimeData;
		while(true)
		{
			get_time_info(kTimeData);

			Tick(kTimeData);

			if(RunLogicBreakCondition(enmRunStatus))
				break;

			_Sleep(10);
		}

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] Run Logic Status(%d) wait begin\n", __FUNCTION_NAME__,enmRunStatus);
		Wait(600);
		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] Run Logic Status(%d) wait end\n", __FUNCTION_NAME__,enmRunStatus);

		__LEAVE_FUNCTION
	}

	bool ServiceManager::RunLogicBreakCondition(const ServiceState enmRunStatus)
	{
		__ENTER_FUNCTION
		switch(enmRunStatus)
		{
		case SS_START:
			return IsAllServiceStatusEqual(SS_START_OK);
		case SS_RUN:
			return m_bShutdown;
		case SS_SHUTDOWN:
			return IsAllServiceStatusEqual(SS_SHUTDOWN_OK);
		default:
			break;
		}
		__LEAVE_FUNCTION

		return true;
	}

	void ServiceManager::Tick(const TimeData& rkTimeData)
	{
		__ENTER_FUNCTION

		AssertEx(m_pkThreadPool, "");

		for(ServicePtrVec::iterator iter = m_ServicePtrVec.begin(); iter != m_ServicePtrVec.end(); ++ iter)
		{
			const tint32 nState = (*iter)->GetRunningState();
			switch(nState)
			{
			case RS_READY:
				if((*iter)->NeedSchedule())
				{
					(*iter)->SetRunningState(RS_SCHEDULE);
					m_pkThreadPool->schedule(boost::bind(&Service::Run,*iter,rkTimeData));
				}
				break;
			case RS_SCHEDULE:
				break;
			case RS_RUNNING:
				break;
			default:
				break;
			}
		}

		__LEAVE_FUNCTION
	}

	void ServiceManager::ShutDown(tint32 nReason)
	{
		__ENTER_FUNCTION

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] receive shutdown command(%d)\n", __FUNCTION_NAME__,nReason);

		m_bShutdown = true;

		__LEAVE_FUNCTION
	}

	void ServiceManager::Wait(tint32 nSecondTimeOut)
	{
		__ENTER_FUNCTION

		AssertEx(m_pkThreadPool,"");

		boost::xtime xt;
		xtime_get(&xt,boost::TIME_UTC_);

		xt.sec += nSecondTimeOut;

		VerifyEx(m_pkThreadPool->wait(xt),"");

		__LEAVE_FUNCTION
	}

	bool ServiceManager::IsAllServiceStatusEqual(ServiceState enmStatus)
	{
		__ENTER_FUNCTION

		for(ServicePtrVec::iterator iter = m_ServicePtrVec.begin(); iter != m_ServicePtrVec.end(); ++ iter)
		{
			if((*iter)->GetServiceState() != enmStatus)
				return false;
		}
		return true;

		__LEAVE_FUNCTION

		return false;
	}
	void ServiceManager::SetAllServiceStatus(ServiceState enmStatus)
	{
		__ENTER_FUNCTION

		for(ServicePtrVec::iterator iter = m_ServicePtrVec.begin(); iter != m_ServicePtrVec.end(); ++ iter)
		{
			(*iter)->SetServiceState(enmStatus);
		}

		__LEAVE_FUNCTION
	};


	void ServiceManager::LogCpuMemStat(const tchar* szDesc)
	{
		__ENTER_PROJECT

#if defined(__LINUX__)

		static CpuMemStat s_CpuMemStat;
		s_CpuMemStat.ReBuildCpu();
		s_CpuMemStat.ReBuildMem();

		m_stLogEngine.log(log_mask_info, "[ServiceManager::%s] Desc(%s),CPU(%0.2f),VmSize(%llu),VmRss(%llu)\n",
			__FUNCTION_NAME__, 
			szDesc != null_ptr ? szDesc : "unknown",
			s_CpuMemStat.GetCpuRate(),
			s_CpuMemStat.GetVmSize(),
			s_CpuMemStat.GetVmRSS());

#endif

		__LEAVE_PROJECT
	}
}