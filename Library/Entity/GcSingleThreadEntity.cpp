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
			HandleHead* h=GetHandleHead(handle);
			if(h&&h->ref!=MaxRef)
			{
				h->ref++;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::DecreaseHandleRef(GcHandle* handle)
		{
			HandleHead* h=GetHandleHead(handle);
			if(h&&h->ref)
			{
				h->ref--;
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
			if(o&&o->pin!=MaxPin)
			{
				o->pin++;
				return GetObjectAddress(o);
			}
			else
			{
				return 0;
			}
		}

		bool GcSingleThread::DecreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->pin)
			{
				o->pin--;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::DisposeHandle(GcHandle* handle)
		{
			HandleHead* h=GetHandleHead(handle);
			if(h)
			{
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
			HandleHead* h=GetHandleHead(handle);
			return h?!h->object:false;
		}

		vint GcSingleThread::GetHandleSize(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return GetObjectSize(o);
		}

		vint GcSingleThread::GetHandleRepeat(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o?o->repeat:-1;
		}

		bool GcSingleThread::ReadHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o)
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
			if(o)
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