#ifndef _LUA_OBJECT_H_
#define _LUA_OBJECT_H_

#include "types.h"
#include "lib.h"
#include "define.h"
#include "outputstream.h"
#include "stringutility.h"

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

		virtual tfloat64	ToFloat()	const { AssertEx(false,"");return 0.0f;}
		virtual tint32		ToInt()		const { AssertEx(false,"");return 0;}
		virtual bool		ToBool()	const { AssertEx(false,"");return false;}
		virtual tstring		ToString()	const { AssertEx(false,"");return "";}

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

	typedef std::map<tstring,luaobject*> LuaObjectMap;

	class luanumber : public luaobject
	{
	public:
		luanumber( luastate* l );
		luanumber( luastate* l , int index );
		virtual ~luanumber(){};

		virtual bool		IsNumber()		const{ return true; }
		virtual const char*	GetTypeName()	const{ return "number"; }
		virtual void		GetFromStack();

		virtual tfloat64	ToFloat()	const { return m_fValue;}
		virtual tint32		ToInt()		const { return (tint32)m_fValue;}
		virtual bool		ToBool()	const {	return ((int)m_fValue) != 0;}
		virtual tstring		ToString()	const;

	public:
		virtual tfloat64     GetNumber() const {return m_fValue;};

	protected:
		 tfloat64	m_fValue;
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

		virtual tstring		ToString()	const {return m_strValue;}

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
		luaobject* GetLuaObject(const bsvector<tstring>& strVec);

	private:
		luaobject* GetChild(const tstring& strName);

	protected:
		LuaObjectMap	m_KeyObjs;
	};
}
#endif //_LUA_OBJECT_H_