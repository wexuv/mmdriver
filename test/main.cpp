#include "driver.h"
#include "servicemanager.h"
#include "MonitorService.h"
#include "NetService.h"
#include "ClientService.h"
#include "ScriptService.h"
#include "HttpService.h"

using namespace driver;

//±‡“Î∆˜∞Ê±æ(Visual Studio 2010 || Gcc 4.4.6)
#if defined(__WINDOWS__)
	STATIC_ASSERT(_MSC_VER == 1600);
#elif defined(__LINUX__)
	STATIC_ASSERT(__GNUC__ == 4);
	STATIC_ASSERT(__GNUC_MINOR__ == 4);
	STATIC_ASSERT(__GNUC_PATCHLEVEL__ == 6);
#else
	#error windows or linux is required.
#endif

namespace driver
{
	void RunServiceManager()
	{
		gServiceManager.Create(2);
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
