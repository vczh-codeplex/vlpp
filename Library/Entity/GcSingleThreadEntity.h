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
			Callback				callback;
			void*					userData;
		public:
			GcSingleThread(Callback _callback, void* _userData);
			~GcSingleThread();

			GcHandle*				CreateHandle(GcMeta* meta, vint repeat);
			GcMeta*					GetHandleMeta(GcHandle* handle);
			bool					IsValidHandle(GcHandle* handle);
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