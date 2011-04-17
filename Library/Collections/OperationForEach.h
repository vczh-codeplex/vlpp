/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Data Structure::Operations

��չ��
	ʵ��һ����������IteratorType CreateForEachIterator(const CollectionType& collection);
	CollectionType������Ҫ����������
	IteratorType�̳���ForEachIterator<T>
	����д��vl::collections�����ռ���
***********************************************************************/

#ifndef VCZH_COLLECTIONS_FOREACH
#define VCZH_COLLECTIONS_FOREACH
#include "..\Basic.h"
#include "..\Pointer.h"
#include "Interfaces.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
ForEach������ʩ
***********************************************************************/

		template<typename T>
		class ForEachIterator : public Object
		{
		public:
			virtual bool				Available(T& variable)const=0;
			virtual void				Next()const=0;

			operator bool()const
			{
				return true;
			}
		};

/***********************************************************************
IEnumerable<T>֧��
***********************************************************************/

		template<typename T>
		class EnumerableForEachIterator : public ForEachIterator<T>
		{
		protected:
			Ptr<IEnumerator<T>>			iterator;
		public:
			EnumerableForEachIterator(const IEnumerable<T>& enumerable)
				:iterator(enumerable.CreateEnumerator())
			{
			}

			EnumerableForEachIterator(const EnumerableForEachIterator<T>& enumerableIterator)
				:iterator(enumerableIterator.iterator)
			{
			}

			bool Available(T& variable)const
			{
				if(iterator->Available())
				{
					variable=iterator->Current();
					return true;
				}
				else
				{
					return false;
				}
			}

			void Next()const
			{
				iterator->Next();
			}
		};

		template<typename T>
		EnumerableForEachIterator<T> CreateForEachIterator(const IEnumerable<T>& enumerable)
		{
			return enumerable;
		}

/***********************************************************************
ForEach��
***********************************************************************/

#define SCOPE_VARIABLE(TYPE, VARIABLE, VALUE)\
		if(bool __scope_variable_flag__=true)\
			for(TYPE VARIABLE = VALUE;__scope_variable_flag__;__scope_variable_flag__=false)

#define FOREACH(TYPE, VARIABLE, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		for(TYPE VARIABLE;__foreach_iterator__.Available(VARIABLE);__foreach_iterator__.Next())

#define FOREACH_INDEXER(TYPE, VARIABLE, INDEXER, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		SCOPE_VARIABLE(vint, INDEXER, 0)\
		for(TYPE VARIABLE;__foreach_iterator__.Available(VARIABLE);__foreach_iterator__.Next(),INDEXER++)
	}
}

#endif