/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Garbage Collector

Classes:
	GcSingleThread							：单线程垃圾收集器
***********************************************************************/

#ifndef VCZH_ENTITY_GCSINGLETHREADENTITY
#define VCZH_ENTITY_GCSINGLETHREADENTITY

#include "GcEntityCommon.h"
#include "GeneralObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
		class GcSingleThread : public Object
		{
		public:
			typedef void(*Callback)(GcSingleThread* gc, GcHandle* handle, void* userData);

		protected:
			struct ObjectHead
			{
				GcMeta*				meta;
				ObjectHead*			prev;
				ObjectHead*			next;
				__int32				repeat;
				__int32				ref;
				__int32				pin;
				bool				mark;
			};

			friend class collections::ReadonlyListEnumerator<ObjectHead*>;

			static const __int32	MaxRef=2147483647;
			static const __int32	MaxPin=2147483647;

			__forceinline ObjectHead* GetObjectHead(GcHandle* handle)
			{
				if(IsValidHandle(handle))
				{
					return (ObjectHead*)handle;
				}
				else
				{
					return 0;
				}
			}

			__forceinline char* GetObjectAddress(ObjectHead* o)
			{
				return (char*)o+sizeof(ObjectHead);
			}

			__forceinline vint GetObjectSize(ObjectHead* o)
			{
				return o->meta->mainSegment.size+o->repeat*o->meta->repeatSegment.size;
			}
		protected:

			Callback				callback;
			void*					userData;
			GeneralObjectPool		pool;
			vint					maxSize;
			vint					usedSize;
			ObjectHead*				firstObject;
			ObjectHead*				lastObject;

			void					MarkSegment(GcMetaSegment* segment, char* address, collections::List<ObjectHead*>& roots);
		public:
			static const vint		ObjectAddressOffset=sizeof(ObjectHead);

			GcSingleThread(Callback _callback, void* _userData, vint poolUnitSize, vint poolUnitCount);
			~GcSingleThread();

			GcHandle*				CreateHandle(GcMeta* meta, vint repeat);
			bool					IsValidHandle(GcHandle* handle);
			GcMeta*					GetHandleMeta(GcHandle* handle);
			bool					IncreaseHandleRef(GcHandle* handle);
			bool					DecreaseHandleRef(GcHandle* handle);
			char*					IncreaseHandlePin(GcHandle* handle);
			bool					DecreaseHandlePin(GcHandle* handle);
			bool					DisposeHandle(GcHandle* handle);
			bool					IsHandleDisposed(GcHandle* handle);
			vint					GetHandleSize(GcHandle* handle);
			vint					GetHandleRepeat(GcHandle* handle);
			bool					ReadHandle(GcHandle* handle, vint offset, vint length, char* buffer);
			bool					WriteHandle(GcHandle* handle, vint offset, vint length, char* buffer);
			bool					Collect();
		};
	}
}

#endif