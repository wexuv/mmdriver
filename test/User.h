#ifndef _USER_H_
#define _USER_H_

#include "time_ex.h"

namespace driver
{
#define DECL_RTTI_NAME( name ) \
	virtual const char* GetRTTIName() const\
	{\
	static const char* s_local_name = (""#name"");\
	return s_local_name;\
};\

	struct UserData
	{
		tuint32 m_nID;

		void Clear()
		{
			m_nID = invalid_id;
		}
	};

	class ObjUser
	{
	public:
		ObjUser();
		virtual ~ObjUser();
		DECL_RTTI_NAME( ObjUser )

	protected:
		void Tick(const TimeData& rkTimeData);

	public:
		void	SetID(tuint32 nID) {m_kUserData.m_nID = nID;};
		tuint32 GetID()
		{
			return m_kUserData.m_nID;
		};

	private:
		UserData m_kUserData;
	};
}


#endif //_USER_H_