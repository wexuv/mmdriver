#include "driver.h"
#include "servicemanager.h"
#include "MonitorService.h"
#include "NetService.h"
#include "ClientService.h"
#include "ScriptService.h"
#include "HttpManagerService.h"
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
		luaobject* pHttpServiceCount = g_Config.GetLuaObject("Service.HttpManager.count");
		if(pHttpServiceCount == null_ptr)
			return;

		tint32 nThreadCount = pLuaThreadObj->ToInt();
		gServiceManager.Create(nThreadCount);

		MonitorService* pMonitorService = new MonitorService();
		gServiceManager.Register(pMonitorService);

		HttpManagerService* pHttpManagerService = new HttpManagerService();
		gServiceManager.Register(pHttpManagerService);
		for(tint32 i = 0; i < pHttpServiceCount->ToInt(); ++ i)
		{
			HttpService* pHttpService = pHttpManagerService->NewHttpService();
			gServiceManager.Register(pHttpService);
		}

		NetService* pNetService = new NetService();
		gServiceManager.Register(pNetService);

		//gServiceManager.Register(new ScriptService());
		ClientService* pClientService1 = new ClientService();
		ClientService* pClientService2 = new ClientService();
		gServiceManager.Register(pClientService1);
		gServiceManager.Register(pClientService2);

		gServiceManager.Run();

		SAFE_DELETE(pClientService2);
		SAFE_DELETE(pClientService1);
		SAFE_DELETE(pNetService);
		SAFE_DELETE(pHttpManagerService);
		SAFE_DELETE(pMonitorService);
	}
}

int main(int argc, char* argv[])
{
	__ENTER_PROJECT

	RunServiceManager();

	__LEAVE_PROJECT
	return 0;
}
