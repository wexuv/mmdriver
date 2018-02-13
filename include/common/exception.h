#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "types.h"
#include "lib.h"
#include "define.h"

namespace driver 
{

	/*
	线程安全，非线程安全?
	*/
	void _do_assert_(const tchar* szFile,tuint32 nline,const tchar* szFunc,const tchar* szExpr, const tchar* szMsg, bool bReThrow);

	/*
	AssertEx: 如果失败->打印assert日志->报错->向外抛出一个异常,其后的代码可能不会被执行
	*/
#define AssertEx(expr,msg) \
	{	\
	if(!(expr))	\
	{	\
	_do_assert_(__FILE__,__LINE__,__FUNCTION_NAME__,#expr,msg,true);	\
	}	\
	}

	/*
	函数起始和结束位置加__ENTER_FUNCTION,__LEAVE_FUNCTION
	二者之间所有异常会被捕获，并向上层抛出异常
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
	VerifyEx特性：和AssertEx类似，但不会再次抛出异常
	*/
#define VerifyEx(expr,msg) \
	{ \
	if(!(expr)) \
	{ \
	_do_assert_(__FILE__,__LINE__,__FUNCTION_NAME__,#expr,msg,false); \
	} \
	}

	/*
	与__ENTER_FUNCTION,__LEAVE_FUNCTION类似，夹在中间的异常会被捕获，但不会再抛出异常
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
	拦截到异常后，可添加处理代码
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