#ifndef _SCRIPT_INTERFACE_INL_
#define _SCRIPT_INTERFACE_INL_

#include "ScriptService.h"
#include "luabinder.h"
#include "User.h"

namespace driver
{
	static int bnd_LuaCreate_ObjUser( lua_State* L )
	{
		luabinder binder(L);
		ObjUser* h = NULL;

		AssertEx(false,"不应该在Lua中创建对象！");

		if( h == NULL )
		{
			//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING,"全局类%s的对象还没有创建就试图在Lua脚本中调用，请检查初始化时序！！" , "ObjUser" );
			return 0;
		}
		binder.PushUserType(h,/*h->GetRTTIName()*/"ObjUser" );
		return 1;
	}
	static int bnd_LuaCreate_GetObjUserID( lua_State* L )
	{
		luabinder binder(L);
		ObjUser* pkUser = (ObjUser*)binder.CheckUserType(1,"ObjUser");

		tuint32 ret = pkUser->GetID();
		LuaPushValue(L, ret);

		//Utility::LuaCall( *pkUser ,&ObjUser::GetID , L , 2 );
		return 1;
	}
	static int bnd_LuaDestroy_ObjUser(lua_State* L)
	{
		luabinder binder(L);
		ObjUser* h = (driver::ObjUser*) binder.CheckUserType(1,"ObjUser");
		AssertEx(h!=null_ptr,"ObjUserPtr = null_ptr");
		return 0;
	}
	static const luaL_reg lib_ObjUser[] = {
		{"Create", bnd_LuaCreate_ObjUser},
		{"GetID", bnd_LuaCreate_GetObjUserID},
		{NULL, NULL}
	};
	static int luaopen_ObjUser (lua_State* L)
	{
		luabinder binder(L);
		binder.Init("ObjUser", 0  , lib_ObjUser , bnd_LuaDestroy_ObjUser );
		return 1;
	}
}

namespace driver
{
	static int bnd_LuaCreate_ScriptService( lua_State* L )
	{
		luabinder binder(L);
		ScriptService* h = NULL;

		AssertEx(false,"不应该在Lua中创建对象！");

		if( h == NULL )
		{
			//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING,"全局类%s的对象还没有创建就试图在Lua脚本中调用，请检查初始化时序！！" , "ScriptService" );
			return 0;
		}
		binder.PushUserType(h,/*h->GetRTTIName()*/"ScriptService" );
		return 1;
	}
	static int bnd_LuaDestroy_ScriptService(lua_State* L)
	{
		luabinder binder(L);
		ScriptService* h = (driver::ScriptService*) binder.CheckUserType(1,"ScriptService");
		AssertEx(h!=null_ptr,"ScriptService = null_ptr");
		return 0;
	}
	static const luaL_reg lib_ScriptService[] = {
		{"Create", bnd_LuaCreate_ScriptService},
		{NULL, NULL}
	};
	static int luaopen_ScriptService (lua_State* L)
	{
		luabinder binder(L);
		binder.Init("ScriptService", 0  , lib_ScriptService , bnd_LuaDestroy_ScriptService );
		return 1;
	}
}

#endif //_SCRIPT_INTERFACE_INL_