#include <string.h>
#include "BroadcastStream.h"

namespace vl
{
	namespace stream
	{
/***********************************************************************
BroadcastStream
***********************************************************************/

		BroadcastStream::BroadcastStream()
			:closed(false)
			,position(0)
		{
		}

		BroadcastStream::~BroadcastStream()
		{
		}

		BroadcastStream::_ListInterface& BroadcastStream::Targets()
		{
			return streams.Wrap();
		}

		bool BroadcastStream::CanRead()const
		{
			return false;
		}

		bool BroadcastStream::CanWrite()const
		{
			return !closed;
		}

		bool BroadcastStream::CanSeek()const
		{
			return false;
		}

		bool BroadcastStream::CanPeek()const
		{
			return false;
		}

		bool BroadcastStream::IsLimited()const
		{
			return false;
		}

		bool BroadcastStream::IsAvailable()const
		{
			return !closed;
		}

		void BroadcastStream::Close()
		{
			closed=true;
			position=-1;
		}

		pos_t BroadcastStream::Position()const
		{
			return position;
		}

		pos_t BroadcastStream::Size()const
		{
			return position;
		}

		void BroadcastStream::Seek(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::Seek(pos_t)#不支持此操作。");
		}

		void BroadcastStream::SeekFromBegin(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::SeekFromBegin(pos_t)#不支持此操作。");
		}

		void BroadcastStream::SeekFromEnd(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::SeekFromEnd(pos_t)#不支持此操作。");
		}

		vint BroadcastStream::Read(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"BroadcastStream::Read(void*, vint)#不支持此操作。");
		}

		vint BroadcastStream::Write(void* _buffer, vint _size)
		{
			for(vint i=0;i<streams.Count();i++)
			{
				streams[i]->Write(_buffer, _size);
			}
			position+=_size;
			return _size;
		}

		vint BroadcastStream::Peek(void* _buffer, vint _size)
		{
			CHECK_FAIL(L"BroadcastStream::Peek(void*, vint)#不支持此操作。");
		}
	}
}