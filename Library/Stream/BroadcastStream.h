/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Stream::BroadcastStream

Interfaces:
	BroadcastStream					���㲥��
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
			int						Read(void* _buffer, int _size);
			int						Write(void* _buffer, int _size);
			int						Peek(void* _buffer, int _size);
		};
	}
}

#endif