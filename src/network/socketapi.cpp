#include "clientsocket.h"

namespace driver
{
	namespace socketapi
	{
		WindowsSocketAutoInit _global_wsai;

		SOCKET socket_ex(tint32 domain,tint32 type, tint32 protocol)
		{
			SOCKET s = ::socket(domain,type,protocol);

			if(s == INVALID_SOCKET)
			{

			}

			return s;
		}

		bool listen_ex(SOCKET s,tint32 backlog)
		{
			if(::listen(s,backlog) == SOCKET_ERROR)
				return false;
			return true;
		}

		bool bind_ex(SOCKET s,const struct sockaddr* addr, tuint32 addrlen)
		{
			if(::bind( s, addr, addrlen) == SOCKET_ERROR)
			{
				return false;
			}
			return true;
		}

		bool closesocket_ex(SOCKET s)
		{
#if defined(__WINDOWS__)

			if( closesocket(s) == SOCKET_ERROR)
			{
				return false;
			}
			return true;

#elif defined(__LINUX__)

			if(close(s) < 0)
			{
				return false;
			}
			return true;
#else
#error windows or linux is required.
#endif
		}

		SOCKET accept_ex(SOCKET s,struct sockaddr* addr, tuint32* addrlen)
		{
#if defined(__WINDOWS__)

			SOCKET client = ::accept(s,addr,(int*)addrlen);

#elif defined(__LINUX__)

			SOCKET client = ::accept(s, addr,addrlen);
#else
#error windows or linux is required.
#endif
			return client;
		}

		bool set_non_block (SOCKET s, bool bNonBlock)
		{
#if defined(__WINDOWS__)

			unsigned long argp = (bNonBlock == true) ? 1 : 0;

			if(ioctlsocket(s,FIONBIO, &argp) == SOCKET_ERROR)
			{
				return false;
			}
			return true;

#elif defined(__LINUX__)

			tint32 flags = fcntl(fd, F_GETFL, 0);
			if (flags < 0)
			{
				return false;
			}

			if(bNonBlock)
			{
				flags |= O_NONBLOCK;
				flags |= O_NDELAY;
			}
			else
			{
				flags &= ~O_NONBLOCK;
				flags &= ~O_NDELAY;
			}

			flags = fcntl(fd, F_SETFL, flags);

			if(flags < 0)
			{
				return false;
			}
			return true;

#else
#error windows or linux is required.
#endif
		}

		bool setsockopt_ex(SOCKET s, tint32 level, tint32 optname, const void* optval, tuint32 optlen)
		{
#if defined(__WINDOWS__)

			if(setsockopt(s, level,optname, (char*)optval, optlen) == SOCKET_ERROR)
				return false;
			return true;

#elif defined(__LINUX__)

			if(setsockopt(s, level, optname, optval,optlen) == SOCKET_ERROR)
			{
				return false;
			}
			return true;

#else
#error windows or linux is required.
#endif
		}

		bool getsockopt_ex(SOCKET s, tint32 level, tint32 optname, void* optval, tuint32* optlen)
		{
#if defined(__WINDOWS__)

			if(getsockopt(s, level,optname, (char*)optval, (int*)optlen) == SOCKET_ERROR)
				return false;
			return true;

#elif defined(__LINUX__)

			if(getsockopt(s, level, optname, optval,optlen) == SOCKET_ERROR)
			{
				return false;
			}
			return true;

#else
#error windows or linux is required.
#endif
		}

		bool connect_ex (SOCKET s, struct sockaddr_in* server_addr, tuint32 addr_len)
		{
			if( ::connect(s,(const sockaddr*)server_addr,addr_len) == SOCKET_ERROR)
				return false;
			return true;
		}

		bool connect_nonblock (SOCKET s, struct sockaddr_in* server_addr, tuint32 addr_len, tint32 timeout)
		{
			if (timeout < 0)
			{
				return false;
			}

			if(!set_non_block(s))
			{
				return false;
			}

			tint32 err_no = 0;

			if (::connect(s, (const sockaddr*)server_addr, addr_len) == SOCKET_ERROR)
			{
#if defined(__LINUX__)
				if (errno != EINPROGRESS)
				{
					return false;
				}
#endif
				fd_set rset;
				fd_set wset;

				FD_ZERO(&rset);
				FD_ZERO(&wset);

				FD_SET(s, &rset);
				FD_SET(s, &wset);

				struct timeval tval;
				tval.tv_sec = timeout / 1000;
				tval.tv_usec = (timeout % 1000) * 1000;

				tint32 ret = ::select(static_cast<tint32>(s+1), &rset, &wset, NULL, &tval);
				if (0 >= ret)
				{
					closesocket_ex(s);
					return false;
				}

				if (FD_ISSET(s, &rset) || FD_ISSET(s, &wset))
				{			
					tuint32 len = sizeof(err_no);
					if (!getsockopt_ex(s, SOL_SOCKET, SO_ERROR, &err_no, &len))
					{
						closesocket_ex(s);
						return false;
					}
				}
				else
				{
					closesocket_ex(s);
					return false;
				}

			}

			if (err_no)
			{
				closesocket_ex(s);
				return false;
			}

			return true;
		}

