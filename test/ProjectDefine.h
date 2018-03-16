#ifndef _PROJECT_DEFINE_H_
#define _PROJECT_DEFINE_H_

//ʹ��curl�� 
#define _MM_USE_CURL

#ifdef _MM_USE_CURL

#include "curl/include/curl.h"

#ifdef _DEBUG
#pragma comment(lib,"libcurld.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif

#pragma comment(lib,"wldap32.lib")

#endif//_MM_USE_CURL


//ʹ��protobuf
#define _MM_USE_PROTOBUF

#ifdef _MM_USE_PROTOBUF

#ifdef _DEBUG
#pragma comment(lib,"libprotobufd.lib")
#else
#pragma comment(lib,"libprotobuf.lib")
#endif

#endif//_MM_USE_PROTOBUF


#endif //_PROJECT_DEFINE_H_