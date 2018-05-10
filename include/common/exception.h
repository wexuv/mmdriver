#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "types.h"
#include "lib.h"
#include "define.h"

namespace driver 
{

	/*
	�̰߳�ȫ
	*/
	void _do_assert_(const tchar* szFile,tuint32 nline,const tchar* szFunc,const tchar* szExpr, const tchar* szMsg, bool bReThrow);

	/*
	AssertEx: ���ʧ��->��ӡassert��־->����->�����׳�һ���쳣,���Ĵ�����ܲ��ᱻִ��
	*/
#define AssertEx(expr,msg) \
	{	\
	if(!(expr))	\
	{	\
	_do_assert_(__FILE__,__LINE__,__FUNCTION_NAME__,#expr,msg,true);	\
	}	\
	}

	/*
	������ʼ�ͽ���λ�ü�__ENTER_FUNCTION,__LEAVE_FUNCTION
	����֮�������쳣�ᱻ���񣬲����ϲ��׳��쳣
	*/
#define __ENTER_FUNCTION \
	{ \
	try \
	{
#define __LEAVE_FUNCTION \
	}\
	catch(const std::exception &err) \
	{ \
	AssertEx(false,err.what()); \
	} \
	catch(const std::string &err) \
	{ \
	AssertEx(false,err.c_str()); \
	} \
	catch(const tchar *err) \
	{ \
	AssertEx(false,err); \
	} \
	catch(...) \
	{ \
	AssertEx(false,__FUNCTION_NAME__); \
	} \
	}

	/*
	VerifyEx���ԣ���AssertEx���ƣ��������ٴ��׳��쳣
	*/
#define VerifyEx(expr,msg) \
	{ \
	if(!(expr)) \
	{ \
	_do_assert_(__FILE__,__LINE__,__FUNCTION_NAME__,#expr,msg,false); \
	} \
	}

	/*
	��__ENTER_FUNCTION,__LEAVE_FUNCTION���ƣ������м���쳣�ᱻ���񣬵��������׳��쳣
	*/
#define __ENTER_PROJECT \
	{ \
	try \
	{
#define __LEAVE_PROJECT \
	}\
	catch(const std::exception &err) \
	{ \
	VerifyEx(false,err.what()); \
	} \
	catch(const std::string &err) \
	{ \
	VerifyEx(false,err.c_str()); \
	} \
	catch(const tchar *err) \
	{ \
	VerifyEx(false,err); \
	} \
	catch(...) \
	{ \
	VerifyEx(false,__FUNCTION_NAME__); \
	} \
	}

	/*
	���ص��쳣�󣬿���Ӵ������
	*/
#define __ENTER_PROTECT_EX \
	{ \
	bool _bExpCatched = false; \
	try \
	{
#define __CATCH_PROJECT_EX \
	}\
	catch(const std::exception &err) \
	{ \
	VerifyEx(false,err.what()); \
	_bExpCatched = true; \
	} \
	catch(const std::string &err) \
	{ \
	VerifyEx(false,err.c_str()); \
	_bExpCatched = true; \
	} \
	catch(const tchar *err) \
	{ \
	VerifyEx(false,err); \
	_bExpCatched = true; \
	} \
	catch(...) \
	{ \
	VerifyEx(false,__FUNCTION_NAME__); \
	_bExpCatched = true; \
	} \
	if(_bExpCatched) \
	{
#define __LEAVE_PROJECT_EX \
	} \
	}

}
#endif //_EXCEPTION_H_