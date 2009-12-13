/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Stream::MemoryStream

Interfaces:
	MemoryStream					£ºÄÚ´æÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_MEMORYSTREAM
#define VCZH_STREAM_MEMORYSTREAM

#include "Interfaces.h"

namespace vl
{
	namespace stream
	{
		class MemoryStream : public Object, public virtual IStream
		{
		protected:
			int						block;
			char*					buffer;
			int						size;
			int						position;
			int						capacity;

			void					PrepareSpace(int totalSpace);
		public:
			MemoryStream(int _block=65536);
			~MemoryStream();

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