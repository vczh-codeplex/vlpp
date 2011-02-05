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
				__int32				repeat;
				__int32				pin;
			};

			struct HandleHead
			{
				ObjectHead*			object;
				__int32				ref;
			};

			static const __int32	MaxPin=2147483647;
			static const __int32	MaxRef=2147483647;

			Callback				callback;
			void*					userData;

			__forceinline ObjectHead* GetObjectHead(GcHandle* handle)
			{
				if(IsValidHandle(handle))
				{
					return ((HandleHead*)handle)->object;
				}
				else
				{
					return 0;
				}
			}

			__forceinline HandleHead* GetHandleHead(GcHandle* handle)
			{
				return IsValidHandle(handle)?(HandleHead*)handle:0;
			}

			__forceinline char* GetObjectAddress(ObjectHead* o)
			{
				return (char*)o+sizeof(ObjectHead);
			}

			__forceinline vint GetObjectSize(ObjectHead* o)
			{
				return o->meta->mainSegment.size+o->repeat*o->meta->repeatSegment.size;
			}
		public:
			GcSingleThread(Callback _callback, void* _userData);
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