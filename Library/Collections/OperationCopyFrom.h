/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONCOPYFROM
#define VCZH_COLLECTIONS_OPERATIONCOPYFROM

#include "Interfaces.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
ÈÝÆ÷¸´ÖÆ
***********************************************************************/

		template<typename T, typename K>
		void CopyFrom(IArray<T, K>& dst, const IReadonlyList<T, K>& src, bool append=false)
		{
			int start=0;
			if(append)
			{
				start=dst.Count();
				dst.Resize(start+src.Count());
			}
			else
			{
				dst.Resize(src.Count());
			}
			for(int i=0;i<src.Count();i++)
			{
				dst[start+i]=src[i];
			}
		}

		template<typename T, typename K>
		void CopyFrom(ICollection<T, K>& dst, const IReadonlyList<T, K>& src, bool append=false)
		{
			if(!append)
			{
				dst.Clear();
			}
			for(int i=0;i<src.Count();i++)
			{
				dst.Add(src[i]);
			}
		}

		template<typename KT, typename VT, typename KK, typename VK>
		void CopyFrom(IDictionary<KT, VT, KK, VK>& dst, const IReadonlyList<Pair<KT, VT>>& src, bool append=false)
		{
			if(!append)
			{
				dst.Clear();
			}
			for(int i=0;i<src.Count();i++)
			{
				const Pair<KT, VT>& pair=src[i];
				dst.Set(pair.key, pair.value);
			}
		}

		template<typename KT, typename VT, typename KK, typename VK>
		void CopyFrom(IGroup<KT, VT, KK, VK>& dst, const IReadonlyList<Pair<KT, VT>>& src, bool append=false)
		{
			if(!append)
			{
				dst.Clear();
			}
			for(int i=0;i<src.Count();i++)
			{
				const Pair<KT, VT>& pair=src[i];
				dst.Add(pair.key, pair.value);
			}
		}

		template<typename T, typename K>
		void CopyFrom(IArray<T, K>& dst, const IEnumerable<T>& src, bool append=false)
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
				int start=0;
				if(append)
				{
					start=dst.Count();
					dst.Resize(start+count);
				}
				else
				{
					dst.Resize(count);
				}
				while(enumerator->Available())
				{
					dst[start+enumerator->Index()]=enumerator->Current();
					enumerator->Next();
				}
				delete enumerator;
			}
			catch(...)
			{
				delete enumerator;
				throw;
			}
		}

		template<typename T, typename K>
		void CopyFrom(ICollection<T, K>& dst, const IEnumerable<T>& src, bool append=false)
		{
			IEnumerator<T>* enumerator=src.CreateEnumerator();
			try
			{
				if(!append)
				{
					dst.Clear();
				}
				while(enumerator->Available())
				{
					dst.Add(enumerator->Current());
					enumerator->Next();
				}
				delete enumerator;
			}
			catch(...)
			{
				delete enumerator;
				throw;
			}
		}

		template<typename KT, typename VT, typename KK, typename VK>
		void CopyFrom(IDictionary<KT, VT, KK, VK>& dst, const IEnumerable<Pair<KT, VT>>& src, bool append=false)
		{
			IEnumerator<Pair<KT, VT>>* enumerator=src.CreateEnumerator();
			try
			{
				if(!append)
				{
					dst.Clear();
				}
				while(enumerator->Available())
				{
					const Pair<KT, VT>& pair=enumerator->Current();
					dst.Set(pair.key, pair.value);
					enumerator->Next();
				}
				delete enumerator;
			}
			catch(...)
			{
				delete enumerator;
				throw;
			}
		}

		template<typename KT, typename VT, typename KK, typename VK>
		void CopyFrom(IGroup<KT, VT, KK, VK>& dst, const IEnumerable<Pair<KT, VT>>& src, bool append=false)
		{
			IEnumerator<Pair<KT, VT>>* enumerator=src.CreateEnumerator();
			try
			{
				if(!append)
				{
					dst.Clear();
				}
				while(enumerator->Available())
				{
					const Pair<KT, VT>& pair=enumerator->Current();
					dst.Add(pair.key, pair.value);
					enumerator->Next();
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