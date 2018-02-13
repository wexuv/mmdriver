
#include "ConnectionPool.h"

namespace driver
{
	ConnectionPool::ConnectionPool()
	{
	}

	ConnectionPool::~ConnectionPool()
	{
	}

	Connection* ConnectionPool::get_idle_connect()
	{
		size_t iVecSize = m_ClientSocketVec.size();
		for(size_t i = 0; i < iVecSize; ++ i)
		{
			if(m_ClientSocketVec[i]->idle())
			{
				return m_ClientSocketVec[i];
			}
		}

		if(m_ClientSocketVec.size() >= MAX_CLIENT_CONNECTION)
			return null_ptr;

		Connection* pkConnect = new Connection();
		if(pkConnect == null_ptr)
			return null_ptr;
		m_ClientSocketVec.push_back(pkConnect);

		return pkConnect;
	}
}
