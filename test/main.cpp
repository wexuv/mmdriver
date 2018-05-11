#include "driver.h"
#include "servicemanager.h"
#include "MonitorService.h"
#include "NetService.h"
#include "ClientService.h"
#include "ScriptService.h"
#include "HttpService.h"
#include "Config.h"

using namespace driver;

//±àÒëÆ÷°æ±¾(Visual Studio 2010 || Gcc 4.4.6)
#if defined(__WINDOWS__)
	STATIC_ASSERT(_MSC_VER == 1600);
#elif defined(__LINUX__)
	STATIC_ASSERT(__GNUC__ == 4);
	STATIC_ASSERT(__GNUC_MINOR__ == 8);
	STATIC_ASSERT(__GNUC_PATCHLEVEL__ == 5);
#else
	#error windows or linux is required.
#endif

namespace driver
{
	void RunServiceManager()
	{
		g_Config.Init();

		luaobject* pLuaServiceObj = g_Config.GetLuaObject("Service");
		if(pLuaServiceObj == null_ptr)
		{
			AssertEx(false,"load service config failed");
			return;
		}
		luaobject* pLuaThreadObj = g_Config.GetLuaObject("ThreadCount");
		if(pLuaThreadObj == null_ptr)
		{
			AssertEx(false,"load thread info failed");
			return;
		}

		tint32 nThreadCount = pLuaThreadObj->ToInt();
		gServiceManager.Create(nThreadCount);


		gServiceManager.Register(new MonitorService());
		gServiceManager.Register(new HttpService());
		gServiceManager.Register(new NetService());
		//gServiceManager.Register(new ScriptService());
		gServiceManager.Register(new ClientService());
		gServiceManager.Register(new ClientService());
		gServiceManager.Run();
	}
}

int main(int argc, char* argv[])
{
	__ENTER_PROJECT

	RunServiceManager();

	__LEAVE_PROJECT
	return 0;
}
