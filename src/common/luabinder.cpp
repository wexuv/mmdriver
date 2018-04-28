#include "luabinder.h"
//#include "..\test\ScriptInterface.inl"

namespace driver
{
	void luabinder::Init( const char* tname, const char* bname, const luaL_Reg *flist, int (*destroy) (lua_State*) )
	{
		/*
		���㷨��
		1.Ϊÿһ����Ҫ�������ഴ��һ���µ�metatable
		2.ÿһ����ʵ������һ��userdata����������������metatable
		3.��metatable�б������Ա������ַ���������պ���
		*/
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "++++++register class %s with base %s begin++++++" , tname , bname );
		int iStack = lua_gettop( m_pkLuaState );
		lua_newtable(m_pkLuaState);                 //����һ���µı�ѹ��ջ�� Top | newtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		lua_pushstring(m_pkLuaState,"v");			// ѹ��һ���ַ����������±�λ����-2��,v = weak-value stack[-1] = "V"  Top | "v" | newtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__mode");     //stack[-2][_mode]="v"�����ñ�Ϊweek value��ͬʱ����ջ�����ַ���  Top | newtable | Bottom
		lua_pushvalue(m_pkLuaState,-1);             //����λ��Ŀǰջ����Ԫ�أ����ǸղŴ����ı�  Top | newtable | newtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua meta table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		lua_setmetatable(m_pkLuaState,-2);          //�����Լ�Ϊ�Լ���Ԫ��  Top | newtable | Bottom
		lua_replace(m_pkLuaState,LUA_ENVIRONINDEX); //��ջ����Ԫ���滻Ŀǰ�Ļ�����������ջ���ı�  Top |  | Bottom
		luaL_register(m_pkLuaState,tname,flist);    //��ȫ�ֱ��д���һ����tnameΪ���ֵı�����flistΪ�䷽������ѹ��ջ��  Top | libtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua method table 0x%0x for %s" , lua_topointer( m_pkLuaState , -1 ) , tname );
		int nResult = luaL_newmetatable(m_pkLuaState,tname);      //��Registry�д���һ���µ�����Ϊtname��metatable. Top | newtable2 libtable | Bottom
		//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "create lua meta table 0x%0x in registry for %s" , lua_topointer( m_pkLuaStatem_pkLuaState , -1 ) , tname );
		/*if ( nResult == 0 )
		{
		GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "�Ѿ���%sע�ᵽLua Registry�У�" , tname );
		}
		else
		{
		GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "ע��%s��Lua Registry��,Ԫ��ָ��Ϊ" , tname );
		}*/
		lua_pushvalue(m_pkLuaState,-2);             //����libtable.��ѹ��ջ�� Top | libtable newtable2 libtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__index");    //����newtable2["__index"] = libtable��������ջ����libtable copy Top | newtable2 libtable | Bottom
		lua_pushcfunction(m_pkLuaState,destroy);    //ѹ��destroy���� Top | destry newtable2 libtable | Bottom
		lua_setfield(m_pkLuaState,-2,"__gc");       //����newtable2["__gc"] = destroy������destroy����ָ�� Top | newtable2 libtable | Bottom
		if (bname) 
		{
			luaL_getmetatable(m_pkLuaState,bname);	 //��û����metatable Top | base mt newtable2 libtable | Bottom
			if ( lua_topointer( m_pkLuaState , -1 ) == NULL )
			{
				//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING , "�ڱ�¶��%s��Luaʱ�������%s��û�б�¶������ʼ��˳��" , tname , bname );
			}
			//GfxWriteLog( LOG_LUABIND_FILELINE , LOG_SWITCH_LUABIND , "get base %s lua meta table 0x%0x for %s" ,bname , lua_topointer( m_pkLuaState , -1 ) , tname );
			lua_setfield(m_pkLuaState,-2,"_base");  // newtable2["_base"] = base mt   Top | newtable2 libtable | Bottom
		}
		lua_pop(m_pkLuaState,1);                    // ������һ��Push��ջ��Ԫ�أ���ջ��Ԫ�� Top | newtable2 | Bottom
		if (bname) 
		{
			lua_getfield(m_pkLuaState,LUA_GLOBALSINDEX,bname);//��û���ķ�����table Top | base table | newtable 2 | Bottom
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
		//���������ɵ��ǽ�C++����ָ��ѹջ�Ĳ���
		//��˵�һ����ѡ��ɶ���ָ����LightUserData��ʽѹ������ջ
		//��Ҫע����ǻ�����ֻ�д�Lua����C������ʱ��Ŵ���
		//Ȼ���ȫ�ֱ��в�ѯ�ö���ָ���Ƿ���ǰע�����
		//���û��ע�ᣬ��ô�ʹ���һ���µ�userdata���������metatable������ȥ
		//�ڶ�����Ϊ�˽������ͼ�飬��Ҫ�������ַ����Ͷ���ָ���ӳ�����뻷����
		//�����lua_pushvalue����ջ����-1��ʾ����ջ����Ԫ�ظ���һ�ݣ���ѹ��
		//����ѹ�������ַ���
		//�����lua_rawset,�����������lua_settable��Ҫ������������һ����Key���ڶ�����Value
		//���Զ���ջ�ϵ�������Ԫ�أ�����LUA_GLOBALSINDEX��tableȡ����ִ��table[udata]=tname�Ĳ���
		//LuaObjectDebugWindowOutputStream debugWin;
		//GfxLuaStackDump( debugWin , m_pkLuaState );

		if( !strcmp(tname , "MM_BaseObject" ) )
		{
			//GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING ,
			//	"����0x%0xû����дGetRTTIName�麯�����������Lua���󷽷����ʲ���ȷ������LuaBinder::PushUserType�����öϵ�鿴������" , udata );
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
			//Ϊ�˱�֤��ע���ʹ�õ�ʱ��������ͬ���������õģ�������Ҫ���ж�����
			//����luaL_checkudataʧ��ʱ�����exist��ֱ���˳���������Ҫ����дһ��ֻ�Ǽ���
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
				//��������ͬ��ָ��ָ���˲�ͬ����ʵ����ַ
				//������������������´���һ��
				lua_pop( m_pkLuaState , 3 );
				_register_lua_object( udata , tname , false );
				/*GfxWriteLog( LOG_WARNING_FILELINE , LOG_SWITCH_WARNING , 
				"��鵽��%s����0x%0x���ڱ�¶��Lua��ʱ��ʹ�õĲ�ͬ��������"
				"��ᵼ���޷���Lua�����е�����ȷ��C++���󷽷�������PushUserType���ã�" , 
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
		//�������Ҫ��ջ����Nil���������������޷�ֱ�Ӱ�C++����ָ�봫��Lua������
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
			//	"�޷������%s�ķ����������Ƿ����REGISTER_INIT_LUA_EXPORT_OBJECT_FUNC(%s),�����Ƿ���ʹ���˸���Ľӿڣ���û�б�¶���࣬��������������д��GetRTTIName", tname , tname );
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

		////���ؾ�̬�⺯��(c++)->(lua)
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
			//GfxWriteLog(LOG_ERROR_FILELINE, LOG_SWITCH_ERROR ,"The Error: %s", sErrorStr.c_str());
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

	void luastate::DumpStack( OutputStreamFile& file) 
	{
		int i;
		int top = lua_gettop(m_L);
		for (i = 1; i <= top; i++) {  /* repeat for each level */
			int t = lua_type(m_L, i);
			switch (t) {

			case LUA_TSTRING:  /* strings */
				file.Print("`%s'", lua_tostring(m_L, i));
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
			//#pragma NOTE("�����жϷ�ʽ�޷�֧�ֲ���Ԫ�����ַ���������Lua����")
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