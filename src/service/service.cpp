#include "service.h"

namespace driver
{
	Service::Service()
	{
		m_enmRunState = RS_READY;
		m_enmServiceState = SS_NONE;

		m_nServiceID = invalid_id;

		m_nUpdateTime = 0;
	}

	Service::~Service()
	{
		m_TimerExPtrVec.clear();
		m_MessageHandlerVec.clear();
	}

	tint32 Service::GetServiceID(void)
	{
		return m_nServiceID;
	}

	void Service::SetRunningState(RunningState enmRuningState)
	{
		m_enmRunState = enmRuningState; 
	}

	RunningState Service::GetRunningState() 
	{
		return m_enmRunState;
	}

	void Service::SetServiceState(ServiceState enmServiceState)
	{
		m_enmServiceState = enmServiceState;
	}

	ServiceState Service::GetServiceState()
	{
		return m_enmServiceState;
	}

	void Service::Run(const TimeData& rkTimeData)
	{
		SetRunningState(RS_RUNNING);
		SetUpdateTime(rkTimeData.m_nMiliSec);
		__ENTER_FUNCTION

		switch(m_enmServiceState)
		{
		case SS_START:
			m_enmServiceState = SS_START_PROCESS;
			Init();
			break;
		case SS_START_PROCESS:
			if(IsStartOK())
			{
				m_enmServiceState = SS_START_OK;
			}
			break;
		case SS_START_OK:
			break;

		case SS_RUN:
			Tick(rkTimeData);
			break;

		case SS_SHUTDOWN:
			m_enmServiceState = SS_SHUTDOWN_PROCESS;
			Shutdown();
			break;

		case SS_SHUTDOWN_PROCESS:
			if(IsShutdownOK())
			{
				m_enmServiceState = SS_SHUTDOWN_OK;
			}
			break;

		case SS_SHUTDOWN_OK:
			break;

		default:
			break;
		}

		__LEAVE_FUNCTION

		SetRunningState(RS_READY);
	}

	bool Service::NeedSchedule()
	{
		return m_enmServiceState != SS_SHUTDOWN_OK;
	}

	bool Service::IsStartOK()
	{
		return true;
	}

	bool Service::IsShutdownOK()
	{
		return true;
	}

	void Service::Tick(const TimeData& rkTimeData)
	{
		for(TimerExPtrVec::iterator iter = m_TimerExPtrVec.begin(); iter != m_TimerExPtrVec.end(); ++ iter)
		{
			(*iter)->Tick(rkTimeData);
		}
	}

	bool Service::Init()
	{
		return true;
	}

	bool Service::Shutdown()
	{
		return true;
	}

	void Service::AddTimer(TimerExPtr kTimerEx)
	{		
		m_TimerExPtrVec.push_back(kTimerEx);
	}

	bool Service::RegisterMessage(MessageID nID,MessageCallback Handler)
	{
		if(nID < 0 || nID >= MessageID(-1))
			return false;

		if(nID > m_MessageHandlerVec.size())
		{
			m_MessageHandlerVec.resize(nID + 1);
		}

		if(m_MessageHandlerVec[nID] != null_ptr)
			return false;

		m_MessageHandlerVec[nID] = Handler;
		return true;
	}
}
