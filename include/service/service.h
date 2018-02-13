#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "types.h"
#include "lib.h"
#include "time_ex.h"
#include "std_ex.h"
#include "timer_ex.h"
#include "exception.h"
#include "message.h"
#include "msg_queue.h"

namespace driver
{

#define MSG_QUEUE_SIZE_DEFAULT		0x10000000		// 16M

	enum ServiceState
	{
		SS_NONE,

		SS_START,
		SS_START_OK,

		SS_RUN,

		SS_SHUTDOWN,
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

	protected:
		/*
		����falseʱ���÷��񲻻ᱻ���ŵ��ȡ�
		*/
		virtual bool NeedSchedule();

		virtual bool Init();

		virtual void Tick(const TimeData& rkTimeData);		 

		virtual bool Shutdown();


	protected:
		TimerExPtrVec		m_TimerExPtrVec;
		MessageHandlerVec	m_MessageHandlerVec;

		RunningState	m_enmRunState;
		ServiceState	m_enmServiceState;

		tint32			m_nServiceID;


	/*ͨ�ź͹����ڴ�
	ͨ�������ڴ�ͻ��ζ��н�����Ϣ����
	���������service֮��ͨ�ţ�����ֱ��ͨ�������ڴ���ʻ��ζ���
	1�Զ������£���Ҫͨ��service manager������Ϣת��
	*/
	protected:
		bool InitMessageQueue(tuint32 unKey, tint32 nInputSize = MSG_QUEUE_SIZE_DEFAULT,tint32 nOutputSize = MSG_QUEUE_SIZE_DEFAULT);
		bool PushMessage(const tchar* pMsgBuff, tuint16 usMsgSize);
		bool PopMessage(tchar* pMsgBuff, tuint16& usMsgSize);
		bool _sendMessage(const tchar* pMsgBuff, tuint16 usMsgSize);
		bool _recvMessasge (tchar* pMsgBuff, tuint16& usMsgSize);

	private:
		Share_Memory	m_kShareMemory;
		Msg_Queue		m_kMsgOutput;
		Msg_Queue		m_kMsgInput;
	};

	typedef std::shared_ptr<Service>	ServicePtr;
}

#endif //_SERVICE_H_