		tint32 recv_ex(SOCKET s,void* buf,tuint32 len,tuint32 flags)
		{
#if defined(__WINDOWS__)

			tint32 nRecv = ::recv(s,(tchar*)buf,len,flags);
			if(nRecv == SOCKET_ERROR)
			{
				tint32 iErrNo = net_error_no();
				if (WSAEWOULDBLOCK == iErrNo)
				{
					return conn_error_code_would_block;
				}
				return conn_error_code_unknown;
			}
			return nRecv;

#elif defined(__LINUX__)

			tint32 nRecv = ::recv(s,buf,len,flags);
			if(nRecv == SOCKET_ERROR)
			{
				if (EAGAIN == iErrNo || EWOULDBLOCK == iErrNo)
				{
					return conn_error_code_would_block;
				}
				return conn_error_code_unknown;
			}
			return nRecv;

#else
#error windows or linux is required.
#endif
		}

		tint32 send_ex(SOCKET s,const void* buf,tuint32 len,tuint32 flags)
		{
#if defined(__WINDOWS__)

			tint32 nSent = ::send(s,(const tchar*)buf,len,flags);

			if(nSent == SOCKET_ERROR)
			{
				tint32 iErrNo = net_error_no();
				if (WSAEWOULDBLOCK == iErrNo)
				{
					return conn_error_code_would_block;
				}
				return conn_error_code_unknown;
			}

			return nSent;

#elif defined(__LINUX__)

			tint32 nSent = ::send(s,buf,len,flags);

			if(nSent == SOCKET_ERROR)
			{
				if (EAGAIN == iErrNo || EWOULDBLOCK == iErrNo)
				{
					return conn_error_code_would_block;
				}
				return conn_error_code_unknown;
			}
			return nSent;

#else
#error windows or linux is required.
#endif
		}

		tint32 net_error_no()
		{
#if defined(__WINDOWS__)

			tint32 iErr = WSAGetLastError();
			switch(iErr)
			{
			case WSANOTINITIALISED:
			case WSAENETDOWN:
			case WSAEINPROGRESS:
			case WSAENOTSOCK:
			case WSAEFAULT:
			case WSAEINTR:
			case WSAEWOULDBLOCK:
			case WSAEAFNOSUPPORT:
			case WSAEMFILE:
			case WSAENOBUFS:
			case WSAEPROTONOSUPPORT:
			case WSAEPROTOTYPE:
			case WSAESOCKTNOSUPPORT:
			case WSAEINVAL:
			case WSAENOPROTOOPT:
			case WSAEADDRINUSE:
			case WSAEALREADY:
			case WSAEADDRNOTAVAIL:
			case WSAECONNREFUSED:
			case WSAEISCONN:
			case WSAENETUNREACH:
			case WSAETIMEDOUT:
			case WSAENOTCONN:
			case WSAENETRESET:
			case WSAEOPNOTSUPP:
			case WSAESHUTDOWN:
			case WSAEMSGSIZE:
			case WSAECONNABORTED:
			case WSAECONNRESET:
			case WSAEHOSTUNREACH:

			default:
				break;
			}
			return iErr;

#elif defined(__LINUX__)

			tint32 iErr = errno;
			switch(iErr)
			{
			case EINTR:
			case EINVAL:
			case EBADF:
			case EACCES:
			case EAGAIN:
			case EDEADLK:
			case EMFILE:
			case ENOLCK:
			case EPROTO:
			case EPROTONOSUPPORT:
			case ENFILE:
			case ENOBUFS:
			case ENOTSOCK:
			case ENOPROTOOPT:
			case EFAULT:
			case EALREADY:
			case EINPROGRESS:
			case ECONNREFUSED:
			case EISCONN:
			case ETIMEDOUT:
			case ENETUNREACH:
			case EADDRINUSE:
			case EWOULDBLOCK:
			case ECONNRESET:
			case EPIPE:
			case ENOTCONN:
			case EOPNOTSUPP:
			default:
				break;
			}
			return iErr;

#else
#error windows or linux is required.
#endif
		}
	}
}
