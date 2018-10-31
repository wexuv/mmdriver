#最低版本
cmake_minimum_required(VERSION 3.0)

#工程名
project(MMCore C CXX)

set(LIB_PREFIX lib)
set(MMCore_DEBUG_POSTFIX "d" CACHE STRING "Default debug postfix")

#工程include
include_directories(../../include ../../lib/threadpool ../../lib/boost ../../lib/lua/include ../../lib/mysql/include ../../lib/sqlite3 ../../lib/tinyxml ../../lib/redis/include)

#源代码目录
aux_source_directory(../../src/common SRC_COMMON)
aux_source_directory(../../src/db SRC_DB)
aux_source_directory(../../src/network SRC_NETWORK)
aux_source_directory(../../src/service SRC_SERVICE)

#lua
#set(SRC_LUA ../../lib/lua/lapi.c ../../lib/lua/lauxlib.c)

#protobuf
#set(SRC_PROTOBUF ../../lib/proto/importer.cc)

if(WIN32)
	add_definitions(-D__WINDOWS__ -D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
elseif(UNIX)
	add_definitions(-D__LINUX__ -D_DEBUG)
	set(CMAKE_CXX_FLAGS "-g -O2")
	set(CMAKE_VERBOSE_MAKEFILE on)
endif()

#生成静态库
add_library(libMMCore ${SRC_COMMON} ${SRC_DB} ${SRC_NETWORK} ${SRC_SERVICE} )

set_target_properties(libMMCore PROPERTIES
    OUTPUT_NAME ${LIB_PREFIX}MMCore
    DEBUG_POSTFIX "${MMCore_DEBUG_POSTFIX}")