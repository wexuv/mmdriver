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
		bool Init();

	public:
		std::string		m_LogPath; 
	};

	extern Config g_Config;
}

#endif //_CONFIG_H_