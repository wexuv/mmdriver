#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include "types.h"
#include "lib.h"
#include "socketapi.h"

namespace driver 
{
	enum 
	{
		max_package_size = 0xFFFF,			// 消息包最大长度为65535
		recv_buffer_size = 0x100000,		// 接收缓冲区大小，该值必须大于65535（得容得下一个完整的应用层消息包）
		send_buffer_size = 0x100000,		// 发送缓冲区大小，该值必须大于65535（得容得下一个完整的应用层消息包）
	};

	enum
	{
		max_c2s_pkg_size	= 0x1400,		// 5K		
		max_s2c_pkg_size	= 0x2C00,		// 11K
		max_cs_channel_size	= 0x1000000		// 16M
	};

	enum enmTCPStatus
	{
		tcp_status_closed	= 0,
		tcp_status_opened	= 1,
		tcp_status_connected= 2,
	};

	class ClientSocket
	{
		typedef boost::function<void(ClientSocket* pkConnectSocket)> OnDisConnect;
		typedef boost::function<tint32(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen)> OnRecvMessage; 

		friend class ServerSocket;
		friend class SocketBinder;

	public:
		ClientSocket (void);
		~ClientSocket (void);

		void close (void);

		SOCKET get_fd (void) const;
		
		// 该连接是否已经完整，如果是监听端口，则打开了就OK；如果是普通的连接，则必须是三次握手成功
		bool valid (void) const;

		tuint32 get_ip();

		bool set_buf_size(size_t nRecvBufSize = max_c2s_pkg_size,size_t nSendBufSize = max_s2c_pkg_size);

		//非线程安全，消息缓存，不会立即发送
		tint32 send_data (const char* data, tuint16 size);

		//非线程安全，消息会立即发送
		tint32 send_ex (const char* data, tuint16 size);

		/*******************************
		//主动客户端使用
		*******************************/
		bool open(char* local_ip = null_ptr);

		// timeout的单位为毫秒
		bool connect (const char* strIP, tuint16 port, bool nonblock = true, tint32 timeout = 2);

		bool connect (tuint32 ip, tuint16 port, bool nonblock = true, tint32 timeout = 2);
		/********************************/

	public:
		//设置断开连接回调函数
		template<class T>
		void SetOnDisConnectFun(void (T::*func_)(ClientSocket* pkConnectSocket),T* pIns)
		{
			m_funOnDisConnect = boost::bind(func_,pIns,_1);
		};
		/*设置接受消息回调函数
		返回值：<0 错误,断开链接  >=0 已经读取的长度, 消息处理函数在socketbinder::recv_and_send线程中触发
		*/
		template<class T>
		void SetOnRecvMessageFun(tint32 (T::*func_)(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen),T* pIns)
		{
			m_funOnRecvMessage = boost::bind(func_,pIns,_1,_2,_3);
		};

	protected:

		bool set_fd (SOCKET s);

		enmTCPStatus get_status (void) const;

		//接收网络包，并调用消息处理函数
		bool recv_ex();

		//接受网络包，并不调用消息处理函数
		tint32 recv_data (void);

		//非线程安全，发送缓存消息
		tint32 send_data_ex();

		void set_ip_info (tuint32 ip, tuint16 port);

		void disconnect();

	protected:
		SOCKET m_socket_fd;
		enmTCPStatus m_socket_status;

		tuint32	m_uIP;
		tuint16 m_usPort;

		//receiving buffer
		size_t	m_nRecvBufSize;
		char*	m_recv_buffer;
		size_t	m_recv_buffer_len;

		//sending buffer
		size_t	m_nSendBufSize;
		char*	m_send_buffer;
		size_t	m_send_head;
		size_t	m_send_tail;

		OnDisConnect	m_funOnDisConnect;

		OnRecvMessage	m_funOnRecvMessage;
	};
}

#endif //__CLIENT_SOCKET_H__
