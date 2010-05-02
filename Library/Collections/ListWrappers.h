/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::List Wrappers

Classes:
	ReadonlyListEnumerator<T>
	ReadonlyListConverter<T>
	ReadonlyListWrapper<T>
	ArrayWrapper<T>
	CollectionWrapper<T>
	ListWrapper<T>
***********************************************************************/

#ifndef VCZH_COLLECTIONS_LISTWRAPPERS
#define VCZH_COLLECTIONS_LISTWRAPPERS

#include "Interfaces.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
´úÀí
***********************************************************************/

		template<typename T, typename K=typename KeyType<T>::Type>
		class ReadonlyListEnumerator : public Object, public virtual IEnumerator<T>
		{
		private:
			const IReadonlyList<T, K>*			container;
			int									index;
		public:
			ReadonlyListEnumerator(const IReadonlyList<T, K>* _container, int _index)
			{
				container=_container;
				index=_index;
			}

			ReadonlyListEnumerator<T>* Clone()const
			{
				return new ReadonlyListEnumerator<T, K>(container, index);
			}

			const T& Current()const
			{
				return container->Get(index);
			}

			int Index()const
			{
				return index;
			}

			bool Next()
			{
				index++;
				return Available();
			}

			bool Available()const
			{
				return index>=0 && index<container->Count();
			}

			void Reset()
			{
				index=0;
			}
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class ReadonlyListConverter : public virtual IReadonlyList<T, K>
		{
		public:
			IEnumerator<T>* CreateEnumerator()const
			{
				return new ReadonlyListEnumerator<T, K>(this, 0);
			}

			bool Contains(const K& item)const
			{
				return IndexOf(item)!=-1;
			}

			const T& operator[](int index)const
			{
				return Get(index);
			}

			int IndexOf(const K& item)const
			{
				int count=Count();
				for(int i=0;i<count;i++)
				{
					if(Get(i)==item)
					{
						return i;
					}
				}
				return -1;
			}
		};

		template<typename C, typename T, typename K=typename KeyType<T>::Type>
		class ReadonlyListWrapper : public Object, public virtual IReadonlyList<T, K>
		{
		private:
			C*									container;
		public:
			ReadonlyListWrapper(C* _container=0)
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

			IEnumerator<T>* CreateEnumerator()const
			{
				return new ReadonlyListEnumerator<T, K>(this, 0);
			}

			bool Contains(const K& item)const
			{
				return container->Contains(item);
			}

			int Count()const
			{
				return container->Count();
			}

			const T& Get(int index)const
			{
				return container->Get(index);
			}

			const T& operator[](int index)const
			{
				return container->operator[](index);
			}

			int IndexOf(const K& item)const
			{
				return container->IndexOf(item);
			}
		};

		template<typename C, typename T, typename K=typename KeyType<T>::Type>
		class ArrayWrapper : public Object, public virtual IArray<T, K>
		{
		private:
			C*									container;
		public:
			ArrayWrapper(C* _container=0)
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

			IEnumerator<T>* CreateEnumerator()const
			{
				return new ReadonlyListEnumerator<T, K>(this, 0);
			}

			bool Contains(const K& item)const
			{
				return container->Contains(item);
			}

			int Count()const
			{
				return container->Count();
			}

			const T& Get(int index)const
			{
				return container->Get(index);
			}

			const T& operator[](int index)const
			{
				return container->operator[](index);
			}

			int IndexOf(const K& item)const
			{
				return container->IndexOf(item);
			}

			void Set(int index, const K& item)
			{
				container->Set(index, item);
			}

			T& operator[](int index)
			{
				return container->operator[](index);
			}

			void Resize(int size)
			{
				container->Resize(size);
			}
		};

		template<typename C, typename T, typename K=KeyType<T>::Type>
		class CollectionWrapper : public Object, public virtual ICollection<T, K>
		{
		private:
			C*									container;
		public:
			CollectionWrapper(C* _container=0)
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

			IEnumerator<T>* CreateEnumerator()const
			{
				return new ReadonlyListEnumerator<T, K>(this, 0);
			}

			bool Contains(const K& item)const
			{
				return container->Contains(item);
			}

			int Count()const
			{
				return container->Count();
			}

			const T& Get(int index)const
			{
				return container->Get(index);
			}

			const T& operator[](int index)const
			{
				return container->operator[](index);
			}

			int IndexOf(const K& item)const
			{
				return container->IndexOf(item);
			}

			int Add(const T& item)
			{
				return container->Add(item);
			}

			bool Remove(const K& item)
			{
				return container->Remove(item);
			}

			bool RemoveAt(int index)
			{
				return container->RemoveAt(index);
			}

			bool RemoveRange(int index, int count)
			{
				return container->RemoveRange(index, count);
			}

			bool Clear()
			{
				return container->Clear();
			}
		};

		template<typename C, typename T, typename K=KeyType<T>::Type>
		class ListWrapper : public Object, public virtual IList<T, K>
		{
		private:
			C*									container;
		public:
			ListWrapper(C* _container=0)
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

			IEnumerator<T>* CreateEnumerator()const
			{
				return new ReadonlyListEnumerator<T, K>(this, 0);
			}

			bool Contains(const K& item)const
			{
				return container->Contains(item);
			}

			int Count()const
			{
				return container->Count();
			}

			const T& Get(int index)const
			{
				return container->Get(index);
			}

			const T& operator[](int index)const
			{
				return container->operator[](index);
			}

			int IndexOf(const K& item)const
			{
				return container->IndexOf(item);
			}

			int Add(const T& item)
			{
				return container->Add(item);
			}

			bool Remove(const K& item)
			{
				return container->Remove(item);
			}

			bool RemoveAt(int index)
			{
				return container->RemoveAt(index);
			}

			bool RemoveRange(int index, int count)
			{
				return container->RemoveRange(index, count);
			}

			bool Clear()
			{
				return container->Clear();
			}

			int Insert(int index, const T& item)
			{
				return container->Insert(index, item);
			}

			bool Set(int index, const K& item)
			{
				return container->Set(index, item);
			}

			T& operator[](int index)
			{
				return container->operator[](index);
			}
		};
	}
}

#endif