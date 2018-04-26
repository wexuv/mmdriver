#ifndef _OUTPUT_STREAM_H_
#define _OUTPUT_STREAM_H_

#include "exception.h"
#include "types.h"

namespace driver 
{
	/**
	�ļ�������
	��������ʵ�ֲ�ͬ��Print�����������ӡ�����Դ��ڡ���ӡ���ļ�����ӡ���ַ���Buffer
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
	��ӡ�����Դ���
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
	д���ļ�
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
	д���ַ���Buffer
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