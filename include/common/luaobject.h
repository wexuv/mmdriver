#ifndef _LUA_OBJECT_H_
#define _LUA_OBJECT_H_

#include "types.h"
#include "lib.h"
#include "define.h"
#include "outputstream.h"

namespace driver 
{
	class luastate;

	class luaobject
	{
	public:
		luaobject(luastate* l ):m_pkLuaState(l){ m_iStackIndex = -1;}
		luaobject( luastate* l , int index ):m_pkLuaState(l),m_iStackIndex(index){}

		virtual ~luaobject() {};
			
		virtual bool IsNil()                 const{ return false; }
		virtual bool IsBoolean()             const{ return false; }
		virtual bool IsLightUserData()       const{ return false; }
		virtual bool IsNumber()              const{ return false; }
		virtual bool IsString()              const{ return false; }
		virtual bool IsTable()               const{ return false; }
		virtual bool IsArray()               const{ return false; }
		virtual bool IsFunction()            const{ return false; }
		virtual bool IsUserData()            const{ return false; }

		virtual const char* GetTypeName()  const{ return "nil"; }
		virtual void        GetFromStack()   = 0;

	public:
		void        SetName( const tstring& name ){ m_strName = name; }
		tstring		GetName() const { return m_strName; }

	protected:
		luastate* m_pkLuaState;
		tint32    m_iStackIndex;
		tstring   m_strName;
	};

	class luastring : public luaobject
	{
	public:
		luastring(luastate* l );
		luastring( luastate* l , int index );
		virtual ~luastring(){};

		virtual bool		IsString()     const{ return true; }
		virtual const char* GetTypeName()  const{ return "string"; }
		virtual void		GetFromStack();

	public:
		virtual tstring     GetString() const {return m_strValue;};

	protected:
		tstring m_strValue;
	};
	
	class luatable : public luaobject
	{
	public:
		luatable( luastate* l );
		luatable( luastate* l , int index );
		virtual ~luatable();

		virtual bool		IsTable()      const{ return true; }
		virtual const char* GetTypeName()  const{ return "table"; }
		virtual void		GetFromStack();

	public:
		luaobject* GetLuaObject(const tstring& strName);

	protected:
		typedef std::map<tstring,luaobject*> LuaObjectMap;
		LuaObjectMap	m_KeyObjs;
	};
}
#endif //_LUA_OBJECT_H_