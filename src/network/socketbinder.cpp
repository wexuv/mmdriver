#include "socketbinder.h"
#include "std_ex.h"

namespace driver 
{
	SocketBinder::SocketBinder ()
	{
	}

	SocketBinder::~SocketBinder (void)
	{
		m_BindContainer.clear();
	}

	void SocketBinder::bind(ClientSocket* pkClientSocket)
	{
		if(pkClientSocket !=  null_ptr)
		{
			m_BindContainer.push_back(pkClientSocket);
		}
	}

	void SocketBinder::unbind(ClientSocket* pkClientSocket)
	{
		for(BindContainer::iterator iter = m_BindContainer.begin(); iter != m_BindContainer.end(); ++ iter)
		{
			if(pkClientSocket == *iter)
			{
				m_BindContainer.erase(iter);
				return;
			}
		}
	}

	bool SocketBinder::recv()
	{
		//recv 
		fd_set kReadFDSet;
		fd_set kWriteFDSet;
		fd_set kExceptSet;

		FD_ZERO(&kReadFDSet);
		FD_ZERO(&kWriteFDSet);
		FD_ZERO(&kExceptSet);

		timeval stTv;
		stTv.tv_sec	 = 0;
		stTv.tv_usec = 1000;

		SOCKET iMaxFD = INVALID_SOCKET;

		bsvector<ClientSocket*> kActiveConnection; 

		for(BindContainer::iterator iter = m_BindContainer.begin(); iter != m_BindContainer.end(); ++ iter)
		{
			if ((*iter)->valid())
			{
				FD_SET((*iter)->get_fd(), &kReadFDSet);

				if ((*iter)->get_fd() > iMaxFD)
				{
					iMaxFD = (*iter)->get_fd();
				}

				kActiveConnection.push_back(*iter);
			}
		}

		if (iMaxFD > 0)
		{
			++iMaxFD; 

			tint32 iRet = select(static_cast<tint32>(iMaxFD), &kReadFDSet, &kWriteFDSet, &kExceptSet, &stTv);
			if(iRet <= 0)
			{
				return false;
			}

			for (size_t i=0; i<kActiveConnection.size(); ++i)
			{
				if (FD_ISSET(kActiveConnection[i]->get_fd(), &kReadFDSet))
				{
					// ½ÓÊÕÍøÂç°ü
					kActiveConnection[i]->recv_ex();
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SocketBinder::send()
	{
		for(BindContainer::iterator iter = m_BindContainer.begin(); iter != m_BindContainer.end(); ++ iter)
		{
			if ((*iter)->valid())
			{
				(*iter)->send_data_ex();
			}
		}
		return true;
	}
}
