#ifndef _SYSTEM_ERROR_H_
#define _SYSTEM_ERROR_H_

namespace driver
{
	typedef uint32_t ResultType;

	enum enmSystemError
	{
		RE_SUCCESS = 0,
		RE_FAIL = 1,
		RE_LOGIN_NAME_ALREADY_USED	=	2,	//重名 
		RE_LOGIN_CREATE_CHAR_FAIL	= 3,	//创建角色失败
		RE_LOGIN_CREATE_CHAR_SUCCESS = 4,	//创建角色成功
	};
}

#endif //_SYSTEM_ERROR_H_