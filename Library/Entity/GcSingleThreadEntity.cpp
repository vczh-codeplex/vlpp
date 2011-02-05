#include "GcSingleThreadEntity.h"
#include <memory.h>

namespace vl
{
	namespace entities
	{
/***********************************************************************
GcSingleThread
***********************************************************************/

		GcSingleThread::GcSingleThread(Callback _callback, void* _userData)
			:callback(_callback)
			,userData(_userData)
		{
		}

		GcSingleThread::~GcSingleThread()
		{
		}

		GcHandle* GcSingleThread::CreateHandle(GcMeta* meta, vint repeat)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::IsValidHandle(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		GcMeta* GcSingleThread::GetHandleMeta(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o?o->meta:0;
		}

		bool GcSingleThread::IncreaseHandleRef(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->ref!=MaxRef)
			{
				o->ref++;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::DecreaseHandleRef(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->ref)
			{
				o->ref--;
				return true;
			}
			else
			{
				return false;
			}
		}

		char* GcSingleThread::IncreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta?GetObjectAddress(o):0;
		}

		bool GcSingleThread::DecreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta;
		}

		bool GcSingleThread::DisposeHandle(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				o->meta=0;
				CHECK_FAIL(L"NotImplemented");
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::IsHandleDisposed(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return !(&o->meta);
		}

		vint GcSingleThread::GetHandleSize(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta?GetObjectSize(o):-1;
		}

		vint GcSingleThread::GetHandleRepeat(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta?o->repeat:-1;
		}

		bool GcSingleThread::ReadHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				char* source=GetObjectAddress(o);
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					memcpy(buffer, source+offset, length);
					return true;
				}
			}
			return false;
		}

		bool GcSingleThread::WriteHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				char* source=GetObjectAddress(o);
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					memcpy(source+offset, buffer, length);
					return true;
				}
			}
			return false;
		}

		bool GcSingleThread::Collect()
		{
			CHECK_FAIL(L"NotImplemented");
		}
	}
}