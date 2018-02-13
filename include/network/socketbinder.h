#ifndef __SOCKET_BINDER_H__
#define __SOCKET_BINDER_H__

#include "lib.h"
#include "std_ex.h"
#include "clientsocket.h"

namespace driver 
{
	class SocketBinder
	{
		typedef std::list<ClientSocket*> BindContainer;

	public:
		SocketBinder ();
		~SocketBinder (void);

	public:
		//--------------------------------
		//���º�������ͬһ�̵߳���
		//--------------------------------
		void bind(ClientSocket* pkClientSocket);

		void unbind(ClientSocket* pkClientSocket);

		bool recv();

		bool send();
		//--------------------------------

	private:
		BindContainer	m_BindContainer;
	};
}

#endif //__SOCKET_BINDER_H__
