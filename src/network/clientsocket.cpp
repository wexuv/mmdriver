#include "clientsocket.h"

namespace driver
{

	ClientSocket::ClientSocket (void)
	{
		m_socket_fd = INVALID_SOCKET;
		m_socket_status = tcp_status_closed;

		m_uIP = 0;
		m_usPort = 0;

		m_nRecvBufSize = 0;
		m_recv_buffer = null_ptr;
		m_recv_buffer_len = 0;

		m_nSendBufSize = 0;
		m_send_buffer = null_ptr;
		m_send_head = 0;
		m_send_tail = 0;

		m_funOnDisConnect.clear();
		m_funOnRecvMessage.clear();
	}

	ClientSocket::~ClientSocket (void)
	{
		this->close();
	}

	void ClientSocket::close (void)
	{
		if (m_socket_fd != INVALID_SOCKET)
		{
			socketapi::closesocket_ex(m_socket_fd);
			m_socket_fd = INVALID_SOCKET;
		}

		m_socket_status = tcp_status_closed;

		m_uIP = 0;
		m_usPort = 0;

		m_nRecvBufSize = 0;
		SAFE_DELETE(m_recv_buffer);
		m_recv_buffer_len = 0;

		m_nSendBufSize = 0;
		SAFE_DELETE(m_send_buffer);
		m_send_head = 0;
		m_send_tail = 0;

		m_funOnDisConnect.clear();
		m_funOnRecvMessage.clear();
	}

	bool ClientSocket::set_fd (SOCKET s)
	{
		if (s == INVALID_SOCKET)
		{
			return false;
		}

		m_socket_fd		= s;
		m_socket_status = tcp_status_connected;

		return socketapi::set_non_block(m_socket_fd);
	}

	SOCKET ClientSocket::get_fd (void) const
	{
		return m_socket_fd;
	}

	enmTCPStatus ClientSocket::get_status (void) const
	{
		return m_socket_status;
	}

	bool ClientSocket::open(char* local_ip)
	{
		this->close();

		m_socket_fd = socketapi::socket_ex(AF_INET, SOCK_STREAM, 0);
		if (m_socket_fd == INVALID_SOCKET)
		{
			return false;
		}

		if (NULL != local_ip)
		{
			struct sockaddr_in local_address;
			memset(&local_address, 0, sizeof(local_address));

			local_address.sin_family = AF_INET;
			local_address.sin_addr.s_addr = inet_addr(local_ip);
			tuint32 addr_size = sizeof(local_address);
			if (!socketapi::bind_ex(m_socket_fd, (const struct sockaddr*) &local_address, addr_size))
			{
				this->close();
				return false;
			}

		}

		m_socket_status = tcp_status_opened;

		return true;
	}

	bool ClientSocket::connect (const char* strIP, tuint16 port, bool nonblock, tint32 timeout)
	{
		if (NULL == strIP)
		{
			return false;
		}

		tuint32 iIP = ntohl(inet_addr(strIP));

		return connect(iIP,port,nonblock,timeout);
	}

	bool ClientSocket::connect (tuint32 ip, tuint16 port, bool nonblock, tint32 timeout)
	{
		if (tcp_status_opened != m_socket_status || m_socket_fd == INVALID_SOCKET)
		{
			return false;
		}

		struct sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(ip);
		server_addr.sin_port = htons(port);
		tuint32 addr_size = sizeof(server_addr);

		if (!socketapi::connect_nonblock(m_socket_fd, &server_addr, addr_size, timeout))
		{
			this->close();
			return false;
		}

		if (!socketapi::set_non_block(m_socket_fd,nonblock))
		{
			this->close();
			return false;
		}

		m_socket_status = tcp_status_connected;
		return true;
	}

	bool ClientSocket::valid (void) const
	{
		if (m_socket_fd == INVALID_SOCKET)
		{
			return false;
		}
		else
		{
			if (tcp_status_connected == m_socket_status)
			{
				return true;
			}
			return false;
		}
	}


	bool ClientSocket::recv()
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

		SOCKET iMaxFD = get_fd();

		FD_SET(get_fd(), &kReadFDSet);

