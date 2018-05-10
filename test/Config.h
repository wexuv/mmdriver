#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "driver.h"
#include "luaobject.h"

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

		luaobject* GetLuaObject(const tstring& strName);

	public:
		bstMutex		m_bstAssertMutex;

		luastate*		m_pkLuaState;
	
		tstring			m_strLogPath;
	};

	extern Config g_Config;
}

#endif //_CONFIG_H_