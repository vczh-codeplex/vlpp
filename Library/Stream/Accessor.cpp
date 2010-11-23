#include <string.h>
#include "Accessor.h"

namespace vl
{
	namespace stream
	{

/***********************************************************************
TextReader
***********************************************************************/

		WString TextReader::ReadString(vint length)
		{
			wchar_t* buffer=new wchar_t[length+1];
			vint i=0;
			for(;i<length;i++)
			{
				if((buffer[i]=ReadChar())==L'\0')
				{
					break;
				}
			}
			buffer[i]=L'\0';
			WString result(buffer);
			delete[] buffer;
			return result;
		}

		WString TextReader::ReadLine()
		{
			WString result;
			wchar_t buffer[65537];
			buffer[0]=L'\0';
			vint i=0;
			while(true)
			{
				wchar_t c=ReadChar();
				if(c==L'\n' || c==L'\0')
				{
					buffer[i]=L'\0';
					result+=buffer;
					buffer[0]=L'\0';
					i=0;
					break;
				}
				else
				{
					if(i==65536)
					{
						buffer[i]=L'\0';
						result+=buffer;
						buffer[0]=L'\0';
						i=0;
					}
					buffer[i++]=c;
				}
			}
			result+=buffer;
			if(result.Length()>0 && result[result.Length()-1]==L'\r')
			{
				return result.Left(result.Length()-1);
			}
			else
			{
				return result;
			}
		}

		WString TextReader::ReadToEnd()
		{
			WString result;
			wchar_t buffer[65537];
			buffer[0]=L'\0';
			vint i=0;
			while(true)
			{
				wchar_t c=ReadChar();
				if(c==L'\0')
				{
					buffer[i]=L'\0';
					result+=buffer;
					buffer[0]=L'\0';
					i=0;
					break;
				}
				else
				{
					if(i==65536)
					{
						buffer[i]=L'\0';
						result+=buffer;
						buffer[0]=L'\0';
						i=0;
					}
					buffer[i++]=c;
				}
			}
			result+=buffer;
			return result;
		}

/***********************************************************************
TextWriter
***********************************************************************/

		void TextWriter::WriteString(const wchar_t* string)
		{
			while(*string)
			{
				WriteChar(*string++);
			}
		}

		void TextWriter::WriteString(const WString& string)
		{
			if(string.Length())
			{
				WriteString(string.Buffer());
			}
		}

		void TextWriter::WriteLine(const wchar_t* string)
		{
			WriteString(string);
			WriteString(L"\r\n");
		}

		void TextWriter::WriteLine(const WString& string)
		{
			WriteString(string);
			WriteString(L"\r\n");
		}

/***********************************************************************
StreamReader
***********************************************************************/

		StreamReader::StreamReader(IStream& _stream)
			:stream(&_stream)
		{
		}

		bool StreamReader::IsEnd()
		{
			return stream==0;
		}

		wchar_t StreamReader::ReadChar()
		{
			if(stream)
			{
				wchar_t buffer=0;
				if(stream->Read(&buffer, sizeof(buffer))==0)
				{
					stream=0;
					return 0;
				}
				else
				{
					return buffer;
				}
			}
			else
			{
				return L'\0';
			}
		}

/***********************************************************************
StreamWriter
***********************************************************************/

		StreamWriter::StreamWriter(IStream& _stream)
			:stream(&_stream)
		{
		}

		void StreamWriter::WriteChar(wchar_t c)
		{
			stream->Write(&c, sizeof(c));
		}

		void StreamWriter::WriteString(const wchar_t* string)
		{
			stream->Write((void*)string, wcslen(string)*sizeof(*string));
		}

		void StreamWriter::WriteString(const WString& string)
		{
			stream->Write((void*)string.Buffer(), string.Length()*sizeof(wchar_t));
		}

/***********************************************************************
EncoderStream
***********************************************************************/

		EncoderStream::EncoderStream(IStream& _stream, IEncoder& _encoder)
			:stream(&_stream)
			,encoder(&_encoder)
			,position(0)
		{
			encoder->Setup(stream);
		}

