#ifndef _LUABINDER_H_
#define _LUABINDER_H_

#include "types.h"
#include "lib.h"
#include "define.h"
#include "outputstream.h"
#include "luaobject.h"

extern "C" {  
#include <lua.h>
#include <lauxlib.h>  
#include <lualib.h>
}  

#ifdef _DEBUG
#define LUA_STACK_START_CHECK( L )                             \
	int __luaStartSize( lua_gettop( L ) );
#define LUA_STACK_END_CHECK( L, S )                            \
	int __luaEndSize( lua_gettop( L ) );                        \
	AssertEx( __luaEndSize == ( __luaStartSize + ( S ) ),"" );
#else
#define LUA_STACK_START_CHECK( L )                            
#define LUA_STACK_END_CHECK( L, S )                           
#endif

#define LUA_TARRAY      ( LUA_TTHREAD + 1 )

#define LUA_EXPORT_OBJECT_FUNC( className )	(&luaopen_##className)

namespace driver 
{
	class luabinder
	{
	public:
		luabinder(lua_State* l ):m_pkLuaState(l){}

	public:
		void Init( const char* tname, const char* bname, const luaL_Reg *flist, int (*destroy) (lua_State*) );
		void PushCFunction( const char* name , lua_CFunction pfnFunc );
		void PushConstant( const char* name , long value );
		void PushUserType( void* udata,const char* tname );
		void * CheckUserType( int index, const char *tname );

	protected:
		void _register_lua_object( void* udata,const char* tname , bool bPop );

	protected:
		lua_State* m_pkLuaState;
	};

	class luastate
	{
	public:
		luastate();
		virtual ~luastate();

		bool init(void* inst);
		bool close();

		lua_State* GetLuaState() {return m_L;};

		bool DoFile(const std::string& strName);

		void RegisterInitFunction( lua_CFunction fpInit );

		void DumpStack( OutputStream& file);

		tint32     GetLuaObjectType( tint32 index );
		luaobject* GetLuaObjectFromCache(const tstring& strName);
		luaobject* GetLuaObjectFromStack(tint32 index);

	private:
		luaobject* GetLuaObject(const tstring& strName);

	protected:
		lua_State*		m_L;
		LuaObjectMap	m_CacheObjs;
	};

	struct LuaNil{};

	inline void LuaPushValue(lua_State* L, bool value)				{  lua_pushboolean(L, value);  }
	inline void LuaPushValue(lua_State* L, char value)				{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, unsigned char value)		{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, short value)				{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, unsigned short value)	{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, int value)				{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, unsigned int value)		{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, long value)				{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, unsigned long value)		{  lua_pushinteger(L, value);  }
	inline void LuaPushValue(lua_State* L, long long value)			{  lua_pushinteger(L, (lua_Integer)value);  }
	inline void LuaPushValue(lua_State* L, double value)			{  lua_pushnumber(L, (lua_Number)value);  }
	inline void LuaPushValue(lua_State* L, float value)				{  lua_pushnumber(L, (lua_Number)value);  }
	inline void LuaPushValue(lua_State* L, const char* value)		{  lua_pushstring(L, value);  }
	inline void LuaPushValue(lua_State* L, const LuaNil&)			{  lua_pushnil(L);  }
	inline void LuaPushValue(lua_State* L, lua_CFunction value)		{  lua_pushcclosure(L, value, 0);  }

	inline void LuaPushValue(lua_State* L, std::string& value)								{  lua_pushstring(L, value.c_str());  }
	inline void LuaPushValue(lua_State* L, const std::string& value)						{  lua_pushstring(L, value.c_str());  }

	template<class ValueType> 
	inline void	LuaPushValue(lua_State* L, ValueType& value)		
	{  
		luabinder binder(L);
		binder.PushUserType( &value , value.GetRTTIName());
	}
}
#endif //_LUABINDER_H_