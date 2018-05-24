#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mysql_result.h"

namespace driver
{
	Mysql_Result::Mysql_Result (void):
		mysql_result_ (NULL),
		current_row_ (NULL),
		current_field_ (0),
		fields_length_ (NULL),
		row_count_ (0),
		field_count_ (0)
	{
		//do nothing
	}

	Mysql_Result::~Mysql_Result (void)
	{
		if (NULL != mysql_result_)			
		{
			mysql_free_result (mysql_result_);
			mysql_result_ = NULL;
		}
	}

	void Mysql_Result::set_result (MYSQL_RES* result)
	{
		if (NULL != mysql_result_)
		{
			//释放原有的结果集
			free_result ();
			mysql_result_ = NULL;
		}

		//重新初始化成员变量
		mysql_result_ = result;

		if (NULL != mysql_result_)
		{
			//得到行数,列数
			row_count_   = static_cast<u_int>(mysql_num_rows (mysql_result_));
			field_count_ = static_cast<u_int>(mysql_num_fields (mysql_result_));
		}

		return;
	}

	void Mysql_Result::free_result (void)
	{
		if (NULL != mysql_result_)			
		{
			mysql_free_result(mysql_result_);
			//重新初始化成员变量
			mysql_result_  = NULL;
			current_row_   = NULL;
			current_field_ = 0;
			fields_length_ = NULL;
			row_count_     = 0;
			field_count_   = 0;
		}

		return;
	}

	bool Mysql_Result::fetch_next_row (void)
	{
		if (NULL == mysql_result_)
		{
			return false;
		}

		//检索结果集的下一行
		current_row_ = mysql_fetch_row (mysql_result_);

		//如果NEXT行为空,结束访问
		if(NULL == current_row_)
		{
			return false;
		}

		//得到结果集内当前行的列的长度
		fields_length_ = mysql_fetch_lengths (mysql_result_);
		current_field_ = 0;
		return true;
	}

	Mysql_Result& Mysql_Result::operator >> (int8_t& val)
	{
		if (NULL != current_row_[current_field_])
		{
			val = (int8_t)atoi(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;
		
		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (int16_t& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (int16_t)atoi(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (tint32& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (tint32)atoi(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}


	Mysql_Result& Mysql_Result::operator >> (int64_t& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (int64_t)tatoll(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (uint8_t& val)
	{
		if (NULL != current_row_[current_field_])
		{
			val = (uint8_t)atoi(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (uint16_t& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (uint16_t)atoi(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;
	    
		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (uint32_t& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			sscanf(current_row_[current_field_], "%u", &val);
			//val = (uint32_t)atoll(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;
	    
		return *this;
	}


	Mysql_Result& Mysql_Result::operator >> (uint64_t& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			sscanf(current_row_[current_field_], "%lu", &val);
			//val = (uint64_t)atoll(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (float& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (float)atof(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}
	Mysql_Result& Mysql_Result::operator >> (double& val) 
	{
		if (NULL != current_row_[current_field_])
		{
			val = (double)atof(current_row_[current_field_]);
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::operator >> (std::string& val)
	{
		if (NULL != current_row_[current_field_])
		{
			val.assign(current_row_[current_field_]);
		}
		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::get_string (char* buff, size_t buff_len)
	{
		if ((NULL != buff) && (NULL != current_row_[current_field_]) && (buff_len > fields_length_[current_field_]))
		{
			memcpy(buff, current_row_[current_field_], fields_length_[current_field_]+1);
		}
		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::get_string (unsigned char* buff, size_t buff_len)
	{
		if ((NULL != buff) && (NULL != current_row_[current_field_]) && (buff_len > fields_length_[current_field_]))
		{
			memcpy(buff, current_row_[current_field_], fields_length_[current_field_]+1);
		}
		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::get_blob (char* buff, size_t& buff_len)
	{
		if ((NULL != buff) && (NULL != current_row_[current_field_]) && (buff_len >= fields_length_[current_field_]))
		{
			memcpy(buff, current_row_[current_field_], fields_length_[current_field_]);
			buff_len = fields_length_[current_field_];
		}
		else
		{
			buff_len = 0;
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}

	Mysql_Result& Mysql_Result::get_blob (unsigned char* buff, size_t& buff_len)
	{
		if ((NULL != buff) && (NULL != current_row_[current_field_]) && (buff_len >= fields_length_[current_field_]))
		{
			memcpy(buff, current_row_[current_field_], fields_length_[current_field_]);
			buff_len = fields_length_[current_field_];
		}
		else
		{
			buff_len = 0;
		}

		current_field_ = (current_field_ < field_count_-1) ? (current_field_+1) : current_field_;

		return *this;
	}
}




