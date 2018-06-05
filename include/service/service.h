#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "types.h"
#include "lib.h"
#include "time_ex.h"
#include "std_ex.h"
#include "timer_ex.h"
#include "exception.h"
#include "message.h"

namespace driver
{
	enum ServiceState
	{
		SS_NONE,

		SS_START,
		SS_START_PROCESS,
		SS_START_FAIL,
		SS_START_OK,

		SS_RUN,

		SS_SHUTDOWN,
		SS_SHUTDOWN_PROCESS,
		SS_SHUTDOWN_OK,
	};

	enum RunningState
	{
		RS_READY,
		RS_SCHEDULE,
		RS_RUNNING,
	};

	class Service
	{
		friend class ServiceManager;

		typedef bsvector<TimerExPtr> TimerExPtrVec;

		typedef boost::function<bool(MessageConstPtr& rMessageConstPtr)> MessageCallback; 

		typedef bsvector<MessageCallback> MessageHandlerVec;

	public:
		Service();
		virtual ~Service();

	public:
		virtual tint32	GetServiceType() = 0;
		virtual tint32	GetServiceID(void);

		void SetRunningState(RunningState enmRuningState);
		RunningState GetRunningState();

		void SetServiceState(ServiceState enmServiceState);
		ServiceState GetServiceState();

		void AddTimer(TimerExPtr kTimerEx);

		bool RegisterMessage(MessageID nID,MessageCallback Handler);

	private:
		void Run(const TimeData& rkTimeData);

		inline void	   SetUpdateTime(tuint64 nTime) { m_nUpdateTime = nTime; }
		inline tuint64 GetUpdateTime() { return m_nUpdateTime; }

	protected:
		/*
		返回false时，该服务不会被安排调度。
		*/
		virtual bool NeedSchedule();
		virtual bool IsStartOK();
		virtual bool IsShutdownOK();

		virtual bool Init();

		virtual void Tick(const TimeData& rkTimeData);		 

		virtual bool Shutdown();

	protected:
		TimerExPtrVec		m_TimerExPtrVec;
		MessageHandlerVec	m_MessageHandlerVec;

		RunningState	m_enmRunState;
		ServiceState	m_enmServiceState;

		tint32			m_nServiceID;

		tuint64			m_nUpdateTime;
	};

	typedef std::shared_ptr<Service>	ServicePtr;
}

#endif //_SERVICE_H_