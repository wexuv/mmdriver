#ifndef _PROJECT_DEFINE_H_
#define _PROJECT_DEFINE_H_

//使用curl库 
#define _MM_USE_CURL

#ifdef _MM_USE_CURL

#include "curl/include/curl.h"

#if defined(__WINDOWS__)

#ifdef _DEBUG
#pragma comment(lib,"libcurld.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif

#pragma comment(lib,"wldap32.lib")

#endif//__WINDOWS__

#endif//_MM_USE_CURL


//使用protobuf
#define _MM_USE_PROTOBUF

#ifdef _MM_USE_PROTOBUF

#if defined(__WINDOWS__)

#ifdef _DEBUG
#pragma comment(lib,"libprotobufd.lib")
#else
#pragma comment(lib,"libprotobuf.lib")
#endif

#endif//__WINDOWS__

#endif//_MM_USE_PROTOBUF


#endif //_PROJECT_DEFINE_H_