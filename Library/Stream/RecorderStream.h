/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Stream::RecorderStream

Interfaces:
	RecorderStream					£º±¸·ÝÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_RECORDERSTREAM
#define VCZH_STREAM_RECORDERSTREAM

#include "Interfaces.h"

namespace vl
{
	namespace stream
	{
		class RecorderStream : public Object, public virtual IStream
		{
		protected:
			IStream*				in;
			IStream*				out;
		public:
			RecorderStream(IStream& _in, IStream& _out);
			~RecorderStream();

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