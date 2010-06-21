/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Data Structure::Interfaces

Interfaces:
	IEnumerator<T>									��ö����
	IEnumerable<T>									����ö�ٶ���
	IReadonlyList<T>								��ֻ���б�
	IArray<T>										������
	ICollection<T>									������
	IList<T>										���б�
	IReadonlyDictionary<K,V>						��ֻ��ӳ��
	IDictionary<K,V>								��ӳ��
	IReadonlyGroup<K,V>								��ֻ������ӳ��
	IGroup<K,V>										������ӳ��
***********************************************************************/

#ifndef VCZH_COLLECTIONS_INTERFACES
#define VCZH_COLLECTIONS_INTERFACES

#include "..\Basic.h"
#include "Pair.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
�ӿ�
***********************************************************************/

		template<typename T>
		class IEnumerator : public virtual Interface
		{
		public:
			virtual IEnumerator<T>*						Clone()const=0;
			virtual const T&							Current()const=0;
			virtual vint								Index()const=0;
			virtual bool								Next()=0;
			virtual bool								Available()const=0;
			virtual void								Reset()=0;
		};

		template<typename T>
		class IEnumerable : public virtual Interface
		{
		public:
			virtual IEnumerator<T>*						CreateEnumerator()const=0;
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class IReadonlyList : public virtual IEnumerable<T>
		{
		public:
			virtual bool								Contains(const K& item)const=0;
			virtual vint								Count()const=0;
			virtual const T&							Get(vint index)const=0;
			virtual const T&							operator[](vint index)const=0;
			virtual vint								IndexOf(const K& item)const=0;
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class IArray : public virtual IReadonlyList<T, K>
		{
		public:
			virtual void								Set(vint index, const K& item)=0;
			virtual T&									operator[](vint index)=0;
			virtual void								Resize(vint size)=0;
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class ICollection : public virtual IReadonlyList<T, K>
		{
		public:
			virtual vint								Add(const T& item)=0;
			virtual bool								Remove(const K& item)=0;
			virtual bool								RemoveAt(vint index)=0;
			virtual bool								RemoveRange(vint index, vint count)=0;
			virtual bool								Clear()=0;
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class IList : public virtual ICollection<T, K>
		{
		public:
			virtual vint								Insert(vint index, const T& item)=0;
			virtual bool								Set(vint index, const K& item)=0;
			virtual T&									operator[](vint index)=0;
		};

		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class IReadonlyDictionary : public virtual IEnumerable<Pair<KT, VT>>
		{
		public:
			virtual const IReadonlyList<KT, KK>&		Keys()const=0;
			virtual const IReadonlyList<VT, VK>&		Values()const=0;
			virtual vint								Count()const=0;
			virtual const VT&							Get(const KK& key)const=0;
			virtual const VT&							operator[](const KK& key)const=0;
		};

		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class IDictionary : public virtual IReadonlyDictionary<KT, VT, KK, VK>
		{
		public:
			virtual bool								Set(const KK& key, const VT& value)=0;
			virtual bool								Add(const KT& key, const VT& value)=0;
			virtual bool								Remove(const KK& key)=0;
			virtual bool								Clear()=0;
		};

		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class IReadonlyGroup : public virtual IEnumerable<Pair<KT, VT>>
		{
		public:
			virtual const IReadonlyList<KT, KK>&		Keys()const=0;
			virtual vint								Count()const=0;
			virtual const IReadonlyList<VT, VK>&		Get(const KK& key)const=0;
			virtual const IReadonlyList<VT, VK>&		GetByIndex(vint index)const=0;
			virtual const IReadonlyList<VT, VK>&		operator[](const KK& key)const=0;
			virtual bool								Contains(const KK& key)const=0;
			virtual bool								Contains(const KK& key, const VK& value)const=0;
		};

		template<typename KT, typename VT, typename KK=typename KeyType<KT>::Type, typename VK=typename KeyType<VT>::Type>
		class IGroup: public virtual IReadonlyGroup<KT, VT, KK, VK>
		{
		public:
			virtual bool								Add(const KT& key, const VT& value)=0;
			virtual bool								Remove(const KK& key)=0;
			virtual bool								Remove(const KK& key, const VK& value)=0;
			virtual bool								Clear()=0;
		};
	}
}

#endif