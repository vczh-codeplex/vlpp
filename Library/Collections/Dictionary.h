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
		template<
			typename KT,
			typename VT,
			typename ValueContainer=List<VT, typename KeyType<VT>::Type>,
			typename KK=typename KeyType<KT>::Type, 
			typename VK=typename KeyType<VT>::Type
		>
		class Dictionary : public Object, private NotCopyable
		{
		protected:
			SortedList<KT, KK>					keys;
			ValueContainer						values;
			mutable DictionaryWrapper<Dictionary<KT, VT, ValueContainer, KK, VK>, KT, VT, KK, VK>	wrapper;
		public:
			Dictionary()
			{
				wrapper.SetContainer(this);
			}

			void SetLessMemoryMode(bool mode)
			{
				keys.SetLessMemoryMode(mode);
				values.SetLessMemoryMode(mode);
			}

			template<typename T>
			void CopyKeysToCollection(T& dst, bool append=false)const
			{
				CopyToCollection(dst, keys, append);
			}

			template<typename T>
			void CopyKeysToArray(T& dst, bool append=false)const
			{
				CopyToArray(dst, keys, append);
			}

			template<typename T>
			void CopyValuesToCollection(T& dst, bool append=false)const
			{
				CopyToCollection(dst, values, append);
			}

			template<typename T>
			void CopyValuesToArray(T& dst, bool append=false)const
			{
				CopyToArray(dst, values, append);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return keys.Wrap();
			}

			const IReadonlyList<VT, VK>& Values()const
			{
				return values.Wrap();
			}

			vint Count()const
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
				vint index=keys.IndexOf(key);
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
				CHECK_ERROR(!keys.Contains(key), L"Dictionary<KT, KK, ValueContainer, VT, VK>::Add(const KT&, const VT&)#key“—¥Ê‘⁄°£");
				vint index=keys.Add(key);
				values.Insert(index, value);
				return true;
			}

			bool Remove(const KK& key)
			{
				vint index=keys.IndexOf(key);
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

		template<
			typename KT,
			typename VT,
			typename ValueContainer=List<VT, typename KeyType<VT>::Type>,
			typename KK=typename KeyType<KT>::Type,
			typename VK=typename KeyType<VT>::Type
		>
		class Group : public Object, private NotCopyable
		{
		protected:
			SortedList<KT, KK>				keys;
			List<ValueContainer*>			values;
			mutable GroupWrapper<Group<KT, VT, ValueContainer, KK, VK>, KT, VT, KK, VK>	wrapper;
		public:
			Group()
			{
				wrapper.SetContainer(this);
			}

			~Group()
			{
				Clear();
			}

			template<typename T>
			void CopyKeysToCollection(T& dst, bool append=false)const
			{
				CopyToCollection(dst, keys, append);
			}

			template<typename T>
			void CopyKeysToArray(T& dst, bool append=false)const
			{
				CopyToArray(dst, keys, append);
			}

			template<typename T>
			void CopyValuesToCollection(vint index, T& dst, bool append=false)const
			{
				CopyToCollection(dst, *(values.Get(index)), append);
			}

			template<typename T>
			void CopyValuesToArray(vint index, T& dst, bool append=false)const
			{
				CopyToArray(dst, *(values.Get(index)), append);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return keys.Wrap();
			}

			vint Count()const
			{
				return keys.Count();
			}

			const IReadonlyList<VT, VK>& Get(const KK& key)const
			{
				return values.Get(keys.IndexOf(key))->Wrap();
			}

			const IReadonlyList<VT, VK>& GetByIndex(vint index)const
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
				vint index=keys.IndexOf(key);
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
				ValueContainer* target=0;
				vint index=keys.IndexOf(key);
				if(index==-1)
				{
					target=new ValueContainer;
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
				vint index=keys.IndexOf(key);
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
				vint index=keys.IndexOf(key);
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
				for(vint i=0;i<values.Count();i++)
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