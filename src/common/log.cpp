#include "log.h"
#include "time_ex.h"
#include <sys/stat.h>
#include <stdarg.h>

namespace driver 
{
	stLogTitleEntry g_astTitleSet[] = 
	{
		{log_mask_none, "default"},
		{log_mask_info, "info"},
		{log_mask_error, "error"},
		{log_mask_warning, "warning"},
		{log_mask_detail, "detail"},
		{log_mask_package, "package"},
	};

	Log_Engine::Log_Engine (void)
	{
		m_file_name[0] = '\0';
		m_file_prefix[0] = '\0';
		m_log_mask = -1;
		m_file_count = 1;
	}

	Log_Engine::~Log_Engine (void)
	{
		m_file.Close();
	}

	bool Log_Engine::init (int32_t log_mask, const char* file_prefix)
	{
		m_log_mask = log_mask;

		if (NULL == file_prefix)
		{
			tsnprintf(m_file_prefix, MAX_FILE_NAME_LENGTH, "%s", "server_log_name");
		}
		else
		{
			tsnprintf(m_file_prefix, MAX_FILE_NAME_LENGTH, "%s", file_prefix);
		}

		char date[MAX_TIME_STRING_LENGTH+1] = {0};
		get_string_current_date(date,MAX_TIME_STRING_LENGTH);

		tsnprintf(m_file_name, MAX_FILE_NAME_LENGTH, "%s-%s.log", m_file_prefix, date);

		bool re = m_file.Open(m_file_name, "a+");

		return re;
	}

	bool Log_Engine::log (int32_t log_level, const char* content, ...)
	{
		if (whether_do_log(log_level) && check_file())
		{
			char date_time[MAX_TIME_STRING_LENGTH+1] = {0};
			get_string_current_date_time(date_time,MAX_TIME_STRING_LENGTH);

			char* title = get_log_level_title(log_level);
			m_file.Print("[%s][%s]", date_time, title);

			va_list vl;
			va_start(vl, content);
			m_file.PrintArg(content, vl);
			m_file.Flush();
			va_end(vl);

			return true;
		}
		return false;
	}

	bool Log_Engine::dump_bin (int32_t log_level, const char* buff, uint32_t size)
	{
		if (whether_do_log(log_level) && check_file())
		{
			char date_time[MAX_TIME_STRING_LENGTH+1] = {0};
			get_string_current_date_time(date_time,MAX_TIME_STRING_LENGTH);

			char* title = get_log_level_title(log_level);

			m_file.Print("[%s][%s][buffer size:%4u]", date_time, title, size);

			if (size > max_dump_info_size)
			{
				size = max_dump_info_size;
			}

			for(uint32_t i=0; i<size; ++i)
			{
				if(0 == (i % max_dump_line_size))
				{
					m_file.Print("\n[%s] %04u>\t", date_time, (i/max_dump_line_size + 1));
				}
				m_file.Print("%02X ", (unsigned char)buff[i]);
			}

			m_file.Print("\n");

			m_file.Flush();

			return true;
		}

		return false;
	}

	bool Log_Engine::check_file (void)
	{
		// 1.如果文件未打开，则打开它
		if (!m_file.IsOpen())
		{
			if(!m_file.Open(m_file_name, "a+"))
			{
				return false;
			}
		}

		// 2.看日期是否变了
		char date[MAX_TIME_STRING_LENGTH+1] = {0};
		get_string_current_date(date,MAX_TIME_STRING_LENGTH);

		char tmp_file_name[MAX_FILE_NAME_LENGTH+1] = {0};
		tsnprintf(tmp_file_name, MAX_FILE_NAME_LENGTH, "%s-%s.log", m_file_prefix, date);

		if (strcmp(m_file_name, tmp_file_name) != 0)
		{
			m_file.Close();

			STRNCPY(m_file_name, tmp_file_name, sizeof(m_file_name));
			if(!m_file.Open(m_file_name, "a+"))
			{
				return false;
			}

			return true;
		}

		// 3.如果文件个数已经到上限了，则只写当前文件
		if (max_log_file_count == m_file_count)
		{
			return true;
		}

		// 4.单文件大小是否超，如果超了，则改文件名
		struct stat file_stat;
		if(stat(m_file_name, &file_stat) < 0)
		{
			// 文件有可能中途被删除
			m_file.Close();
			if(!m_file.Open(m_file_name, "a+"))
			{
				return false;
			}

			if (stat(m_file_name, &file_stat) < 0)
			{
				return false;
			}
		}

		if(file_stat.st_size < max_log_file_size)
		{
			return true;
		}

		tsnprintf(tmp_file_name, MAX_FILE_NAME_LENGTH, "./log/%s-%s-%d.log", m_file_prefix, date, m_file_count);
		if(0 == access(tmp_file_name, 0))
		{
			if(remove(tmp_file_name) < 0)
			{
				return false;
			}
		}

		m_file.Close();

		rename(m_file_name, tmp_file_name);
		++m_file_count;

		if(!m_file.Open(m_file_name, "a+"))
		{
			return false;
		}

		return true;
	}

	char* Log_Engine::get_log_level_title(int32_t log_level)
	{
		if (log_level <= log_mask_none)
		{
			return g_astTitleSet[0].m_szTitle;
		}

		for (size_t i=0; i<sizeof(g_astTitleSet)/sizeof(g_astTitleSet[i]); ++i)
		{
			if (g_astTitleSet[i].m_iLogLevel == log_level)
			{
				return g_astTitleSet[i].m_szTitle;
			}
		}

		return g_astTitleSet[0].m_szTitle;
	}
}