		if (iMaxFD > 0)
		{
			tint32 iRet = select(static_cast<tint32>(iMaxFD+1), &kReadFDSet, &kWriteFDSet, &kExceptSet, &stTv);
			if(iRet <= 0)
			{
				return false;
			}

			if (FD_ISSET(get_fd(), &kReadFDSet))
			{
				// 接收网络包
				recv_ex();
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ClientSocket::recv_ex()
	{
		if(!m_funOnRecvMessage)
			return false;

		// 接收网络包
		tint32 iRet = recv_data();

		if (conn_error_code_success == iRet || conn_error_code_would_block == iRet || conn_error_code_recv_not_enough_buffer == iRet)
		{
			tint32 nReadLen = m_funOnRecvMessage(this,m_recv_buffer,m_recv_buffer_len);

			if(nReadLen < 0 || (nReadLen == 0 && conn_error_code_recv_not_enough_buffer == iRet))
			{
				this->disconnect();
				return false;
			}
			else
			{
				if(nReadLen == 0)
					return true;

				if(nReadLen > (tint32)m_recv_buffer_len)
					return false;

				m_recv_buffer_len -= nReadLen;

				memmove(&m_recv_buffer[0], &m_recv_buffer[nReadLen], m_recv_buffer_len);

				return true;
			}
		}

		this->disconnect();
		return false;
	}

	tint32 ClientSocket::recv_data (void)
	{
		tint32 ret = conn_error_code_success;

		if (tcp_status_connected != m_socket_status || m_socket_fd == INVALID_SOCKET)
		{
			return conn_error_code_invalid_socket;
		}

		tint32 received_byte = 0;

		do // 循环从网络连接上接收数据
		{
			char* pszRecvBuffer = m_recv_buffer + m_recv_buffer_len;
			tuint32 usBuffSize = tuint32(max_c2s_pkg_size - m_recv_buffer_len);

			if(usBuffSize > 0)
			{
				received_byte = socketapi::recv_ex(m_socket_fd, pszRecvBuffer, usBuffSize, 0);
				if (received_byte > 0)
				{
					m_recv_buffer_len += received_byte;
				}
				else if (0 == received_byte)
				{	
					this->disconnect();
					ret = conn_error_code_peer_closed;
					break;
				}
				else if(conn_error_code_would_block == received_byte)
				{
					// 暂时取不到数据
					ret = conn_error_code_would_block;
					break;
				}
				else
				{
					this->disconnect();
					ret = conn_error_code_unknown;
					break;
				}
			}
			else
			{
				ret = conn_error_code_recv_not_enough_buffer;
				break;
			}

		} while (received_byte > 0);

		return ret;
	}

	//非线程安全，消息不会立即发送
	tint32 ClientSocket::send_cache (const char* data, tuint16 size)
	{
		tint32 ret = conn_error_code_success;

		if (NULL == data || 0 == size)
		{
			return conn_error_code_invalid_param;
		}
		if (tcp_status_connected != m_socket_status || m_socket_fd ==INVALID_SOCKET)
		{
			return conn_error_code_invalid_socket;
		}

		size_t data_remained_bytes = m_send_tail - m_send_head;

		if(m_nSendBufSize - m_send_tail >= size)
		{
			memcpy(&m_send_buffer[m_send_tail], data, size);
			m_send_tail += size;
		}
		else if(m_nSendBufSize - m_send_tail + m_send_head >= size)
		{
			memmove(&m_send_buffer[0], &m_send_buffer[m_send_head], (size_t)data_remained_bytes);
			m_send_head = 0;
			m_send_tail = data_remained_bytes;
			memcpy(&m_send_buffer[m_send_tail], data, size);
			m_send_tail += size;
		}
		else
		{
			ret = conn_error_code_send_not_enough_buffer;
		}

		return ret;
	}

	//发送缓存消息
	tint32 ClientSocket::send_cache_data()
	{
		if (tcp_status_connected != m_socket_status || m_socket_fd ==INVALID_SOCKET)
		{
			return conn_error_code_invalid_socket;
		}

		size_t data_remained_bytes = 0;
		data_remained_bytes = m_send_tail - m_send_head;

		// 发送上次的剩余数据
		const char* data_ptr = &m_send_buffer[m_send_head];
		tint32 sended_bytes = 0;
		while (data_remained_bytes > 0)
		{
			sended_bytes = socketapi::send_ex(m_socket_fd, data_ptr, (tuint32)data_remained_bytes, 0);
			if (sended_bytes > 0)
			{
				m_send_head += sended_bytes;
				data_ptr += sended_bytes;
				data_remained_bytes -= sended_bytes;
			}
			else if (sended_bytes < 0 && conn_error_code_would_block != sended_bytes)
			{
				this->disconnect();
				return conn_error_code_unknown;
			}
			else
			{
				break;
			}
		}

		if (0 == data_remained_bytes)
		{
			m_send_head = 0;
			m_send_tail = 0;
			return conn_error_code_success;
		}
		else
		{
			return conn_error_code_not_smooth_socket;
		}
	}

	tint32 ClientSocket::send_data (const char* data, tuint16 size)
	{
		tint32 ret = conn_error_code_success;
		if (NULL == data || 0 == size)
		{
			return conn_error_code_invalid_param;
		}

		if (tcp_status_connected != m_socket_status || m_socket_fd ==INVALID_SOCKET)
		{
			return conn_error_code_invalid_socket;
		}

		size_t data_remained_bytes = 0;
		data_remained_bytes = m_send_tail - m_send_head;

		// 发送上次的剩余数据
		const char* data_ptr = &m_send_buffer[m_send_head];

		tint32 sended_bytes = 0;
		while (data_remained_bytes > 0)
		{
			sended_bytes = socketapi::send_ex(m_socket_fd, data_ptr, (tuint32)data_remained_bytes, 0);
			if (sended_bytes > 0)
			{
				m_send_head += sended_bytes;
				data_ptr += sended_bytes;
				data_remained_bytes -= sended_bytes;
			}
			else if (sended_bytes < 0 && conn_error_code_would_block != sended_bytes)
			{
				this->disconnect();
				ret = conn_error_code_unknown;
				break;
			}
			else
			{
				if(m_nSendBufSize - m_send_tail >= size)
				{
					memcpy(&m_send_buffer[m_send_tail], data, size);
					m_send_tail += size;
				}
				else if(m_nSendBufSize - m_send_tail + m_send_head >= size)
				{
					memmove(&m_send_buffer[0], &m_send_buffer[m_send_head], (size_t)data_remained_bytes);
					m_send_head = 0;
					m_send_tail = data_remained_bytes;
					memcpy(&m_send_buffer[m_send_tail], data, size);
					m_send_tail += size;
				}
				else
				{
					ret = conn_error_code_send_not_enough_buffer;
				}

				break;
			}
		}

		if (0 == data_remained_bytes)
		{
			m_send_head = 0;
			m_send_tail = 0;
		}
		else
		{	
			return ret;
		}

		// 发送本次数据
		data_remained_bytes = size;
		data_ptr = data;

		while (data_remained_bytes > 0)
		{
			sended_bytes = socketapi::send_ex(m_socket_fd, data_ptr, (tuint32)data_remained_bytes, 0);
			if (sended_bytes > 0)
			{
				data_ptr += sended_bytes;
				data_remained_bytes -= sended_bytes;
			}
			else if (sended_bytes < 0 && conn_error_code_would_block != sended_bytes)
			{
				this->disconnect();
				ret = conn_error_code_unknown;
				break;
			}
			else
			{
				memcpy(&m_send_buffer[m_send_tail], data_ptr, data_remained_bytes);
				m_send_tail += data_remained_bytes;
				break;
			}
		}

		return ret;
	}

	tuint32 ClientSocket::get_ip()
	{
		return m_uIP;
	}

	bool ClientSocket::set_buf_size(size_t nRecvBufSize,size_t nSendBufSize)
	{
		SAFE_DELETE(m_recv_buffer);
		SAFE_DELETE(m_send_buffer);

		m_recv_buffer = new char[nRecvBufSize];
		if(m_recv_buffer == null_ptr)
		{
			return false;
		}

		m_send_buffer = new char[nSendBufSize];
		if(m_send_buffer == null_ptr)
		{
			//释放掉上面分配的接收缓存
			SAFE_DELETE(m_recv_buffer);
			return false;
		}

		m_nRecvBufSize = nRecvBufSize;
		m_nSendBufSize = nSendBufSize;

		return true;
	}

	void ClientSocket::set_ip_info (tuint32 ip, tuint16 port)
	{
		m_uIP = ip;
		m_usPort = port;
	}

	void ClientSocket::disconnect()
	{
		if(m_funOnDisConnect)
		{
			m_funOnDisConnect(this);
		}
		this->close();
	}
}
