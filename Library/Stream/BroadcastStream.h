/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Stream::BroadcastStream

Interfaces:
	BroadcastStream					£º¹ã²¥Á÷
***********************************************************************/

#ifndef VCZH_STREAM_BROADCASTSTREAM
#define VCZH_STREAM_BROADCASTSTREAM

#include "Interfaces.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace stream
	{
		class BroadcastStream : public Object, public virtual IStream
		{
			typedef collections::IList<IStream*>	_ListInterface;
			typedef collections::List<IStream*>		_List;
		protected:
			bool					closed;
			pos_t					position;
			_List					streams;
		public:
			BroadcastStream();
			~BroadcastStream();

			_ListInterface&			Targets();
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
			vint						Read(void* _buffer, vint _size);
			vint						Write(void* _buffer, vint _size);
			vint						Peek(void* _buffer, vint _size);
		};
	}
}

#endif