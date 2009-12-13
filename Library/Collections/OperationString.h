/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONSTRING
#define VCZH_COLLECTIONS_OPERATIONSTRING

#include "Interfaces.h"
#include "..\String.h"

namespace vl
{
	namespace collections
	{
		template<typename T, typename K>
		void CopyFrom(IArray<T, K>& dst, const ObjectString<T>& src, bool append=false)
		{
			int start=0;
			if(append)
			{
				start=dst.Count();
				dst.Resize(start+src.Length());
			}
			else
			{
				dst.Resize(src.Length());
			}
			for(int i=0;i<src.Length();i++)
			{
				dst[start+i]=src[i];
			}
		}

		template<typename T, typename K>
		void CopyFrom(ICollection<T, K>& dst, const ObjectString<T>& src, bool append=false)
		{
			if(!append)
			{
				dst.Clear();
			}
			for(int i=0;i<src.Length();i++)
			{
				dst.Add(src[i]);
			}
		}

		template<typename T, typename K>
		void CopyFrom(ObjectString<T>& dst, const IReadonlyList<T, K>& src, bool append=false)
		{
			T* buffer=new T[src.Count()+1];
			try
			{
				for(int i=0;i<src.Count();i++)
				{
					buffer[i]=src[i];
				}
				buffer[src.Count()]=0;
				if(append)
				{
					dst+=buffer;
				}
				else
				{
					dst=buffer;
				}
				delete[] buffer;
			}
			catch(...)
			{
				delete[] buffer;
				throw;
			}
		}

		template<typename T>
		void CopyFrom(ObjectString<T>& dst, const IEnumerable<T>& src, bool append=false)
		{
			IEnumerator<T>* enumerator=src.CreateEnumerator();
			try
			{
				int count=0;
				while(enumerator->Available())
				{
					count++;
					enumerator->Next();
				}
				enumerator->Reset();
				T* buffer=new T[count+1];
				try
				{
					while(enumerator->Available())
					{
						buffer[enumerator->Index()]=enumerator->Current();
						enumerator->Next();
					}
					buffer[count]=0;
					if(append)
					{
						dst+=buffer;
					}
					else
					{
						dst=buffer;
					}
					delete[] buffer;
				}
				catch(...)
				{
					delete[] buffer;
					throw;
				}
				delete enumerator;
			}
			catch(...)
			{
				delete enumerator;
				throw;
			}
		}
	}
}

#endif