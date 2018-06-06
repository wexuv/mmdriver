#ifndef _DB_STRUCT_H_
#define _DB_STRUCT_H_

#include "Object.h"
#include "time_ex.h"
#include "DBStruct.pb.h"

namespace driver
{
	enum
	{
		CHAR_NAME_LENGTH	= 32,
	};
		
	enum
	{
		MAX_BLOB_INFO_LENGTH		= 0xFFFF,
	};

	template <class TYPE> 
	bool EncodeBlob (TYPE& rkInfo, char* pBuff, tint32& usSize)
	{
		if(!rkInfo.SerializePartialToArray(pBuff,usSize))
		{
			return false;
		}
		usSize = rkInfo.ByteSize();
		pBuff[usSize]=0;

		return true;
	}

	template <class TYPE>
	bool DecodeBlob (TYPE& rkInfo, char* pBuff, tint32 usSize)
	{
		if(!rkInfo.ParseFromArray(pBuff,usSize))
		{
			return false;

		}
		return true;
	}

	struct CharData
	{
		tuint32 m_nID;
		tchar	m_szCharName[CHAR_NAME_LENGTH];
		google::protobuf::DBCharTitle	m_TitleData;

		void Clear()
		{
			m_nID = invalid_id;
			m_szCharName[0] = '\0';
		}
	};
}


#endif //_DB_STRUCT_H_