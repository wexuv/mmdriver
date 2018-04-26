#ifndef _LOG_H_
#define _LOG_H_

#include "exception.h"
#include "types.h"
#include "outputstream.h"

namespace driver 
{
	enum
	{
		log_mask_none				= tint32(0x00000000),				//
		log_mask_info				= tint32(0x00000001),				// ��ʾ��Ϣ
		log_mask_error				= tint32(0x00000002),				// ������Ϣ
		log_mask_warning			= tint32(0x00000004),				// ������Ϣ
		log_mask_detail				= tint32(0x00000008),				// ��ϸ��Ϣ
		log_mask_package			= tint32(0x00000010),				// ���������ݰ�
	};

	enum 
	{
		max_log_level_title_len = 16,
		max_log_file_size		= 0x40000000,		// �����ļ����1G
		max_log_file_count		= 8,				// һ�����8���ļ�
	};

	struct stLogTitleEntry 
	{
		tint32 m_iLogLevel;
		tchar m_szTitle[max_log_level_title_len+1];
	};

	class Log_Engine
	{
	public:
		enum
		{
			max_dump_info_size = (tuint32)0x400,
			max_dump_line_size = (tuint32)0x10,
		};

		Log_Engine (void);
		~Log_Engine (void);

		bool init (tint32 log_mask, const tchar* file_prefix);

		inline void set_log_mask (tint32 log_mask)
		{
			m_log_mask = log_mask;
		}

		bool log (tint32 log_level, const tchar* content, ...);

		bool dump_bin (tint32 log_level, const tchar* buff, tuint32 size);

	private:
		inline bool whether_do_log (tint32 log_level)
		{
			return((m_log_mask & log_level) != 0);
		}

		bool check_file (void);

		tchar* get_log_level_title (tint32 log_level);

	private:
		OutputStreamFile	m_file;
		//FILE*	m_file;			// �͵�ǰ��־�ļ����ƶ�Ӧ���ļ�������
		tchar	m_file_name[MAX_FILE_NAME_LENGTH+1];	// ��ǰ�ļ���������·����
		tchar	m_file_prefix[MAX_FILE_NAME_LENGTH+1];	// �ļ���ǰ׺���������ļ�������ǰ׺+����+���кţ�
		tint32	m_log_mask;		// ��־����
		tint8	m_file_count;	// ��־�ļ�������
	};
}

#endif //_LOG_H_