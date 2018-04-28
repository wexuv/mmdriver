#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "driver.h"

namespace driver
{
	class Config
	{
	public:
		Config();
		~Config();

	public:
		void Clear();
		bool Init();

	public:
		tstring		m_szServerIP;
		tint32		m_nServerPort;

		tstring		m_LogPath; 
	};

	extern Config g_Config;
}

#endif //_CONFIG_H_