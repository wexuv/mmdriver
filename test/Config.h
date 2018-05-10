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

	public:
		luaobject* GetLuaObject(const tstring& strName);

	public:
		luastate*		m_pkLuaState;
		LuaObjectMap	m_CacheObjs;

		tstring			m_strLogPath;
	};

	extern Config g_Config;
}

#endif //_CONFIG_H_