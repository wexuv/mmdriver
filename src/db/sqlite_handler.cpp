#include "common/driver.h"
#include "db/sqlite_handler.h"
#include "db/sqlite_result.h"

namespace driver
{
	SQLite_Handler::SQLite_Handler (void):
		handler_ (NULL),
		is_opened_ (false)
	{
		file_name_[0] = '\0';
	}

	SQLite_Handler::~SQLite_Handler (void)
	{
		close();
	}

	SQLite_Handler::SQLite_Handler (const char* file_name)
	{
		STRNCPY(file_name_, file_name, sizeof(file_name_));
		is_opened_ = false;
		this->open ();
	}

	int32_t SQLite_Handler::open (const char* file_name)
	{
		if (is_opened_)
		{
			this->close ();
		}

		STRNCPY(file_name_, file_name, sizeof(file_name_));

		return this->open ();
	}

	int32_t SQLite_Handler::open (void)
	{
		if (is_opened_)
		{
			this->close ();
		}

		int ret = sqlite3_open (file_name_, &handler_);
		if (SQLITE_OK == ret)
		{
			is_opened_ = true;
			return success;
		}
		else
		{
			is_opened_ = false;
		}

		return fail;
	}

	int32_t SQLite_Handler::close (void)
	{
		if (NULL != handler_)
		{
			sqlite3_close (handler_);
			handler_ = NULL;
			is_opened_ = false;
		}

		return success;
	}

	int32_t SQLite_Handler::execute (const char* sql)
	{
		if (NULL == sql)
		{
			return fail;
		}

		if (!is_opened_)
		{
			if (success != this->open ())
			{
				return fail;
			}
		}

		char* error = NULL;
		int ret = sqlite3_exec (handler_, sql, NULL, NULL, &error);
		if (SQLITE_OK == ret)
		{
			return success;
		}

		if (NULL != error)
		{
			sqlite3_free (error);
		}

		return fail;
	}

	int32_t SQLite_Handler::execute (const char* sql, SQLite_Result& dbresult)
	{
		if (NULL == sql)
		{
			return fail;
		}

		if (!is_opened_)
		{
			if (success != this->open ())
			{
				return fail;
			}
		}

		char** result = NULL;
		int row = 0;
		int col = 0;
		char* error = NULL;
		int ret = sqlite3_get_table (handler_, sql, &result, &row, &col, &error);
		if (SQLITE_OK == ret && NULL != result && 0 <= row && 0 <= col)
		{
			dbresult.set_result (result, (size_t)row, (size_t)col);
			return success;
		}

		if (NULL != error)
		{
			sqlite3_free (error);
		}

		return fail;
	}

	int SQLite_Handler::get_last_errno (void)
	{
		return sqlite3_errcode (handler_);
	}

}


