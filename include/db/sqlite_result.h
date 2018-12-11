#ifndef __SQLITE_RESULT_H__
#define __SQLITE_RESULT_H__

#include <string>
#include <stdint.h>

namespace driver
{
	class SQLite_Result
	{
	private:
		char** result_;
		size_t row_count_;
		size_t col_count_;
		size_t current_row_;
		size_t current_col_;

	public:

		SQLite_Result (void);
		~SQLite_Result (void);

		//结果集合是否为空
		bool is_null (void);
		//设置结果集合
		void set_result (char** result, size_t row_count, size_t col_count);
		//释放MYSQL_RES
		void free_result (void);
		//获取下一行数据
		bool fetch_next_row (void);

		SQLite_Result& operator >> (int8_t& val);
		SQLite_Result& operator >> (int16_t& val);
		SQLite_Result& operator >> (int32_t& val);
		SQLite_Result& operator >> (int64_t& val);

		SQLite_Result& operator >> (uint8_t& val);
		SQLite_Result& operator >> (uint16_t& val);
		SQLite_Result& operator >> (uint32_t& val);
		SQLite_Result& operator >> (uint64_t& val);

		SQLite_Result& operator >> (float& val);
		SQLite_Result& operator >> (double& val);
		SQLite_Result& operator >> (std::string& val);

		SQLite_Result& get_string (char* buff, size_t buff_len);
		SQLite_Result& get_string (unsigned char* buff, size_t buff_len);
	};

	inline bool SQLite_Result::is_null (void)
	{
		if(NULL == result_)
		{
			return true;
		}

		return false;
	}
}

#endif	// __SQLITE_RESULT_H__


