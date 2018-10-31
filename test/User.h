#ifndef _USER_H_
#define _USER_H_

#include "Object.h"
#include "common/time_ex.h"
#include "DBStruct.h"

namespace driver
{
	class ObjUser : public Object
	{
	public:
		ObjUser();
		virtual ~ObjUser();
		DECL_RTTI_NAME( ObjUser )

	protected:
		void Tick(const TimeData& rkTimeData);

	public:
		void	SetID(tuint32 nID) {m_kCharData.m_nID = nID;};
		tuint32 GetID()
		{
			return m_kCharData.m_nID;
		};

	private:
		CharData m_kCharData;
	};
}


#endif //_USER_H_