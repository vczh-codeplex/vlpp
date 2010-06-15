/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Stream::CharFormat

Classes:
	CharEncoder									：字符串编码器基类
	CharDecoder									：字符串解码器基类
	MbcsEncoder									：Mbcs编码器
	MbcsDecoder									：Mbcs解码器
	Utf16Encoder								：Utf16编码器
	Utf16Decoder								：Utf16解码器
	Utf16BEEncoder								：Utf16 Big Endian编码器
	Utf16BEDecoder								：Utf16 Big Endian解码器
	Utf8Encoder									：Utf8编码器
	Utf8Decoder									：Utf8解码器
	BomEncoder									：BOM相关编码器
	BomDecoder									：BOM相关解码器
***********************************************************************/

#ifndef VCZH_STREAM_CHARFORMAT
#define VCZH_STREAM_CHARFORMAT

#include "Interfaces.h"

namespace vl
{
	namespace stream
	{

		/*编码资料
		UCS-4和UTF-8的对应关系:
		U-00000000 - U-0000007F:  0xxxxxxx
		U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
		U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
		U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		BOM:
		FFFE	=Unicode			(vceUtf16)
		FEFF	=Unicode Big Endian	(vceUtf16_be)
		EFBBBF	=UTF-8				(vceUtf8)
		other	=MBCS(GBK)			(vceMbcs)
		*/

/***********************************************************************
字符串编码解码基类
***********************************************************************/

		class CharEncoder : public Object, public IEncoder
		{
		protected:
			IStream*						stream;
			unsigned char					cache;
			bool							cacheAvailable;

			virtual vint						WriteString(wchar_t* _buffer, vint chars)=0;
		public:
			CharEncoder();

			void							Setup(IStream* _stream);
			void							Close();
			vint								Write(void* _buffer, vint _size);
		};

		class CharDecoder : public Object, public IDecoder
		{
		protected:
			IStream*						stream;
			unsigned char					cache;
			bool							cacheAvailable;

			virtual vint						ReadString(wchar_t* _buffer, vint chars)=0;
		public:
			CharDecoder();

			void							Setup(IStream* _stream);
			void							Close();
			vint								Read(void* _buffer, vint _size);
		};

/***********************************************************************
Mbcs
***********************************************************************/

		class MbcsEncoder : public CharEncoder
		{
		protected:
			vint								WriteString(wchar_t* _buffer, vint chars);
		};

		class MbcsDecoder : public CharDecoder
		{
		protected:
			vint								ReadString(wchar_t* _buffer, vint chars);
		};

/***********************************************************************
Utf-16
***********************************************************************/

		class Utf16Encoder : public CharEncoder
		{
		protected:
			vint								WriteString(wchar_t* _buffer, vint chars);
		};

		class Utf16Decoder : public CharDecoder
		{
		protected:
			vint								ReadString(wchar_t* _buffer, vint chars);
		};

/***********************************************************************
Utf-16-be
***********************************************************************/

		class Utf16BEEncoder : public CharEncoder
		{
		protected:
			vint								WriteString(wchar_t* _buffer, vint chars);
		};

		class Utf16BEDecoder : public CharDecoder
		{
		protected:
			vint								ReadString(wchar_t* _buffer, vint chars);
		};

/***********************************************************************
Utf-8
***********************************************************************/

		class Utf8Encoder : public CharEncoder
		{
		protected:
			vint								WriteString(wchar_t* _buffer, vint chars);
		};

		class Utf8Decoder : public CharDecoder
		{
		protected:
			vint								ReadString(wchar_t* _buffer, vint chars);
		};

/***********************************************************************
Bom
***********************************************************************/

		class BomEncoder : public Object, public IEncoder
		{
		public:
			enum Encoding
			{
				Mbcs,
				Utf8,
				Utf16,
				Utf16BE
			};
		protected:
			Encoding						encoding;
			IEncoder*						encoder;
		public:
			BomEncoder(Encoding _encoding);
			~BomEncoder();

			void							Setup(IStream* _stream);
			void							Close();
			vint								Write(void* _buffer, vint _size);
		};

		class BomDecoder : public Object, public IDecoder
		{
		private:
			class BomStream : public Object, public IStream
			{
			protected:
				IStream*					stream;
				char						bom[3];
				vint							bomLength;
				vint							bomPosition;
			public:
				BomStream(IStream* _stream, char* _bom, vint _bomLength);

				bool						CanRead()const;
				bool						CanWrite()const;
				bool						CanSeek()const;
				bool						CanPeek()const;
				bool						IsLimited()const;
				bool						IsAvailable()const;
				void						Close();
				pos_t						Position()const;
				pos_t						Size()const;
				void						Seek(pos_t _size);
				void						SeekFromBegin(pos_t _size);
				void						SeekFromEnd(pos_t _size);
				vint							Read(void* _buffer, vint _size);
				vint							Write(void* _buffer, vint _size);
				vint							Peek(void* _buffer, vint _size);
			};
		protected:
			IDecoder*						decoder;
			IStream*						stream;

		public:
			BomDecoder();
			~BomDecoder();

			void							Setup(IStream* _stream);
			void							Close();
			vint								Read(void* _buffer, vint _size);
		};
	}
}

#endif