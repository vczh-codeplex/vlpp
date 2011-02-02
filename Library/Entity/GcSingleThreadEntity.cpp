#include "GcSingleThreadEntity.h"

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

		GcMeta* GcSingleThread::GetHandleMeta(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::IsValidHandle(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::IncreaseHandleRef(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::DecreaseHandleRef(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		char* GcSingleThread::IncreaseHandlePin(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::DecreaseHandlePin(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::DisposeHandle(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::IsHandleDisposed(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		vint GcSingleThread::GetHandleSize(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		vint GcSingleThread::GetHandleRepeat(GcHandle* handle)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::ReadHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::WriteHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			CHECK_FAIL(L"NotImplemented");
		}

		bool GcSingleThread::Collect()
		{
			CHECK_FAIL(L"NotImplemented");
		}
	}
}