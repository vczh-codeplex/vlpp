/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Dictionary Wrappers

Classes:
	ReadonlyDictionaryWrapper<V,K>
	DictionaryWrapper<V,K>
	ReadonlyGroupWrapper<V,K>
	GroupWrapper<V,K>
***********************************************************************/

#ifndef VCZH_COLLECTIONS_DICTIONARYWRAPPERS
#define VCZH_COLLECTIONS_DICTIONARYWRAPPERS

#include "Interfaces.h"
#include "ListWrappers.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
´úÀí
***********************************************************************/

		template<typename C, typename KT, typename VT, typename KK, typename VK>
		class DictionaryWrapper;
		
		template<typename C, typename KT, typename VT, typename KK, typename VK>
		class GroupWrapper;

		template<typename C, typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class ReadonlyDictionaryWrapper : public Object, public virtual IReadonlyDictionary<KT, VT, KK, VK>
		{
			friend class DictionaryWrapper<C, KT, VT, KK, VK>;
		private:
			class Enumerator : public Object, public virtual IEnumerator<Pair<KT, VT>>
			{
			private:
				const IReadonlyDictionary<KT, VT, KK, VK>*	container;
				int											index;
				Pair<KT, VT>								current;

				void UpdateCurrent()
				{
					if(index<container->Count())
					{
						current.key=container->Keys()[index];
						current.value=container->Values()[index];
					}
				}
			public:
				Enumerator(const IReadonlyDictionary* _container, int _index=0)
				{
					container=_container;
					index=_index;
					UpdateCurrent();
				}
				
				IEnumerator<Pair<KT, VT>>* Clone()const
				{
					return new Enumerator(container, index);
				}

				const Pair<KT, VT>& Current()const
				{
					return current;
				}

				int Index()const
				{
					return index;
				}

				bool Next()
				{
					index++;
					UpdateCurrent();
					return Available();
				}

				bool Available()const
				{
					return index>=0 && index<container->Count();
				}

				void Reset()
				{
					index=0;
					UpdateCurrent();
				}
			};

			C*						container;
		public:
			ReadonlyDictionaryWrapper(C* _container=0)
			{
				container=_container;
			}

			C* GetContainer()
			{
				return container;
			}

			void SetContainer(C* _container)
			{
				container=_container;
			}

			IEnumerator<Pair<KT, VT>>* CreateEnumerator()const
			{
				return new Enumerator(this);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return container->Keys();
			}

			const IReadonlyList<VT, VK>& Values()const
			{
				return container->Values();
			}

			int Count()const
			{
				return container->Count();
			}

			const VT& Get(const KK& key)const
			{
				return container->Get(key);
			}

			const VT& operator[](const KK& key)const
			{
				return container->operator[](key);
			}
		};

		template<typename C, typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class DictionaryWrapper : public Object, public virtual IDictionary<KT, VT, KK, VK>
		{
		private:
			C*						container;
		public:
			DictionaryWrapper(C* _container=0)
			{
				container=_container;
			}

			C* GetContainer()
			{
				return container;
			}

			void SetContainer(C* _container)
			{
				container=_container;
			}

			IEnumerator<Pair<KT, VT>>* CreateEnumerator()const
			{
				return new ReadonlyDictionaryWrapper<C, KT, VT, KK, VK>::Enumerator(this);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return container->Keys();
			}

			const IReadonlyList<VT, VK>& Values()const
			{
				return container->Values();
			}

			int Count()const
			{
				return container->Count();
			}

			const VT& Get(const KK& key)const
			{
				return container->Get(key);
			}

			const VT& operator[](const KK& key)const
			{
				return container->operator[](key);
			}

			bool Set(const KK& key, const VT& value)
			{
				return container->Set(key, value);
			}

			bool Add(const KT& key, const VT& value)
			{
				return container->Add(key, value);
			}

			bool Remove(const KK& key)
			{
				return container->Remove(key);
			}

			bool Clear()
			{
				return container->Clear();
			}
		};

		template<typename C, typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class ReadonlyGroupWrapper : public Object, public virtual IReadonlyGroup<KT, VT, KK, VK>
		{
			friend class GroupWrapper<C, KT, VT, KK, VK>;
		private:
			class Enumerator : public Object, public virtual IEnumerator<Pair<KT, VT>>
			{
			private:
				const IReadonlyGroup<KT, VT, KK, VK>*		container;
				int											keyIndex;
				int											valueIndex;
				Pair<KT, VT>								current;

				void UpdateCurrent()
				{
					if(keyIndex<container->Count())
					{
						const IReadonlyList<VT, VK>& values=container->GetByIndex(keyIndex);
						if(valueIndex<values.Count())
						{
							current.key=container->Keys()[keyIndex];
							current.value=values[valueIndex];
						}
					}
				}
			public:
				Enumerator(const IReadonlyGroup* _container, int _keyIndex=0, int _valueIndex=0)
				{
					container=_container;
					keyIndex=_keyIndex;
					valueIndex=_valueIndex;
					UpdateCurrent();
				}
				
				IEnumerator<Pair<KT, VT>>* Clone()const
				{
					return new Enumerator(container, keyIndex, valueIndex);
				}

				const Pair<KT, VT>& Current()const
				{
					return current;
				}

				int Index()const
				{
					if(Available())
					{
						int index=0;
						for(int i=0;i<keyIndex;i++)
						{
							index+=container->GetByIndex(i).Count();
						}
						return index+valueIndex;
					}
					else
					{
						return -1;
					}
				}

				bool Next()
				{
					if(keyIndex<container->Count())
					{
						const IReadonlyList<VT, VK>& values=container->GetByIndex(keyIndex);
						valueIndex++;
						if(valueIndex<values.Count())
						{
							UpdateCurrent();
							return true;
						}
						else
						{
							keyIndex++;
							valueIndex=0;
							UpdateCurrent();
							return keyIndex<container->Count();
						}
					}
					else
					{
						return false;
					}
				}

				bool Available()const
				{
					if(keyIndex<container->Count())
					{
						const IReadonlyList<VT, VK>& values=container->GetByIndex(keyIndex);
						if(valueIndex<values.Count())
						{
							return true;
						}
					}
					return false;
				}

				void Reset()
				{
					keyIndex=0;
					valueIndex=0;
					UpdateCurrent();
				}
			};

			C*									container;
		public:
			ReadonlyGroupWrapper(C* _container=0)
			{
				container=_container;
			}

			C* GetContainer()
			{
				return container;
			}

			void SetContainer(C* _container)
			{
				container=_container;
			}

			IEnumerator<Pair<KT, VT>>* CreateEnumerator()const
			{
				return new Enumerator(this);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return container->Keys();
			}

			int Count()const
			{
				return container->Count();
			}

			const IReadonlyList<VT, VK>& Get(const KK& key)const
			{
				return container->Get(key);
			}

			const IReadonlyList<VT, VK>& GetByIndex(int index)const
			{
				return container->GetByIndex(index);
			}

			const IReadonlyList<VT, VK>& operator[](const KK& key)const
			{
				return container->operator[](key);
			}

			bool Contains(const KK& key)const
			{
				return container->Contains(key);
			}

			bool Contains(const KK& key, const VK& value)const
			{
				return container->Contains(key, value);
			}
		};

		template<typename C, typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class GroupWrapper : public Object, public virtual IGroup<KT, VT, KK, VK>
		{
		private:
			C*									container;
		public:
			GroupWrapper(C* _container=0)
			{
				container=_container;
			}

			C* GetContainer()
			{
				return container;
			}

			void SetContainer(C* _container)
			{
				container=_container;
			}

			IEnumerator<Pair<KT, VT>>* CreateEnumerator()const
			{
				return new ReadonlyGroupWrapper<C, KT, VT, KK, VK>::Enumerator(this);
			}

			const IReadonlyList<KT, KK>& Keys()const
			{
				return container->Keys();
			}

			int Count()const
			{
				return container->Count();
			}

			const IReadonlyList<VT, VK>& Get(const KK& key)const
			{
				return container->Get(key);
			}

			const IReadonlyList<VT, VK>& GetByIndex(int index)const
			{
				return container->GetByIndex(index);
			}

			const IReadonlyList<VT, VK>& operator[](const KK& key)const
			{
				return container->operator[](key);
			}

			bool Contains(const KK& key)const
			{
				return container->Contains(key);
			}

			bool Contains(const KK& key, const VK& value)const
			{
				return container->Contains(key, value);
			}

			bool Add(const KT& key, const VT& value)
			{
				return container->Add(key, value);
			}

			bool Remove(const KK& key)
			{
				return container->Remove(key);
			}

			bool Remove(const KK& key, const VK& value)
			{
				return container->Remove(key, value);
			}

			bool Clear()
			{
				return container->Clear();
			}
		};
	}
}

#endif