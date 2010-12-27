#include "BigObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
/***********************************************************************
BigObjectPool
***********************************************************************/
		
		BigObjectPool::BigObjectPool(vint _totalSize, vint _minObjectSize)
			:totalSize(_totalSize)
			,minObjectSize(_minObjectSize)
			,usedSize(0)
		{
			buffer=new char[totalSize];
		}

		BigObjectPool::~BigObjectPool()
		{
			delete[] buffer;
		}

		char* BigObjectPool::Alloc()
		{
			throw 0;
		}

		bool BigObjectPool::Free(char* handle)
		{
			throw 0;
		}

		bool BigObjectPool::IsValid(char* handle)
		{
			throw 0;
		}

		char* BigObjectPool::GetHandle(char* pointer)
		{
			throw 0;
		}
	}
}