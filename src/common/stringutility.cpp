#include "stringutility.h"

namespace driver
{
	bsvector<tstring> string_utility::SplitString( const tstring& rkSrc , tchar split )
	{
		bsvector<tstring> kResult;
		kResult.clear();

		tstring kFormatPath = rkSrc;
		tstring::size_type kPos = kFormatPath.find_first_of(split);

		while(kPos != tstring::npos && kFormatPath.length() > 0)
		{
			tstring kSubStr = kFormatPath.substr(0,kPos);
			kFormatPath = kFormatPath.substr(kPos + 1);

			kResult.push_back(kSubStr);

			kPos = kFormatPath.find_first_of(split);
		}
		if ( kPos == tstring::npos && kFormatPath.length() > 0 )
		{
			kResult.push_back( kFormatPath );
		}
		return kResult;
	}
}