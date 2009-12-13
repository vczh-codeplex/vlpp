#include <windows.h>
#include "CharFormat.h"

namespace vl
{
	namespace stream
	{

/***********************************************************************
CharEncoder
***********************************************************************/

		CharEncoder::CharEncoder()
			:stream(0)
			,cache(0)
			,cacheAvailable(false)
		{
		}

		void CharEncoder::Setup(IStream* _stream)
		{
			stream=_stream;
		}

		void CharEncoder::Close()
		{
		}

		int CharEncoder::Write(void* _buffer, int _size)
		{
			const int all=(cacheAvailable?1:0)+_size;
			const int chars=all/2;
			const int bytes=chars*2;
			wchar_t* unicode=0;
			bool needToFree=false;
			int result=0;

			if(chars)
			{
				if(cacheAvailable)
				{
					unicode=new wchar_t[chars];
					memcpy(unicode, &cache, sizeof(cache));
					memcpy(((unsigned char*)unicode)+1, _buffer, bytes-sizeof(cache));
					needToFree=true;
				}
				else
				{
					unicode=(wchar_t*)_buffer;
				}
				result=WriteString(unicode, chars)*2-(cacheAvailable?1:0);
				cacheAvailable=false;
			}

			if(needToFree)
			{
				delete[] unicode;
			}
			if(all-bytes>0)
			{
				cache=((unsigned char*)_buffer)[_size-1];
				cacheAvailable=true;
				result++;
			}
			return result;
		}

/***********************************************************************
CharDecoder
***********************************************************************/

		CharDecoder::CharDecoder()
			:stream(0)
			,cache(0)
			,cacheAvailable(false)
		{
		}

		void CharDecoder::Setup(IStream* _stream)
		{
			stream=_stream;
		}

		void CharDecoder::Close()
		{
		}

		int CharDecoder::Read(void* _buffer, int _size)
		{
			unsigned char* unicode=(unsigned char*)_buffer;
			int result=0;
			if(cacheAvailable && _size>0)
			{
				*unicode++=cache;
				cacheAvailable=false;
				result++;
			}

			const int chars=_size/2;
			int bytes=ReadString((wchar_t*)unicode, chars)*2;
			result+=bytes;
			_size-=bytes;
			unicode+=bytes;

			if(_size-result==1)
			{
				wchar_t c;
				if(ReadString(&c, 1)==1)
				{
					unicode[0]=((unsigned char*)&c)[0];
					cache=((unsigned char*)&c)[1];
					cacheAvailable=true;
					result++;
				}
			}
			return result;
		}

/***********************************************************************
Mbcs
***********************************************************************/

		int MbcsEncoder::WriteString(wchar_t* _buffer, int chars)
		{
			int length=WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, chars, mbcs, length, NULL, NULL);
			int result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		int MbcsDecoder::ReadString(wchar_t* _buffer, int chars)
		{
			char* source=new char[chars*2];
			char* reading=source;
			int readed=0;
			while(readed<chars)
			{
				if(stream->Read(reading, 1)!=1)
				{
					break;
				}
				if(IsDBCSLeadByte(*reading))
				{
					if(stream->Read(reading+1, 1)!=1)
					{
						break;
					}
					reading+=2;
				}
				else
				{
					reading++;
				}
				readed++;
			}
			MultiByteToWideChar(CP_THREAD_ACP, 0, source, reading-source, _buffer, chars);
			delete[] source;
			return readed;
		}

/***********************************************************************
Utf-16-be
***********************************************************************/

