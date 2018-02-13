#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

#include "define.h"

#if defined(__WINDOWS__)

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#elif defined(__LINUX__)

#include <arpa/inet.h>
#include <sys/socket.h>

#else
#error windows or linux is required.
#endif

namespace driver 
{

#if defined(__WINDOWS__)

	class WindowsSocketAutoInit
	{
	public:
		WindowsSocketAutoInit(void)
		{
			WORD wVersion = MAKEWORD(2,2);
			WSADATA wsaData;
			tint32 err = WSAStartup(wVersion, &wsaData);
		}
		~WindowsSocketAutoInit(void)
		{
			WSACleanup();
		}
	};
	extern WindowsSocketAutoInit _global_wsai;

#elif defined(__LINUX__)

	typedef tint32	SOCKET;

#define		INVALID_SOCKET	(-1)
#define		SOCKET_ERROR	(-1)

#else
#error windows or linux is required.
#endif

	enum enmConnErrorCode
	{
		conn_error_code_success					= 0,
		conn_error_code_invalid_socket			= -1,
		conn_error_code_recv_not_enough_buffer	= -2,
		conn_error_code_peer_closed				= -3,
		conn_error_code_send_not_enough_buffer	= -4,
		conn_error_code_invalid_param			= -5,
		conn_error_code_unknown					= -6,
		conn_error_code_not_smooth_socket		= -7,
		conn_error_code_would_block				= -8,
	};

	namespace socketapi
	{
		SOCKET socket_ex(tint32 domain,tint32 type, tint32 protocol);

		bool listen_ex(SOCKET s,tint32 backlog);

		bool bind_ex(SOCKET s,const struct sockaddr* addr, tuint32 addrlen);

		SOCKET accept_ex(SOCKET s,struct sockaddr* addr, tuint32* addrlen);

		bool closesocket_ex(SOCKET s);

		bool set_non_block (SOCKET s, bool bNonBlock = true);

		bool setsockopt_ex(SOCKET s, tint32 level, tint32 optname, const void* optval, tuint32 optlen);

		bool getsockopt_ex(SOCKET s, tint32 level, tint32 optname, void* optval, tuint32* optlen);

		bool connect_ex (SOCKET s, struct sockaddr_in* server_addr, tuint32 addr_len);

		bool connect_nonblock (SOCKET s, struct sockaddr_in* server_addr, tuint32 addr_len, tint32 timeout);

		tint32 recv_ex(SOCKET s,void* buf,tuint32 len,tuint32 flags);

		tint32 send_ex(SOCKET s,const void* buf,tuint32 len,tuint32 flags);

		tint32 net_error_no();
	}
}

#endif //__SOCKET_API_H__
