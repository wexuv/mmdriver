#ifndef __SERVER_SOCKET_H__
#define __SERVER_SOCKET_H__

#include "lib.h"
#include "socketbinder.h"

namespace driver 
{
	class ServerSocket
	{
		typedef boost::function<ClientSocket* (tuint32 uClientIP,tuint16 uClientPort)> OnConnect; 

	public:
		ServerSocket (void);
		~ServerSocket (void);

		bool open(u_short port, const char* ip = NULL);

		void close (void);

		bool valid (void) const;

		bool accept();

		tint32 accept_ex(tint32 iMaxAccept);

	public:
		/*�������ӻص�����
		�������ͻ��˵�IP,�˿�
		����ֵ���������ӵ�ClientSocket
		*/
		template<class T>
		void SetOnConnectFun(ClientSocket* (T::*func_)(tuint32 uClientIP,tuint16 uClientPort),T* pIns)
		{
			m_funOnConnect = boost::bind(func_,pIns,_1,_2);
		};

	protected:
		SOCKET			m_socket_fd;

		OnConnect		m_funOnConnect;
	};
}

#endif //__SERVER_SOCKET_H__