		EncoderStream::~EncoderStream()
		{
			Close();
		}

		bool EncoderStream::CanRead()const
		{
			return false;
		}

		bool EncoderStream::CanWrite()const
		{
			return IsAvailable();
		}

		bool EncoderStream::CanSeek()const
		{
			return false;
		}

		bool EncoderStream::CanPeek()const
		{
			return false;
		}

		bool EncoderStream::IsLimited()const
		{
			return stream!=0 && stream->IsLimited();
		}

		bool EncoderStream::IsAvailable()const
		{
			return stream!=0 && stream->IsAvailable();
		}

		void EncoderStream::Close()
		{
			encoder->Close();
			stream=0;
		}

		pos_t EncoderStream::Position()const
		{
			return IsAvailable()?position:-1;
		}

		pos_t EncoderStream::Size()const
		{
			return -1;
		}

		void EncoderStream::Seek(pos_t _size)
		{
			CHECK_FAIL(L"EncoderStream::Seek(pos_t)#��֧�ִ˲�����");
		}

		void EncoderStream::SeekFromBegin(pos_t _size)
		{
			CHECK_FAIL(L"EncoderStream::SeekFromBegin(pos_t)#��֧�ִ˲�����");
		}

		void EncoderStream::SeekFromEnd(pos_t _size)
		{
			CHECK_FAIL(L"EncoderStream::SeekFromEnd(pos_t)#��֧�ִ˲�����");
		}

		vint EncoderStream::Read(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"EncoderStream::Read(void*, vint)#��֧�ִ˲�����");
		}

		vint EncoderStream::Write(void* _buffer, vint _size)
		{
			vint result=encoder->Write(_buffer, _size);
			if(result>=0)
			{
				position+=result;
			}
			return result;
		}

		vint EncoderStream::Peek(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"EncoderStream::Peek(void*, vint)#��֧�ִ˲�����");
		}

/***********************************************************************
DecoderStream
***********************************************************************/

		DecoderStream::DecoderStream(IStream& _stream, IDecoder& _decoder)
			:stream(&_stream)
			,decoder(&_decoder)
			,position(0)
		{
			decoder->Setup(stream);
		}

		DecoderStream::~DecoderStream()
		{
			Close();
		}

		bool DecoderStream::CanRead()const
		{
			return IsAvailable();
		}

		bool DecoderStream::CanWrite()const
		{
			return false;
		}

		bool DecoderStream::CanSeek()const
		{
			return false;
		}

		bool DecoderStream::CanPeek()const
		{
			return false;
		}

		bool DecoderStream::IsLimited()const
		{
			return stream!=0 && stream->IsLimited();
		}

		bool DecoderStream::IsAvailable()const
		{
			return stream!=0 && stream->IsAvailable();
		}

		void DecoderStream::Close()
		{
			decoder->Close();
			stream=0;
		}

		pos_t DecoderStream::Position()const
		{
			return IsAvailable()?position:-1;
		}

		pos_t DecoderStream::Size()const
		{
			return -1;
		}

		void DecoderStream::Seek(pos_t _size)
		{
			CHECK_FAIL(L"DecoderStream::Seek(pos_t)#��֧�ִ˲�����");
		}

		void DecoderStream::SeekFromBegin(pos_t _size)
		{
			CHECK_FAIL(L"DecoderStream::SeekFromBegin(pos_t)#��֧�ִ˲�����");
		}

		void DecoderStream::SeekFromEnd(pos_t _size)
		{
			CHECK_FAIL(L"DecoderStream::SeekFromEnd(pos_t)#��֧�ִ˲�����");
		}

		vint DecoderStream::Read(void* _buffer, vint _size)
		{
			vint result=decoder->Read(_buffer, _size);
			if(result>=0)
			{
				position+=result;
			}
			return result;
		}

		vint DecoderStream::Write(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"DecoderStream::Write(void*, vint)#��֧�ִ˲�����");
		}

		vint DecoderStream::Peek(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"DecoderStream::Peek(void*, vint)#��֧�ִ˲�����");
		}
	}
}