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

		LuaObjectMap::iterator iter = m_CacheObjs.begin();
		for( ; iter != m_CacheObjs.end(); ++ iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_CacheObjs.clear();

		m_strLogPath = "";
	}

	bool Config::Init()
	{
		m_pkLuaState = new luastate();

		m_pkLuaState->init(this);

		m_pkLuaState->DoFile("./Script/config.lua");

		luaobject* pluaObj = GetLuaObject("LogFilePath");
		if(pluaObj != null_ptr)
		{
			m_strLogPath = pluaObj->ToString();
		}

		return true;
	}

	luaobject* Config::GetLuaObject(const tstring& strName)
	{
		bsvector<tstring> luaVec = string_utility::SplitString(strName,'.');
		if(luaVec.empty())
			return null_ptr;

		tstring strRoot = *(luaVec.begin());
		luaVec.erase(luaVec.begin());

		LuaObjectMap::iterator iter = m_CacheObjs.find(strRoot);
		if(iter != m_CacheObjs.end())
		{
			if(iter->second->IsTable())
			{
				luatable* objTable = static_cast<luatable*>(iter->second);
				return objTable->GetLuaObject(luaVec);
			}
			return iter->second;
		}
		luaobject* pluaObj = m_pkLuaState->GetLuaObject(strRoot);
		if(pluaObj != null_ptr)
		{
			m_CacheObjs.insert(LuaObjectMap::value_type(strRoot,pluaObj));
			if(pluaObj->IsTable())
			{
				luatable* objTable = static_cast<luatable*>(pluaObj);
				return objTable->GetLuaObject(luaVec);
			}
			return pluaObj;
		}
		return null_ptr;
	}
}