		int Utf16Encoder::WriteString(wchar_t* _buffer, int chars)
		{
			return stream->Write(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

		int Utf16Decoder::ReadString(wchar_t* _buffer, int chars)
		{
			return stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

/***********************************************************************
Utf-16-be
***********************************************************************/

		int Utf16BEEncoder::WriteString(wchar_t* _buffer, int chars)
		{
			int writed=0;
			while(writed<chars)
			{
				if(stream->Write(((unsigned char*)_buffer)+1, 1)!=1)
				{
					break;
				}
				if(stream->Write(_buffer, 1)!=1)
				{
					break;
				}
				_buffer++;
				writed++;
			}
			if(writed!=chars)
			{
				Close();
			}
			return writed;
		}

		int Utf16BEDecoder::ReadString(wchar_t* _buffer, int chars)
		{
			chars=stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
			unsigned char* unicode=(unsigned char*)_buffer;
			for(int i=0;i<chars;i++)
			{
				unsigned char t=unicode[0];
				unicode[0]=unicode[1];
				unicode[1]=t;
				unicode++;
			}
			return chars;
		}

/***********************************************************************
Utf8
***********************************************************************/

		int Utf8Encoder::WriteString(wchar_t* _buffer, int chars)
		{
			int length=WideCharToMultiByte(CP_UTF8, 0, _buffer, chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_UTF8, 0, _buffer, chars, mbcs, length, NULL, NULL);
			int result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		int Utf8Decoder::ReadString(wchar_t* _buffer, int chars)
		{
			char* source=new char[chars*3];
			char* reading=source;
			int readed=0;
			while(readed<chars)
			{
				if(stream->Read(reading, 1)!=1)
				{
					break;
				}
				if((*reading & 0xE0) == 0xE0)
				{
					if(stream->Read(reading+1, 2)!=2)
					{
						break;
					}
					reading+=3;
				}
				else if((*reading & 0xC0) == 0xC0)
				{
					if(stream->Read(reading+1, 1)!=1)
					{
						break;
					}
					reading+=2;
				}
				else
				{
					reading++;
				}
				readed++;
			}
			MultiByteToWideChar(CP_UTF8, 0, source, reading-source, _buffer, chars);
			delete[] source;
			return readed;
		}

/***********************************************************************
BomEncoder
***********************************************************************/

		BomEncoder::BomEncoder(Encoding _encoding)
			:encoding(_encoding)
			,encoder(0)
		{
			switch(encoding)
			{
			case Mbcs:
				encoder=new MbcsEncoder;
				break;
			case Utf8:
				encoder=new Utf8Encoder;
				break;
			case Utf16:
				encoder=new Utf16Encoder;
				break;
			case Utf16BE:
				encoder=new Utf16BEEncoder;
				break;
			}
		}

		BomEncoder::~BomEncoder()
		{
			Close();
		}

		void BomEncoder::Setup(IStream* _stream)
		{
			switch(encoding)
			{
			case Mbcs:
				break;
			case Utf8:
				_stream->Write("\xEF\xBB\xBF", 3);
				break;
			case Utf16:
				_stream->Write("\xFF\xFE", 2);
				break;
			case Utf16BE:
				_stream->Write("\xFE\xFF", 2);
				break;
			}
			encoder->Setup(_stream);
		}

		void BomEncoder::Close()
		{
			if(encoder)
			{
				encoder->Close();
				delete encoder;
				encoder=0;
			}
		}

		int BomEncoder::Write(void* _buffer, int _size)
		{
			return encoder->Write(_buffer, _size);
		}

/***********************************************************************
BomDecoder
***********************************************************************/

		BomDecoder::BomStream::BomStream(IStream* _stream, char* _bom, int _bomLength)
			:stream(_stream)
			,bomPosition(0)
			,bomLength(_bomLength)
		{
			memcpy(bom, _bom, bomLength);
		}

		bool BomDecoder::BomStream::CanRead()const
		{
			return IsAvailable();
		}

		bool BomDecoder::BomStream::CanWrite()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanSeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanPeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::IsLimited()const
		{
			return stream!=0 && stream->IsLimited();
		}

		bool BomDecoder::BomStream::IsAvailable()const
		{
			return stream!=0 && stream->IsAvailable();
		}

		void BomDecoder::BomStream::Close()
		{
			stream=0;
		}

		pos_t BomDecoder::BomStream::Position()const
		{
			return IsAvailable()?bomPosition+stream->Position():-1;
		}

		pos_t BomDecoder::BomStream::Size()const
		{
			return -1;
		}

		void BomDecoder::BomStream::Seek(pos_t _size)
		{
			CHECK_ERROR(false, L"BomDecoder::BomStream::Seek(pos_t)#不支持此操作。");
		}

		void BomDecoder::BomStream::SeekFromBegin(pos_t _size)
		{
			CHECK_ERROR(false, L"BomDecoder::BomStream::SeekFromBegin(pos_t)#不支持此操作。");
		}

		void BomDecoder::BomStream::SeekFromEnd(pos_t _size)
		{
			CHECK_ERROR(false, L"BomDecoder::BomStream::SeekFromEnd(pos_t)#不支持此操作。");
		}

		int BomDecoder::BomStream::Read(void* _buffer, int _size)
		{
			int result=0;
			unsigned char* buffer=(unsigned char*)_buffer;
			if(bomPosition<bomLength)
			{
				int remain=bomLength-bomPosition;
				result=remain<_size?remain:_size;
				memcpy(buffer, bom+bomPosition, result);
				buffer+=result;
				bomPosition+=result;
				_size-=result;
			}
			if(_size)
			{
				result+=stream->Read(buffer, _size);
			}
			return result;
		}

		int BomDecoder::BomStream::Write(void* _buffer, int _size)
		{
			CHECK_ERROR(false, L"BomDecoder::BomStream::Write(void*, int)#不支持此操作。");
		}

		int BomDecoder::BomStream::Peek(void* _buffer, int _size)
		{
			CHECK_ERROR(false, L"BomDecoder::BomStream::Peek(void*, int)#不支持此操作。");
		}

		BomDecoder::BomDecoder()
			:decoder(0)
		{
		}

		BomDecoder::~BomDecoder()
		{
			Close();
		}

		void BomDecoder::Setup(IStream* _stream)
		{
			char bom[3]={0};
			int length=_stream->Read(bom, sizeof(bom));
			if(strncmp(bom, "\xEF\xBB\xBF", 3)==0)
			{
				decoder=new Utf8Decoder;
				stream=new BomStream(_stream, bom+3, 0);
			}
			else if(strncmp(bom, "\xFF\xFE", 2)==0)
			{
				decoder=new Utf16Decoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else if(strncmp(bom, "\xFE\xFF", 2)==0)
			{
				decoder=new Utf16BEDecoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else
			{
				decoder=new MbcsDecoder;
				stream=new BomStream(_stream, bom, 3);
			}
			decoder->Setup(stream);
		}

		void BomDecoder::Close()
		{
			if(decoder)
			{
				decoder->Close();
				delete decoder;
				decoder=0;
				stream->Close();
				delete stream;
				stream=0;
			}
		}

		int BomDecoder::Read(void* _buffer, int _size)
		{
			return decoder->Read(_buffer, _size);
		}
	}
}