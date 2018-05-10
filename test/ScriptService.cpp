
#include "ScriptService.h"
#include "ScriptInterface.inl"
#include "Config.h"

namespace driver
{
	ScriptService::ScriptService()
	{
		m_pLuaState = null_ptr;
	}

	ScriptService::~ScriptService()
	{
		SAFE_DELETE(m_pLuaState);
	}

	bool ScriptService::Init()
	{
		if(!Service::Init())
			return false;

		tchar szLogFile[MAX_FILE_NAME_LENGTH];
		tsnprintf(szLogFile,sizeof(szLogFile),"%s/ScriptService_%d",g_Config.m_strLogPath.c_str(),GetServiceID());

		m_stLogEngine.init(0xFF, szLogFile);

		m_pLuaState = new luastate();

		m_pLuaState->init(this);

		m_pLuaState->RegisterInitFunction(LUA_EXPORT_OBJECT_FUNC(ScriptService));

		return true;
	}

	void ScriptService::Tick(const TimeData& rkTimeData)
	{
		Service::Tick(rkTimeData);
	}
}
