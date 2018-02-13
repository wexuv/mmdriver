#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include "types.h"
#include "lib.h"
#include "socketapi.h"

namespace driver 
{
	enum 
	{
		max_package_size = 0xFFFF,			// ��Ϣ����󳤶�Ϊ65535
		recv_buffer_size = 0x100000,		// ���ջ�������С����ֵ�������65535�����ݵ���һ��������Ӧ�ò���Ϣ����
		send_buffer_size = 0x100000,		// ���ͻ�������С����ֵ�������65535�����ݵ���һ��������Ӧ�ò���Ϣ����
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

	private:
		ClientSocket& operator=( const ClientSocket &rhs ) {};
		ClientSocket( const ClientSocket &rhs ) {};
	public:
		ClientSocket (void);
		~ClientSocket (void);

		void close (void);

		SOCKET get_fd (void) const;
		
		// �������Ƿ��Ѿ�����������Ǽ����˿ڣ�����˾�OK���������ͨ�����ӣ���������������ֳɹ�
		bool valid (void) const;

		tuint32 get_ip();

		bool set_buf_size(size_t nRecvBufSize = max_c2s_pkg_size,size_t nSendBufSize = max_s2c_pkg_size);

		//���̰߳�ȫ����Ϣ���棬������������
		tint32 send_data (const char* data, tuint16 size);

		//���̰߳�ȫ����Ϣ����������
		tint32 send_ex (const char* data, tuint16 size);

		/*******************************
		//�����ͻ���ʹ��
		*******************************/
		bool open(char* local_ip = null_ptr);

		// timeout�ĵ�λΪ����
		bool connect (const char* strIP, tuint16 port, bool nonblock = true, tint32 timeout = 2);

		bool connect (tuint32 ip, tuint16 port, bool nonblock = true, tint32 timeout = 2);
		/********************************/

	public:
		//���öϿ����ӻص�����
		template<class T>
		void SetOnDisConnectFun(void (T::*func_)(ClientSocket* pkConnectSocket),T* pIns)
		{
			m_funOnDisConnect = boost::bind(func_,pIns,_1);
		};
		/*���ý�����Ϣ�ص�����
		����ֵ��<0 ����,�Ͽ�����  >=0 �Ѿ���ȡ�ĳ���, ��Ϣ��������socketbinder::recv_and_send�߳��д���
		*/
		template<class T>
		void SetOnRecvMessageFun(tint32 (T::*func_)(ClientSocket* pkClientSocket,const char* pRecvBuf, size_t nRecvBufLen),T* pIns)
		{
			m_funOnRecvMessage = boost::bind(func_,pIns,_1,_2,_3);
		};

	protected:

		bool set_fd (SOCKET s);

		enmTCPStatus get_status (void) const;

		//�������������������Ϣ������
		bool recv_ex();

		//���������������������Ϣ������
		tint32 recv_data (void);

		//���̰߳�ȫ�����ͻ�����Ϣ
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
