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
		m_szServerIP = "";
		m_nServerPort = 0;

		m_LogPath = "";
	}

	bool Config::Init()
	{
		luastate* pkLuaState = new luastate();

		pkLuaState->init(this);

		pkLuaState->DoFile("./Script/config.lua");

		luaobject* pluaObj = pkLuaState->GetLuaObject("Log");
		if(pluaObj != null_ptr)
		{
			AssertEx(pluaObj->IsTable(),"");

			luatable* pLuaTable = static_cast<luatable*>(pluaObj);
			luaobject* pLogPath = pLuaTable->GetLuaObject("LogFilePath");

			if(pLogPath != null_ptr)
			{
				AssertEx(pLogPath->IsString(),"");
				luastring* pluastring = static_cast<luastring*>(pLogPath);
				m_LogPath = pluastring->GetString();
			}

			SAFE_DELETE(pluaObj);
		}

		pluaObj = pkLuaState->GetLuaObject("LogFilePath");
		if(pluaObj != null_ptr)
		{
			AssertEx(pluaObj->IsString(),"");
			luastring* pluastring = static_cast<luastring*>(pluaObj);

			m_LogPath = pluastring->GetString();

			SAFE_DELETE(pluaObj);
		}

		pluaObj = pkLuaState->GetLuaObject("ServerSettings");
		if(pluaObj != null_ptr)
		{
			AssertEx(pluaObj->IsTable(),"");

			luatable* pLuaTable = static_cast<luatable*>(pluaObj);
			luaobject* pIp = pLuaTable->GetLuaObject("Ip");
			if(pIp != null_ptr)
			{
				AssertEx(pIp->IsString(),"");
				luastring* pluastring = static_cast<luastring*>(pIp);
				m_szServerIP = pluastring->GetString();
			}

			luaobject* pPort = pLuaTable->GetLuaObject("Port");
			if(pPort != null_ptr)
			{
				AssertEx(pPort->IsNumber(),"");
				luanumber* pluanumber = static_cast<luanumber*>(pPort);
				m_nServerPort = static_cast<tint32>(pluanumber->GetNumber());
			}

			SAFE_DELETE(pluaObj);
		}
		return true;
	}
}
