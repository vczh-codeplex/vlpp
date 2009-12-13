/***********************************************************************
Vczh Library++ 3.0
Developer: ≥¬Ë˜Â´(vczh)
Data Structure::Dictionary

Classes:
	Dictionary<KT, VT, KK, VK>					£∫”≥…‰
	Group<KT, VT, KK, VK>						£∫∂‡÷ÿ”≥…‰
***********************************************************************/

#ifndef VCZH_COLLECTIONS_DICTIONARY
#define VCZH_COLLECTIONS_DICTIONARY

#include "List.h"
#include "DictionaryWrappers.h"

namespace vl
{
	namespace collections
	{
		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class Dictionary : public Object, private NotCopyable
		{
		protected:
			SortedList<KT, KK>					keys;
			List<VT, VK>						values;
			mutable DictionaryWrapper<Dictionary<KT, VT, KK, VK>, KT, VT, KK, VK>	wrapper;
		public:
			Dictionary()
			{
				wrapper.SetContainer(this);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return keys.Wrap();
			}

			const IReadonlyList<VT, VK>& Values()const
			{
				return values.Wrap();
			}

			int Count()const
			{
				return keys.Count();
			}

			const VT& Get(const KK& key)const
			{
				return values.Get(keys.IndexOf(key));
			}

			const VT& operator[](const KK& key)const
			{
				return values.Get(keys.IndexOf(key));
			}

			bool Set(const KK& key, const VT& value)
			{
				int index=keys.IndexOf(key);
				if(index==-1)
				{
					index=keys.Add(key);
					values.Insert(index, value);
				}
				else
				{
					values[index]=value;
				}
				return true;
			}

			bool Add(const KT& key, const VT& value)
			{
				CHECK_ERROR(!keys.Contains(key), L"Dictionary<KT, KK, VT, VK>::Add(const KT&, const VT&)#key“—¥Ê‘⁄°£");
				int index=keys.Add(key);
				values.Insert(index, value);
				return true;
			}

			bool Remove(const KK& key)
			{
				int index=keys.IndexOf(key);
				if(index!=-1)
				{
					keys.RemoveAt(index);
					values.RemoveAt(index);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool Clear()
			{
				keys.Clear();
				values.Clear();
				return true;
			}

			IDictionary<KT, VT, KK, VK>& Wrap()const
			{
				return wrapper;
			}
		};

		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class Group : public Object, private NotCopyable
		{
		protected:
			SortedList<KT, KK>				keys;
			List<List<VT, VK>*>				values;
			mutable GroupWrapper<Group<KT, VT, KK, VK>, KT, VT, KK, VK>	wrapper;
		public:
			Group()
			{
				wrapper.SetContainer(this);
			}

			~Group()
			{
				Clear();
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return keys.Wrap();
			}

			int Count()const
			{
				return keys.Count();
			}

			const IReadonlyList<VT, VK>& Get(const KK& key)const
			{
				return values.Get(keys.IndexOf(key))->Wrap();
			}

			const IReadonlyList<VT, VK>& GetByIndex(int index)const
			{
				return values.Get(index)->Wrap();
			}

			const IReadonlyList<VT, VK>& operator[](const KK& key)const
			{
				return values.Get(keys.IndexOf(key))->Wrap();
			}

			bool Contains(const KK& key)const
			{
				return keys.Contains(key);
			}

			bool Contains(const KK& key, const VK& value)const
			{
				int index=keys.IndexOf(key);
				if(index!=-1)
				{
					return values.Get(index)->Contains(value);
				}
				else
				{
					return false;
				}
			}

			bool Add(const KT& key, const VT& value)
			{
				List<VT, VK>* target=0;
				int index=keys.IndexOf(key);
				if(index==-1)
				{
					target=new List<VT, VK>;
					values.Insert(keys.Add(key), target);
				}
				else
				{
					target=values[index];
				}
				target->Add(value);
				return true;
			}

			bool Remove(const KK& key)
			{
				int index=keys.IndexOf(key);
				if(index!=-1)
				{
					keys.RemoveAt(index);
					List<VT, VK>* target=values[index];
					values.RemoveAt(index);
					delete target;
					return true;
				}
				else
				{
					return false;
				}
			}

			bool Remove(const KK& key, const VK& value)
			{
				int index=keys.IndexOf(key);
				if(index!=-1)
				{
					List<VT, VK>* target=values[index];
					target->Remove(value);
					if(target->Count()==0)
					{
						keys.RemoveAt(index);
						values.RemoveAt(index);
						delete target;
					}
					return true;
				}
				else
				{
					return false;
				}
			}

			bool Clear()
			{
				for(int i=0;i<values.Count();i++)
				{
					delete values[i];
				}
				keys.Clear();
				values.Clear();
				return true;
			}

			IGroup<KT, VT, KK, VK>& Wrap()const
			{
				return wrapper;
			}
		};
	}
}

#endif