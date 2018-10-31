#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_

#include "common/types.h"
#include "common/define.h"
#include "common/lib.h"
#include "boost/threadpool.hpp"
#include "service.h"
#include "common/std_ex.h"
#include "common/log.h"

namespace driver
{
	class ServiceManager
	{
	public:
		ServiceManager();
		~ServiceManager();

	public:
		friend void RunServiceManager(void);

	protected:
		bool Create(tint32 nThreadCount);

		/*
		���̰߳�ȫ����Run֮ǰSevice����ȫ��Regist��������;��ɾ��������ͨ������Service��״̬ʹ֮������ͣ
		*/
		void Register(Service* pService);

		void Run();

		void Tick(const TimeData& rkTimeData);

	public:
		void ShutDown(tint32 nReason);

	protected:
		void RunLogic(const ServiceState enmRunStatus);
		bool RunLogicBreakCondition(const ServiceState enmRunStatus);

		void Wait(tint32 nSecondTimeout);

		void LogCpuMemStat(const tchar* szDesc);

	protected:
		bool IsAllServiceStatusEqual(ServiceState enmStatus);
		void SetAllServiceStatus(ServiceState enmStatus);

		void IsShouldShutDown(void);

	protected:

		typedef boost::threadpool::fifo_pool ThreadPool;
		typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;

		ThreadPoolPtr	m_pkThreadPool;

		typedef bsvector<ServicePtr> ServicePtrVec;

		ServicePtrVec	m_ServicePtrVec;

		bool	m_bShutdown;

		Log_Engine	m_stLogEngine;
	};

	extern ServiceManager gServiceManager;
}

#endif //_SERVICEMANAGER_H_