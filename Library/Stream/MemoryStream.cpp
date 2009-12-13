#include <string.h>
#include "MemoryStream.h"

namespace vl
{
	namespace stream
	{
/***********************************************************************
MemoryStream
***********************************************************************/

		void MemoryStream::PrepareSpace(int totalSpace)
		{
			if(totalSpace>capacity)
			{
				totalSpace=(totalSpace/block+1)*block;
				char* newBuffer=new char[totalSpace];
				if(buffer)
				{
					memcpy(newBuffer, buffer, size);
					delete[] buffer;
				}
				buffer=newBuffer;
				capacity=totalSpace;
			}
		}

		MemoryStream::MemoryStream(int _block)
			:block(_block)
			,buffer(0)
			,size(0)
			,position(0)
			,capacity(0)
		{
			if(block<=0)
			{
				block=65536;
			}
		}

		MemoryStream::~MemoryStream()
		{
			Close();
		}

		bool MemoryStream::CanRead()const
		{
			return block!=0;
		}

		bool MemoryStream::CanWrite()const
		{
			return block!=0;
		}

		bool MemoryStream::CanSeek()const
		{
			return block!=0;
		}

		bool MemoryStream::CanPeek()const
		{
			return block!=0;
		}

		bool MemoryStream::IsLimited()const
		{
			return false;
		}

		bool MemoryStream::IsAvailable()const
		{
			return block!=0;
		}

		void MemoryStream::Close()
		{
			if(buffer)
			{
				delete[] buffer;
			}
			block=0;
			buffer=0;
			size=-1;
			position=-1;
			capacity=0;
		}

		pos_t MemoryStream::Position()const
		{
			return position;
		}

		pos_t MemoryStream::Size()const
		{
			return size;
		}

		void MemoryStream::Seek(pos_t _size)
		{
			SeekFromBegin(position+_size);
		}

		void MemoryStream::SeekFromBegin(pos_t _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::SeekFromBegin(pos_t)#流处于关闭状态，不可执行此操作。");
			int expected=(int)_size;
			if(expected<0)
			{
				position=0;
			}
			else if(expected>=size)
			{
				position=size;
			}
			else
			{
				position=expected;
			}
		}

		void MemoryStream::SeekFromEnd(pos_t _size)
		{
			SeekFromBegin(size-_size);
		}

		int MemoryStream::Read(void* _buffer, int _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Read(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"MemoryStream::Read(void*, int)#参数size不可为负。");
			int max=size-position;
			if(_size>max)
			{
				_size=max;
			}
			memmove(_buffer, buffer+position, _size);
			position+=_size;
			return _size;
		}

		int MemoryStream::Write(void* _buffer, int _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Write(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"MemoryStream::Write(void*, int)#参数size不可为负。");
			PrepareSpace(size+_size);
			memmove(buffer+position, _buffer, _size);
			position+=_size;
			if(size<position)
			{
				size=position;
			}
			return _size;
		}

		int MemoryStream::Peek(void* _buffer, int _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Peek(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"MemoryStream::Peek(void*, int)#参数size不可为负。");
			int max=size-position;
			if(_size>max)
			{
				_size=max;
			}
			memmove(_buffer, buffer+position, _size);
			return _size;
		}
	}
}