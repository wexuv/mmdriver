#ifndef _SCRIPT_SERVICE_H_
#define _SCRIPT_SERVICE_H_

#include "Service.h"
#include "log.h"
#include "luabinder.h"
#include "User.h"

namespace driver
{
	class ScriptService : public Service
	{
	public:
		ScriptService();
		virtual ~ScriptService();

	protected:
		virtual bool Init();
		virtual void Tick(const TimeData& rkTimeData);

	protected:
		luastate* m_pLuaState;

		Log_Engine	m_stLogEngine;
	};
}

#endif //_SCRIPT_SERVICE_H_