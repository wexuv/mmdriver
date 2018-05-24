#include "outputstream.h"
#include <stdarg.h>

namespace driver 
{
	void OutputStream::Print(const char* str, ...)
	{
		va_list arglist;
		va_start(arglist, str);
		PrintArg(str, arglist);
		va_end(arglist);
	}

	bool OutputStreamDebugWindow::Open( const char* fileName,const char* _Mode)
	{
		(void)fileName;
		return true;
	}

	void OutputStreamDebugWindow::PrintArg(const char* str, va_list argp)
	{
		char message[800];
		tvsnprintf(message , 800 , str, argp);

#if defined(__WINDOWS__)
		OutputDebugString(message);
		printf(message);
#elif defined(__LINUX__)
		printf(message);
#else
#error windows or linux is required.
#endif
	}

	bool OutputStreamFile::Open( const char* fileName, const char* _Mode )
	{
		Close();
		m_file = fopen( fileName, _Mode );

		return m_file != NULL;
	}

	void OutputStreamFile::Close()
	{
		if ( m_file )
		{
			fclose( m_file );
			m_file = NULL;
		}
	}

	void OutputStreamFile::PrintArg( const char* str, va_list argp)
	{
		char message[800];
		tvsnprintf(message , 800 , str, argp);
		fputs( message , m_file );
	}

	bool OutputStreamFile::IsOpen()
	{
		return m_file != NULL;
	}

	void OutputStreamFile::Flush()
	{
		fflush(m_file);
	}

	bool OutputStreamBuffer::Open( const char* fileName,const char* _Mode)
	{
		(void)fileName;
		return true;
	}

	void OutputStreamBuffer::PrintArg(const char* str, va_list argp)
	{
		char message[800];
		tvsnprintf(message , 800 , str, argp);
		m_stream<<message;
	}

	tstring OutputStreamBuffer::GetBuffer()
	{
		return m_stream.str().c_str();
	}
}