#include "luabinder.h"
//#include "..\test\ScriptInterface.inl"

namespace driver
{
	void luabinder::Init( const char* tname, const char* bname, const luaL_Reg *flist, int (*destroy) (lua_State*) )
	{
		/*
		绑定算法：
		1.为每一个需要导出的类创建一个新的metatable
		2.每一个类实例都是一个userdata，并关联类所属的metatable
		3.在metatable中保存类成员函数地址和垃圾回收函数
		*/
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "++++++register class %s with base %s begin++++++" , tname , bname );
		int iStack = lua_gettop( m_pkLuaState );
		lua_newtable(m_pkLuaState);                 //创建一个新的表，压入栈顶 Top | newtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		lua_pushstring(m_pkLuaState,"v");			// 压入一个字符串，现在新表位置在-2了,v = weak-value stack[-1] = "V"  Top | "v" | newtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__mode");     //stack[-2][_mode]="v"，设置表为week value，同时弹出栈顶的字符串  Top | newtable | Bottom
		lua_pushvalue(m_pkLuaState,-1);             //复制位于目前栈顶的元素，就是刚才创建的表  Top | newtable | newtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua meta table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		lua_setmetatable(m_pkLuaState,-2);          //设置自己为自己的元表  Top | newtable | Bottom
		lua_replace(m_pkLuaState,LUA_ENVIRONINDEX); //用栈顶的元表替换目前的环境表，并弹出栈顶的表  Top |  | Bottom
		luaL_register(m_pkLuaState,tname,flist);    //在全局表中创建一个以tname为名字的表设置flist为其方法表，并压入栈顶  Top | libtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua method table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		int nResult = luaL_newmetatable(m_pkLuaState,tname);      //在Registry中创建一个新的名字为tname的metatable. Top | newtable2 libtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua meta table 0x%0x in registry for %s" , lua_topointer( m_pkLuaStatem_pkLuaState , -1 ) , tname );
		/*if ( nResult == 0 )
		{
		GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "已经有%s注册到Lua Registry中！" , tname );
		}
		else
		{
		GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "注册%s到Lua Registry中,元表指针为" , tname );
		}*/
		lua_pushvalue(m_pkLuaState,-2);             //复制libtable.并压入栈顶 Top | libtable newtable2 libtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__index");    //设置newtable2["__index"] = libtable，并弹出栈顶的libtable copy Top | newtable2 libtable | Bottom
		lua_pushcfunction(m_pkLuaState,destroy);    //压入destroy函数 Top | destry newtable2 libtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__gc");       //设置newtable2["__gc"] = destroy并弹出destroy函数指针 Top | newtable2 libtable | Bottom
		if (bname) 
		{
			luaL_getmetatable(m_pkLuaState,bname);	 //获得基类的metatable Top | base mt newtable2 libtable | Bottom
			if ( lua_topointer( m_pkLuaState , -1 ) == NULL )
			{
				//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING , "在暴露类%s到Lua时，其基类%s还没有暴露，检查初始化顺序" , tname , bname );
			}
			//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "get base %s lua meta table 0x%0x for %s" ,bname , lua_topointer( m_pkLuaState , -1 ) , tname );
			lua_setfield(m_pkLuaState,-2,"_base");  // newtable2["_base"] = base mt   Top | newtable2 libtable | Bottom
		}
		lua_pop(m_pkLuaState,1);                    // 弹出第一个Push进栈的元素，即栈底元素 Top | newtable2 | Bottom
		if (bname) 
		{
			lua_getfield(m_pkLuaState,LUA_GLOBALSINDEX,bname);//获得基类的方法表table Top | base table | newtable 2 | Bottom
			//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "get base %s lua method table 0x%0x for %s" , bname , lua_topointer( m_pkLuaState , -1 ) , tname );
			lua_setfield(m_pkLuaState,-2,"__index"); // newtable["__index"] = base table Top | newtable2 | Bottom
			lua_pushvalue(m_pkLuaState,-1);          // duplicate libtable
			lua_setmetatable(m_pkLuaState,-2);       // set itself as metatable
		}
		iStack = lua_gettop( m_pkLuaState );
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "++++++register class %s with base %s end++++++" , tname , bname );
		//LuaObjectDebugWindowOutputStream debugWin;
		//GfxLuaStackDump( debugWin , m_pkLuaState );
	}
	void luabinder::PushCFunction( const char* name , lua_CFunction pfnFunc )
	{
		lua_pushcfunction( m_pkLuaState , pfnFunc );
		lua_setglobal( m_pkLuaState , name );
	}
	void luabinder::PushConstant( const char* name , long value )
	{
		lua_pushinteger( m_pkLuaState , value );
		lua_setglobal( m_pkLuaState , name );
		/*LuaObjectDebugWindowOutputStream debugWin;
		GfxLuaStackDump( debugWin , m_pkLuaState );*/
	}
	void luabinder::PushUserType( void* udata,const char* tname )
	{
		//这个函数完成的是将C++对象指针压栈的操作
		//因此第一步首选完成对象指针以LightUserData形式压入虚拟栈
		//需要注意的是环境表只有从Lua调用C函数的时候才存在
		//然后从全局表中查询该对象指针是否以前注册过？
		//如果没有注册，那么就创建一个新的userdata，并把类的metatable关联上去
		//第二步，为了进行类型检查，需要把类名字符串和对象指针的映射填入环境表
		//因此用lua_pushvalue，将栈顶（-1表示虚拟栈顶）元素复制一份，并压入
		//接着压入类名字符串
		//最后用lua_rawset,这个函数类似lua_settable需要两个参数，第一个是Key，第二个是Value
		//会自动从栈上弹出两个元素，并把LUA_GLOBALSINDEX的table取出，执行table[udata]=tname的操作
		//LuaObjectDebugWindowOutputStream debugWin;
		//GfxLuaStackDump( debugWin , m_pkLuaState );

		if( !strcmp(tname , "MM_BaseObject" ) )
		{
			//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING ,
			//	"对象0x%0x没有重写GetRTTIName虚函数，可能造成Lua对象方法访问不正确！请在LuaBinder::PushUserType中设置断点查看！！！" , udata );
		}
		lua_pushlightuserdata(m_pkLuaState,udata);
		//current stack:function:0x1b02110  userdata:0x19bbf60
		lua_rawget(m_pkLuaState,LUA_GLOBALSINDEX);     // get udata in env table
		if (lua_isnil(m_pkLuaState,-1)) 
		{
			_register_lua_object( udata , tname , true );
		}
		else
		{
			//#if /*defined(_DEBUG) &&*/ defined( T6_CLIENT )
			//为了保证在注册和使用的时候都是用相同的类名调用的，这里需要进行额外检查
			//但是luaL_checkudata失败时会调用exist！直接退出，这里需要重新写一个只是检查的
			//void* p = luaL_checkudata (m_pkLuaState , -1 , tname );     // get metatable 
			void *p = lua_touserdata( m_pkLuaState ,  -1 );
			bool bValid = false;
			if (p != NULL) 
			{ 
				if (lua_getmetatable(m_pkLuaState, -1)) 
				{  /* does it have a metatable? */
					lua_getfield(m_pkLuaState, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
					if (lua_rawequal(m_pkLuaState, -1, -2)) 
					{  /* does it have the correct mt? */
						lua_pop(m_pkLuaState, 2);  /* remove both metatables */
						bValid = true;
					}
				}
			}
			if ( !bValid )
			{
				//可能是相同的指针指向了不同的类实例地址
				//遇到这种情况，就重新创建一次
				lua_pop( m_pkLuaState , 3 );
				_register_lua_object( udata , tname , false );
				/*GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING , 
				"检查到类%s对象0x%0x，在暴露到Lua的时候使用的不同的类名！"
				"这会导致无法在Lua函数中调用正确的C++对象方法，请检查PushUserType调用！" , 
				tname , udata );*/
			}
			//#endif
			//LuaObjectDebugWindowOutputStream kDebugStream;
			//GfxLuaStackDump( kDebugStream , m_pkLuaState );
			//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "get registered lua user data 0x%0x for %s instance 0x%0x" , lua_topointer( m_pkLuaState , -1 ) , tname , udata );
		}
	}
	void luabinder::_register_lua_object( void* udata,const char* tname , bool bPop )
	{
		//current stack:function:0x1b02110  nil:0x0
		//这里必须要把栈顶的Nil弹出，否则我们无法直接把C++对象指针传给Lua函数！
		//LuaObjectDebugWindowOutputStream kDebugStream;

		if ( bPop )
		{
			lua_pop( m_pkLuaState , 1 );
		}
		else
		{
			//GfxLuaStackDump( kDebugStream , m_pkLuaState );
		}
		//current stack:function:0x1b02110
		void** ubox = (void**)lua_newuserdata(m_pkLuaState,sizeof(void*));
		//if(!bPop)
		//	GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , 
		//	"create lua user data wrapper 0x%0x for class %s instance 0x%0x" ,
		//	ubox , tname , udata );
		*ubox = udata;                    // store address in udata
		luaL_getmetatable(m_pkLuaState,tname);     // get metatable 
		//if(!bPop)
		//	GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88  table:0x1afa258
		const void* pMetaTable = lua_topointer( m_pkLuaState , -1 );
		if ( pMetaTable == NULL )
		{
			//GfxWriteLog( LOG_ERROR_FILELINE , LOG_SWITCH_ERROR , 
			//	"无法获得类%s的方法表，请检查是否调用REGISTER_INIT_LUA_EXPORT_OBJECT_FUNC(%s),或者是否是使用了父类的接口，而没有暴露子类，但是又在子类重写了GetRTTIName", tname , tname );
		}
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND ,
		//	"get lua meta table 0x%0x for class %s instance 0x%0x" , 
		//	pMetaTable , tname , udata );
		lua_setmetatable(m_pkLuaState,-2);         // set metatable for udata
		//if(!bPop)
		//	GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88
		lua_pushlightuserdata(m_pkLuaState,udata); // push address
		//if(!bPop)
		//	GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88  userdata:0x19bbf60
		lua_pushvalue(m_pkLuaState,-2);            // push udata
		//if(!bPop)
		//	GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88  userdata:0x19bbf60  userdata:0x19bfc88
		lua_rawset(m_pkLuaState,LUA_GLOBALSINDEX); // table[address] = udata

		//GfxLuaStackDump( kDebugStream , m_pkLuaState );
		//current stack:function:0x1b02110    userdata:0x19bfc88
		//kDebugStream.Print("Create Count %d" , LS_LuaState::s_lua_userdata++);
	}
	void * luabinder::CheckUserType( int index, const char *tname )
	{
		lua_getfield(m_pkLuaState, LUA_REGISTRYINDEX, tname);   /* get metatable of given type */
		lua_getmetatable(m_pkLuaState,index);                   /* get associated metatable */
		while (lua_istable(m_pkLuaState,-1)) 
		{
			if (lua_rawequal(m_pkLuaState,-1,-2)) 
			{
				lua_pop(m_pkLuaState,2);
				return *((void**)lua_touserdata(m_pkLuaState,index));
			}
			lua_getfield(m_pkLuaState,-1,"_base");          // get mt._base
			lua_replace(m_pkLuaState,-2);                   // replace: mt = mt._base
		}
		luaL_typerror(m_pkLuaState,index,tname);
		return NULL;
	}

	luastate::luastate()
	{
		m_L = null_ptr;
	}
	luastate::~luastate()
	{
		close();
	}

	int LuaPanicCallback( lua_State *L )
	{
		//GfxWriteLog( LOG_TAG_ERROR, LOG_SWITCH_ERROR , "Lua Panic Called! Major Error Happened!!! %s", lua_tostring(L,-1) );
		return 0;
	}

	bool luastate::init(void* inst)
	{
		m_L = luaL_newstate();
		if(m_L == null_ptr)
			return false;

		lua_atpanic( m_L , LuaPanicCallback );

		luaL_openlibs(m_L);

		////加载静态库函数(c++)->(lua)
		//lua_pushcfunction( m_L,luaopen_ObjUser);
		//if (lua_pcall( m_L,0,0,0)) 
		//{
		//	//GfxWriteLog(LOG_TAG_ERROR, LOG_SWITCH_ERROR , "Lua Error: " , lua_tostring(m_L,-1) );
		//	lua_pop( m_L , -1 );
		//}

		////test
		//DoFile("./Script/test.lua");

		//LUA_STACK_START_CHECK( m_L );
		//lua_getglobal( m_L, "test" );

		//ObjUser obj;
		//obj.SetID(2);

		//LuaPushValue(m_L,obj);
		//tint32 nRet = lua_pcall(m_L,1,1,0);
		//if(nRet != 0)
		//{
		//	std::string sErrorStr = lua_tostring(m_L, -1);

		//	lua_pop(m_L,1);

		//	AssertEx(false,"123");

		//	return false;
		//}

		//tuint32 nID = (tuint32)lua_tointeger(m_L,-1);

		//lua_pop( m_L , 1 );

		//LUA_STACK_END_CHECK( m_L, 0 );

		//{
		//	LUA_STACK_START_CHECK( m_L );
		//	lua_getglobal( m_L, "test" );

		//	ObjUser obj;
		//	obj.SetID(1);

		//	LuaPushValue(m_L,obj);
		//	tint32 nRet = lua_pcall(m_L,1,1,0);
		//	if(nRet != 0)
		//	{
		//		std::string sErrorStr = lua_tostring(m_L, -1);

		//		lua_pop(m_L,1);

		//		AssertEx(false,"123");

		//		return false;
		//	}

		//	tuint32 nID = (tuint32)lua_tointeger(m_L,-1);

		//	lua_pop( m_L , 1 );

		//	LUA_STACK_END_CHECK( m_L, 0 );
		//}
		return true;
	}

	bool luastate::close()
	{
		lua_close(m_L);
		m_L = null_ptr;

		LuaObjectMap::iterator iter = m_CacheObjs.begin();
		for( ; iter != m_CacheObjs.end(); ++ iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_CacheObjs.clear();

		return true;
	}

	bool luastate::DoFile( const std::string& strName )
	{
		bool bErr = false;
		//BYTE* pkBuffer = NULL;
		//unsigned int uiBufferLen = 0;
		//if ( UT_FileManagerBase::GetSingletonPtr() != NULL 
		//	&& UT_FileManagerBase::GetSingletonPtr()->LoadDataFromPackage( strName.c_str(), pkBuffer, uiBufferLen ) )
		//{
		//	//char* pcBuffer = new char[uiBufferLen];
		//	//memcpy(pcBuffer, pkBuffer, uiBufferLen);
		//	bErr = ( luaL_loadbuffer( m_L, (char*)pkBuffer, uiBufferLen, strName.c_str() ) || lua_pcall(m_L, 0, LUA_MULTRET, 0) ) != 0;
		//	T_FREE( pkBuffer , Memory::MEMCATEGORY_GENERAL );
		//}
		//else
		{
			bErr = luaL_dofile(m_L, strName.c_str()) != 0;
		}

		if ( bErr ) 
		{
			std::string sErrorStr = lua_tostring(m_L, -1);

			AssertEx(false,sErrorStr.c_str());

			lua_pop( m_L , -1 );
			return false;	
		}
		return true;
	}

	void luastate::RegisterInitFunction( lua_CFunction fpInit )
	{
		lua_pushcfunction( m_L,fpInit);
		if (lua_pcall( m_L,0,0,0))
		{	
		//GfxWriteLog(LOG_TAG_ERROR, LOG_SWITCH_ERROR , "Lua Error: " , lua_tostring(m_L,-1) );
			lua_pop( m_L , -1 );
		}
	}

	void luastate::DumpStack( OutputStream& file) 
	{
		int i;
		int top = lua_gettop(m_L);
		for (i = 1; i <= top; i++) {  /* repeat for each level */
			int t = lua_type(m_L, i);
			switch (t) {

			case LUA_TSTRING:  /* strings */
				file.Print("'%s'", lua_tostring(m_L, i));
				break;

			case LUA_TBOOLEAN:  /* booleans */
				file.Print(lua_toboolean(m_L, i) ? "true" : "false");
				break;

			case LUA_TNUMBER:  /* numbers */
				file.Print("%g", lua_tonumber(m_L, i));
				break;

			default:  /* other values */
				file.Print("%s:0x%0x", lua_typename(m_L, t) , lua_topointer(m_L , i ) );
				break;

			}
			file.Print("  ");  /* put a separator */
		}
		file.Print("\n");  /* end the listing */
	}

	tint32 luastate::GetLuaObjectType( tint32 index )
	{
		int type =  lua_type( m_L, index);
		if(type == LUA_TTABLE)
		{
			//#pragma NOTE("这种判断方式无法支持部分元素是字符串索引的Lua数组")
			lua_checkstack(m_L, 1);
			lua_rawgeti( m_L, -1, 1 );
			type = lua_type(m_L, -1);
			lua_pop(m_L, 1);
			if( type == LUA_TNIL )
			{
				return LUA_TTABLE;
			}
			else
			{
				return LUA_TARRAY;
			}
		}
		return type;
	}

	luaobject* luastate::GetLuaObject(const tstring& strName)
	{
		LUA_STACK_START_CHECK( m_L );

		lua_getglobal( m_L, strName.c_str() );

		luaobject* pkNewObj = GetLuaObjectFromStack( -1 );

		if(pkNewObj != null_ptr)
		{
			pkNewObj->SetName(strName);
		}

		lua_pop( m_L , 1 );

		LUA_STACK_END_CHECK( m_L, 0 );

		return pkNewObj;
	}

	luaobject* luastate::GetLuaObjectFromCache(const tstring& strName)
	{
		bsvector<tstring> luaVec = string_utility::SplitString(strName,'.');
		if(luaVec.empty())
			return null_ptr;

		tstring strRoot = *(luaVec.begin());
		luaVec.erase(luaVec.begin());

		LuaObjectMap::iterator iter = m_CacheObjs.find(strRoot);
		if(iter != m_CacheObjs.end())
		{
			if(!luaVec.empty())
			{
				if(iter->second->IsTable())
				{
					luatable* objTable = static_cast<luatable*>(iter->second);
					return objTable->GetLuaObject(luaVec);					
				}
				return null_ptr;
			}
			return iter->second;
		}
		luaobject* pluaObj = GetLuaObject(strRoot);
		if(pluaObj != null_ptr)
		{
			m_CacheObjs.insert(LuaObjectMap::value_type(strRoot,pluaObj));
			if(!luaVec.empty())
			{
				if(pluaObj->IsTable())
				{
					luatable* objTable = static_cast<luatable*>(pluaObj);
					return objTable->GetLuaObject(luaVec);
				}
				return null_ptr;
			}

			return pluaObj;
		}
		return null_ptr;
	}

	tint32 luastate::GetLuaObjectFromCache(LuaTypeWrapper<tint32>, const tstring& strName)
	{
		luaobject* pluaObj = GetLuaObjectFromCache(strName);
		if(pluaObj != null_ptr)
		{
			return pluaObj->ToInt();
		}
		return 0;
	}

	tfloat64 luastate::GetLuaObjectFromCache(LuaTypeWrapper<tfloat64>, const tstring& strName)
	{
		luaobject* pluaObj = GetLuaObjectFromCache(strName);
		if(pluaObj != null_ptr)
		{
			return pluaObj->ToFloat();
		}
		return 0.0f;
	}

	tstring luastate::GetLuaObjectFromCache(LuaTypeWrapper<tstring>, const tstring& strName)
	{
		luaobject* pluaObj = GetLuaObjectFromCache(strName);
		if(pluaObj != null_ptr)
		{
			return pluaObj->ToString();
		}
		return "";
	}

	luaobject* luastate::GetLuaObjectFromStack(tint32 index)
	{
		luaobject* pkNewObj = NULL;
		tint32 type = GetLuaObjectType( index );
		switch ( type )
		{
		case LUA_TNUMBER:
			pkNewObj = new luanumber( this , index );
			break;
		case LUA_TTABLE:
			pkNewObj = new luatable( this , index );
			break;
		case LUA_TSTRING:
			pkNewObj = new luastring( this , index );
			break;
		//case LUA_TBOOLEAN:
		//	pkNewObj = tnew LS_LuaBoolean( m_L , index );
		//	break;
		//case LUA_TFUNCTION:
		//	pkNewObj = tnew LS_LuaFuncObject( m_L , index );
		//	break;
		//case LUA_TARRAY:
		//	pkNewObj = tnew LS_LuaArray(m_L,index);
		//	break;
		default:
			break;
		}
		if ( pkNewObj != NULL )
		{
			pkNewObj->GetFromStack();
		}
		return pkNewObj;
	}
}