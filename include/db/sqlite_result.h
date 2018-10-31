#ifndef __SQLITE_RESULT_H__
#define __SQLITE_RESULT_H__

#include <string>
#include "common/driver.h"

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

		//��������Ƿ�Ϊ��
		bool is_null (void);
		//���ý������
		void set_result (char** result, size_t row_count, size_t col_count);
		//�ͷ�MYSQL_RES
		void free_result (void);
		//��ȡ��һ������
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


