#ifndef __SQLITE3_HANDLER_H__
#define __SQLITE3_HANDLER_H__

#include <sqlite3.h>
#include "common/driver.h"

namespace driver
{
	class SQLite_Result;

	class SQLite_Handler
	{
	public:

		enum
		{
			sqlite_filename_length = 256,
		};

	private:

		sqlite3* handler_;
		char file_name_[sqlite_filename_length+1];
		bool is_opened_;

	public:
		
		SQLite_Handler (void);
		~SQLite_Handler (void);

		explicit SQLite_Handler (const char* file_name);

		int32_t open (const char* file_name);

		int32_t close (void);

		//���ڷ�SELECT���(INSERT,UPDATE)
		int32_t execute (const char* sql);

		//����SELECT���,�õ�������ϵķ���
		int32_t execute (const char* sql, SQLite_Result& dbresult);

		//�����������SQLite��API�����Ĵ������
		int get_last_errno (void);

	private:

		int32_t open (void);

		SQLite_Handler& operator= (const SQLite_Handler& handler);
		SQLite_Handler (const SQLite_Handler& handler);
	};
}


#endif	//	__SQLITE3_HANDLER_H__


