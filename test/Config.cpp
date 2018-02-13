
#include "Config.h"

namespace driver
{
	Config g_Config;

	Config::Config()
	{
		Init();
	}

	Config::~Config()
	{
	}

	bool Config::Init()
	{
		
		m_LogPath = "./Log/";
		return true;
	}
}
