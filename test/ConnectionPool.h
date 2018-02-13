#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include "clientsocket.h"
#include "std_ex.h"

namespace driver
{

#define MAX_CLIENT_CONNECTION	5000

	class Connection : public ClientSocket
	{
	public:
		//Á¬½Ó×´Ì¬
		enum ConnectionStatus
		{
			UNUSED = 0,
			CONNECTED = 1,
			VERIFYING = 2,
			LOADING = 3,
			LOGIN = 4,
		};

	public:
		Connection()
		{
			clear();
		}
		~Connection()
		{
		}

		void clear()
		{
			m_nUserID = -1;

			m_nStatus = UNUSED;

			close();
		}

		inline bool idle() const
		{
			return (m_nStatus == UNUSED);
		}

		void SetStatus(ConnectionStatus enmStatus) 
		{ 
			m_nStatus = enmStatus; 
		}
		ConnectionStatus GetStatus()
		{
			return m_nStatus;
		}

	public:
		tuint32				m_nUserID;
		std::string			m_sAccount;
	
		ConnectionStatus	m_nStatus;
	};

	class ConnectionPool
	{
	public:
		ConnectionPool();
		virtual ~ConnectionPool();

	public:
		Connection* get_idle_connect();

	private:
		bsvector<Connection*> m_ClientSocketVec;
	};
}

#endif //_CONNECTION_POOL_H_