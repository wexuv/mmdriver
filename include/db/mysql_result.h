#ifndef _MYSQL_DB_RESULT_H_
#define _MYSQL_DB_RESULT_H_

#include <mysql.h>
#include "common/driver.h"
#include <string>

namespace driver
{
	class Mysql_Result
	{
	private:

		//结果集合
		MYSQL_RES* mysql_result_;
		//结果集合的当前行
		MYSQL_ROW current_row_;
		//结果集合的当前列
		u_int current_field_;
		//结果集内当前行的列的长度
		unsigned long* fields_length_;
		//结果集合的行数
		u_int row_count_;
		//结果集合的列数
		u_int field_count_;

	public:

		Mysql_Result (void);
		~Mysql_Result (void);

		//结果集合是否为空
		bool is_null (void);
		//设置结果集合
		void set_result (MYSQL_RES* result);
		//释放MYSQL_RES
		void free_result (void);
		//获取下一行数据
		bool fetch_next_row (void);


		//重载 >> 操作符号,用于将结果输出
		//注意：只有的fetch_next_row()返回true以后才能执行；
		//并且在程序里自己负责有多少项可以输出。
		Mysql_Result& operator >> (int8_t& val);
		Mysql_Result& operator >> (int16_t& val);
		Mysql_Result& operator >> (tint32& val);
		Mysql_Result& operator >> (int64_t& val);
	    
		Mysql_Result& operator >> (uint8_t& val);
		Mysql_Result& operator >> (uint16_t& val);
		Mysql_Result& operator >> (uint32_t& val);
		Mysql_Result& operator >> (uint64_t& val);

		Mysql_Result& operator >> (float& val);
		Mysql_Result& operator >> (double& val);

		Mysql_Result& operator >> (std::string& val);

		Mysql_Result& get_string (char* buff, size_t buff_len);
		Mysql_Result& get_string (unsigned char* buff, size_t buff_len);

		Mysql_Result& get_blob (char* buff, size_t& buff_len);
		Mysql_Result& get_blob (unsigned char* buff, size_t& buff_len);

	private:

		Mysql_Result (const Mysql_Result&);
		Mysql_Result& operator= (const Mysql_Result&);
	};

	inline bool Mysql_Result::is_null (void)
	{
		if(NULL == mysql_result_)
		{
			return true;
		}

		return false;
	}
}



#endif //_MYSQL_DB_RESULT_H_




