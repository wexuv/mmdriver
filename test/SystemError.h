#ifndef _SYSTEM_ERROR_H_
#define _SYSTEM_ERROR_H_

namespace driver
{
	typedef uint32_t ResultType;

	enum enmSystemError
	{
		RE_SUCCESS = 0,
		RE_FAIL = 1,
		RE_LOGIN_NAME_ALREADY_USED	=	2,	//���� 
		RE_LOGIN_CREATE_CHAR_FAIL	= 3,	//������ɫʧ��
		RE_LOGIN_CREATE_CHAR_SUCCESS = 4,	//������ɫ�ɹ�
	};
}

#endif //_SYSTEM_ERROR_H_