/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Stream::CacheStream

Interfaces:
	CacheStream						£º»º³åÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_CACHESTREAM
#define VCZH_STREAM_CACHESTREAM

#include "Interfaces.h"

namespace vl
{
	namespace stream
	{
		class CacheStream : public Object, public virtual IStream
		{
		protected:
			IStream*				target;
			int						block;
			pos_t					start;
			pos_t					position;

			char*					buffer;
			int						dirtyStart;
			int						dirtyLength;
			int						availableLength;
			pos_t					operatedSize;

			void					Flush();
			void					Load(pos_t _position);
			int						InternalRead(void* _buffer, int _size);
			int						InternalWrite(void* _buffer, int _size);
		public:
			CacheStream(IStream& _target, int _block=65536);
			~CacheStream();

			bool					CanRead()const;
			bool					CanWrite()const;
			bool					CanSeek()const;
			bool					CanPeek()const;
			bool					IsLimited()const;
			bool					IsAvailable()const;
			void					Close();
			pos_t					Position()const;
			pos_t					Size()const;
			void					Seek(pos_t _size);
			void					SeekFromBegin(pos_t _size);
			void					SeekFromEnd(pos_t _size);
			int						Read(void* _buffer, int _size);
			int						Write(void* _buffer, int _size);
			int						Peek(void* _buffer, int _size);
		};
	}
}

#endif