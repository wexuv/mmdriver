#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include "types.h"
#include "std_ex.h"

namespace driver
{
	class string_utility
	{
	public:
		static bsvector<tstring>		SplitString( const tstring& rkSrc , tchar split );
	};
}
#endif //_STRING_UTILITY_H_