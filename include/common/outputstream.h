#ifndef _OUTPUT_STREAM_H_
#define _OUTPUT_STREAM_H_

#include "exception.h"
#include "types.h"

namespace driver 
{
	/**
	文件流基类
	由派生类实现不同的Print方法，比如打印到调试窗口、打印到文件、打印到字符串Buffer
	*/
	class OutputStream
	{
	public:
		OutputStream (void) {};
		~OutputStream (void) {};

		virtual bool Open(const char* fileName,const char* _Mode) = 0;
		virtual void Close() = 0;
		virtual void PrintArg(const char* str, va_list argp) = 0;

	public:
		void Print(const char* str, ...);
	};

	/**
	打印到调试窗口
	*/
	class OutputStreamDebugWindow : public OutputStream
	{
	public:
		OutputStreamDebugWindow() {}
		virtual ~OutputStreamDebugWindow() {}

		virtual bool Open(const char* fileName,const char* _Mode);
		virtual void Close(){}
		virtual void PrintArg(const char* str, va_list argp);
	};
	/**
	写到文件
	*/
	class OutputStreamFile: public OutputStream
	{
	public:
		OutputStreamFile():m_file( NULL ) {}
		virtual ~OutputStreamFile() 
		{
			Close();
		}

		virtual bool Open(const char* fileName,const char* _Mode);
		virtual void Close();
		virtual void PrintArg(const char* str, va_list argp);

	public:
		bool IsOpen();
		void Flush();

	protected:
		FILE* m_file;
	};
	/**
	写到字符串Buffer
	*/
	class OutputStreamBuffer: public OutputStream
	{
	public:
		OutputStreamBuffer(){}
		virtual ~OutputStreamBuffer() {}

		virtual bool Open(const char* fileName,const char* _Mode);
		virtual void Close(){}
		virtual void PrintArg(const char* str, va_list argp);

	public:
		tstring GetBuffer();
	protected:
		tstringstream m_stream;
	};
}

#endif //_OUTPUT_STREAM_H_