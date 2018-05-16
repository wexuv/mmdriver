#include "serversocket.h"
#include "std_ex.h"

namespace driver 
{
	ServerSocket::ServerSocket (void)
	{
		m_socket_fd	= INVALID_SOCKET;

		m_funOnConnect.clear();
	}

	ServerSocket::~ServerSocket (void)
	{
		this->close();
	}

	bool ServerSocket::open(u_short port, const char* ip)
	{
		m_socket_fd = socketapi::socket_ex(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == m_socket_fd)
		{
			return false;
		}

		tuint32 flag = 1;
		if (!socketapi::setsockopt_ex(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, (tuint32)sizeof(flag)))
		{
			this->close();
			return false;
		}

		struct sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		if (NULL != ip)
		{
			server_addr.sin_addr.s_addr = inet_addr(ip);
		}
		else
		{
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
		}

		server_addr.sin_port = (u_short)htons(port);
		tuint32 addr_size = tuint32(sizeof(server_addr));
		if (!socketapi::bind_ex(m_socket_fd, (const sockaddr*) &server_addr, addr_size))
		{
			this->close();
			return false;
		}

		tint32 option_value = 0xFFFF;
		tuint32 option_length = 0;
		option_length = (tuint32)sizeof(option_value);

		if (!socketapi::setsockopt_ex(m_socket_fd, SOL_SOCKET, SO_SNDBUF, (const void *)&option_value, option_length))
		{
			this->close();
			return false;
		}

		if (!socketapi::setsockopt_ex(m_socket_fd, SOL_SOCKET, SO_RCVBUF, (const void *)&option_value, option_length))
		{
			this->close();
			return false;
		}

		if (!socketapi::listen_ex(m_socket_fd, 15))
		{
			this->close();
			return false;
		}

		if (!socketapi::set_non_block(m_socket_fd))
		{
			this->close();
			return false;
		}
		return true;
	}

	void ServerSocket::close (void)
	{
		if (m_socket_fd != INVALID_SOCKET)
		{
			socketapi::closesocket_ex(m_socket_fd);
			m_socket_fd = INVALID_SOCKET;
		}

		m_funOnConnect.clear();
	}

	bool ServerSocket::valid (void) const
	{
		return (m_socket_fd != INVALID_SOCKET);
	}

	bool ServerSocket::accept()
	{
		if(!m_funOnConnect)
		{
			return false;
		}

		fd_set kReadFDSet;
		FD_ZERO(&kReadFDSet);

		timeval kTv;
		kTv.tv_sec	= 0;
		kTv.tv_usec = 1000;

		FD_SET(m_socket_fd, &kReadFDSet);

		tint32 iRet = ::select(static_cast<tint32>(m_socket_fd + 1), &kReadFDSet, NULL, NULL, &kTv);
		if(SOCKET_ERROR == iRet)
		{
			return false;
		}

		if (FD_ISSET(m_socket_fd, &kReadFDSet))
		{
			struct sockaddr_in stPeerAddr;
			tuint32 AddrLength = sizeof(stPeerAddr);
			SOCKET iFD = socketapi::accept_ex(m_socket_fd, (struct sockaddr*)&stPeerAddr, &AddrLength);
			if (INVALID_SOCKET == iFD)
			{
				return false;
			}

			if (!socketapi::set_non_block(iFD))
			{
				socketapi::closesocket_ex(iFD);
				return false;
			}

			tuint32	uIP = ntohl(stPeerAddr.sin_addr.s_addr);
			tuint16	usPort = ntohs(stPeerAddr.sin_port);

			ClientSocket* pkClientSocket = m_funOnConnect(uIP,usPort);
			if(null_ptr == pkClientSocket)
			{
				socketapi::closesocket_ex(iFD);
				return false;
			}

			pkClientSocket->set_fd(iFD);
			pkClientSocket->set_ip_info(uIP,usPort);
			m_ClientContainer.push_back(pkClientSocket);

			return true;
		}

		return false;
	}

	tint32 ServerSocket::accept_ex(tint32 iMaxAccept)
	{
		tint32 iAcceptCount = 0;

		for(tint32 i = 0; i < iMaxAccept; ++ i)
		{
			if(!accept())
				break;
			++iAcceptCount;
		}
		return iAcceptCount;
	}

	bool ServerSocket::recv()
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

		for(ClientContainer::iterator iter = m_ClientContainer.begin(); iter != m_ClientContainer.end();)
		{
			if(*iter == null_ptr || !(*iter)->valid())
			{
				iter = m_ClientContainer.erase(iter);
			}
			else
			{
				FD_SET((*iter)->get_fd(), &kReadFDSet);

				if ((*iter)->get_fd() > iMaxFD)
				{
					iMaxFD = (*iter)->get_fd();
				}

				 ++ iter;
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

			for(ClientContainer::iterator iter = m_ClientContainer.begin(); iter != m_ClientContainer.end(); ++ iter)
			{
				if (FD_ISSET((*iter)->get_fd(), &kReadFDSet))
				{
					// ½ÓÊÕÍøÂç°ü
					(*iter)->recv_ex();
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}
