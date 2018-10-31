#include <stdlib.h>
#include <sqlite3.h>
#include "db/sqlite_result.h"

namespace driver
{
	SQLite_Result::SQLite_Result (void):
		result_ (NULL),
		row_count_ (0),
		col_count_ (0),
		current_row_ (0),
		current_col_ (0)
	{

	}

	SQLite_Result::~SQLite_Result (void)
	{
		if (NULL != result_)
		{
			sqlite3_free_table (result_);
			result_ = NULL;
		}
	}

	void SQLite_Result::set_result (char** result, size_t row_count, size_t col_count)
	{
		if (NULL != result_)
		{
			free_result ();
		}

		result_ = result;
		row_count_ = row_count;
		col_count_ = col_count;
		current_row_ = 0;
		current_col_ = 0;
	}

	void SQLite_Result::free_result (void)
	{
		if (NULL != result_)
		{
			sqlite3_free_table (result_);
			result_ = NULL;
			row_count_ = 0;
			col_count_ = 0;
			current_row_ = 0;
			current_col_ = 0;
		}
	}

	bool SQLite_Result::fetch_next_row (void)
	{
		if (NULL == result_)
		{
			return false;
		}

		if (current_row_ < row_count_)
		{
			++current_row_;
			current_col_ = 0;
			return true;
		}

		return false;
	}

	SQLite_Result& SQLite_Result::operator >> (int8_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (int8_t)atoi(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (int16_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (int16_t)atoi(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (int32_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (int32_t)atoi(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (int64_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (int64_t)tatoll(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (uint8_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (uint8_t)atoi(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (uint16_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (uint16_t)atoi(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (uint32_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (uint32_t)tatoll(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (uint64_t& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (uint64_t)tatoll(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (float& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (float)atof(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (double& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val = (double)atof(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::operator >> (std::string& val)
	{
		if (current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_])
			{
				val.assign(result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::get_string (char* buff, size_t buff_len)
	{
		if (NULL != buff && current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_] && buff_len > strlen(result_[current_index_]))
			{
				strcpy(buff, result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

	SQLite_Result& SQLite_Result::get_string (unsigned char* buff, size_t buff_len)
	{
		if (NULL != buff && current_row_ <= row_count_ && current_col_ < col_count_)
		{
			size_t current_index_ = (current_row_ * col_count_) + current_col_;
			if (NULL != result_[current_index_] && buff_len > strlen(result_[current_index_]))
			{
				strcpy((char*)buff, result_[current_index_]);
			}

			++current_col_;
		}

		return *this;
	}

}


