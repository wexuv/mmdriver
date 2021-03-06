#include "common/luaobject.h"
#include "common/luabinder.h"

namespace driver
{
	const char *const ms_LuaTNames[] = {
		"nil", "boolean", "lightuserdata", "number",
		"string", "table", "function", "userdata", "thread"
	};

	luanumber::luanumber( luastate* l ):luaobject(l)
	{
		m_fValue = 0.0f;
	}
	luanumber::luanumber( luastate* l , int index ):luaobject(l,index)
	{
		m_fValue = 0.0f;
	}

	void luanumber::GetFromStack( )
	{
		m_fValue = static_cast<double>(lua_tonumber(m_pkLuaState->GetLuaState(), m_iStackIndex));
	}

	tstring luanumber::ToString() const
	{
		tchar szValue[32];

		tsnprintf(szValue, 32, "%.10g", ToFloat());

		return tstring(szValue);
	}

	luastring::luastring(luastate* l ):luaobject(l)
	{ 
		m_strValue = "";
	}
	luastring::luastring( luastate* l , int index ):luaobject(l,index)
	{
		m_strValue = "";
	}

	void luastring::GetFromStack()
	{
		m_strValue = static_cast<const char*>(lua_tostring(m_pkLuaState->GetLuaState(), m_iStackIndex));
	}

	luatable::luatable( luastate* l ) : luaobject(l)
	{

	}

	luatable::luatable( luastate* l , int index ) : luaobject(l,index)
	{

	}
	luatable::~luatable()
	{
		LuaObjectMap::iterator iter = m_KeyObjs.begin();
		for( ; iter != m_KeyObjs.end(); ++ iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_KeyObjs.clear();
	}

	void luatable::GetFromStack( )
	{
		lua_State* ls = m_pkLuaState->GetLuaState();
		/*
		Pops a key from the stack, and pushes a key-value pair from the table at 
		the given index (the "next" pair after the given key). If there are no 
		more elements in the table, then lua_next returns 0 (and pushes nothing).
		*/
		lua_pushnil( ls );
		while( lua_next(ls, -2) )
		{
			/* uses 'key' (at index -2) and 'value' (at index -1) */
			char pname[256];
			int luatype = lua_type(ls, -2);
			if( luatype == LUA_TSTRING ) 
			{
				STRNCPY( pname, lua_tostring(ls,-2), 256);
			}
			else if( luatype == LUA_TNUMBER ) 
			{
				tsnprintf( pname, 256 , "%d", int(lua_tonumber(ls,-2)) );
			}
			else
			{
				//GfxWriteLog( LOG_ERROR_FILELINE, LOG_SWITCH_ERROR , "Lua Table Iteration error!!!" );
				lua_pop(ls, 2);
				return;
			}
			luaobject* pvar = m_pkLuaState->GetLuaObjectFromStack(-1);
			if( pvar )
			{
				LuaObjectMap::iterator iter = m_KeyObjs.find( pname );
				if ( iter != m_KeyObjs.end() )
				{
					SAFE_DELETE(iter->second);
					m_KeyObjs.erase( iter );
				}
				pvar->SetName( tstring( pname ) );
				m_KeyObjs.insert( std::make_pair( tstring( pname ) , pvar ) );
			}

			lua_pop( ls, 1 );
		}
	}

	luaobject* luatable::GetLuaObject(const tstring& strName)
	{
		bsvector<tstring> luaVec = string_utility::SplitString(strName,'.');
		if(luaVec.empty())
			return null_ptr;

		return GetLuaObject(luaVec);
	}

	luaobject* luatable::GetLuaObject(const bsvector<tstring>& strVec)
	{
		if(strVec.empty())
			return null_ptr;

		luaobject* obj = this;

		bsvector<tstring>::const_iterator iterVec = strVec.begin();
		for( ; iterVec != strVec.end(); ++ iterVec)
		{
			if ( obj == null_ptr )
				return null_ptr;
			if( !obj->IsTable())
				return null_ptr;
			luatable* objChild = static_cast<luatable*>(obj);
			obj = objChild->GetChild(*iterVec);
		}
		return obj;
	}

	luaobject* luatable::GetChild(const tstring& strName)
	{
		LuaObjectMap::iterator iter = m_KeyObjs.find( strName );
		if ( iter == m_KeyObjs.end() )
			return null_ptr;

		return iter->second;
	}
}