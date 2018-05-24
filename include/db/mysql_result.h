#ifndef _MYSQL_DB_RESULT_H_
#define _MYSQL_DB_RESULT_H_

#include <mysql.h>
#include "driver.h"
#include <string>

namespace driver
{
	class Mysql_Result
	{
	private:

		//�������
		MYSQL_RES* mysql_result_;
		//������ϵĵ�ǰ��
		MYSQL_ROW current_row_;
		//������ϵĵ�ǰ��
		u_int current_field_;
		//������ڵ�ǰ�е��еĳ���
		unsigned long* fields_length_;
		//������ϵ�����
		u_int row_count_;
		//������ϵ�����
		u_int field_count_;

	public:

		Mysql_Result (void);
		~Mysql_Result (void);

		//��������Ƿ�Ϊ��
		bool is_null (void);
		//���ý������
		void set_result (MYSQL_RES* result);
		//�ͷ�MYSQL_RES
		void free_result (void);
		//��ȡ��һ������
		bool fetch_next_row (void);


		//���� >> ��������,���ڽ�������
		//ע�⣺ֻ�е�fetch_next_row()����true�Ժ����ִ�У�
		//�����ڳ������Լ������ж�������������
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




