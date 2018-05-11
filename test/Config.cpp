#include "Config.h"
#include "luabinder.h"

namespace driver
{
	Config g_Config;

	Config::Config()
	{
		Clear();
	}

	Config::~Config()
	{
	}

	void Config::Clear()
	{
		m_pkLuaState = null_ptr;

		m_strLogPath = "";
	}

	bool Config::Init()
	{
		m_pkLuaState = new luastate();

		m_pkLuaState->init(this);

		m_pkLuaState->DoFile("./Script/config.lua");
		m_pkLuaState->DoFile("./Script/service.lua");

		m_strLogPath = m_pkLuaState->GetLuaObjectFromCache<tstring>("LogFilePath");

		return true;
	}

	luaobject* Config::GetLuaObject(const tstring& strName)
	{
		if(m_pkLuaState == null_ptr)
			return null_ptr;

		bstMutexScopedLock LockGuard(m_bstAssertMutex);

		return m_pkLuaState->GetLuaObjectFromCache(strName);
	}
}
