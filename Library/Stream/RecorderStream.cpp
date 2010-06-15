#include <string.h>
#include "RecorderStream.h"

namespace vl
{
	namespace stream
	{
/***********************************************************************
RecorderStream
***********************************************************************/

		RecorderStream::RecorderStream(IStream& _in, IStream& _out)
			:in(&_in)
			,out(&_out)
		{
		}

		RecorderStream::~RecorderStream()
		{
		}

		bool RecorderStream::CanRead()const
		{
			return IsAvailable() && in->CanRead();
		}

		bool RecorderStream::CanWrite()const
		{
			return false;
		}

		bool RecorderStream::CanSeek()const
		{
			return false;
		}

		bool RecorderStream::CanPeek()const
		{
			return false;
		}

		bool RecorderStream::IsLimited()const
		{
			return IsAvailable() && in->IsLimited();
		}

		bool RecorderStream::IsAvailable()const
		{
			return in!=0 && out!=0 && in->IsAvailable() && out->IsAvailable();
		}

		void RecorderStream::Close()
		{
			in=0;
			out=0;
		}

		pos_t RecorderStream::Position()const
		{
			return IsAvailable()?in->Position():-1;
		}

		pos_t RecorderStream::Size()const
		{
			return IsAvailable()?in->Size():-1;
		}

		void RecorderStream::Seek(pos_t _size)
		{
			CHECK_ERROR(false, L"RecorderStream::Seek(pos_t)#不支持此操作。");
		}

		void RecorderStream::SeekFromBegin(pos_t _size)
		{
			CHECK_ERROR(false, L"RecorderStream::SeekFromBegin(pos_t)#不支持此操作。");
		}

		void RecorderStream::SeekFromEnd(pos_t _size)
		{
			CHECK_ERROR(false, L"RecorderStream::SeekFromEnd(pos_t)#不支持此操作。");
		}

		vint RecorderStream::Read(void* _buffer, vint _size)
		{
			_size=in->Read(_buffer, _size);
			out->Write(_buffer, _size);
			return _size;
		}

		vint RecorderStream::Write(void* _buffer, vint _size)
		{
			CHECK_ERROR(false, L"RecorderStream::Write(void*, vint)#不支持此操作。");
		}

		vint RecorderStream::Peek(void* _buffer, vint _size)
		{
			CHECK_ERROR(false, L"RecorderStream::Peek(void*, vint)#不支持此操作。");
		}
	}
}