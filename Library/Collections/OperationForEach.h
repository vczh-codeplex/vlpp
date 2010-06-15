/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Data Structure::Operations

扩展：
	实现一个函数重载IteratorType CreateForEachIterator(const CollectionType& collection);
	CollectionType是所需要的容器类型
	IteratorType继承自ForEachIterator<T>
	必须写在vl::collections命名空间里
***********************************************************************/

#ifndef VCZH_COLLECTIONS_FOREACH
#define VCZH_COLLECTIONS_FOREACH

namespace vl
{
	namespace collections
	{

/***********************************************************************
ForEach基础设施
***********************************************************************/

		template<typename T>
		class ForEachIterator : public Object
		{
		public:
			virtual const T&			Current()const=0;
			virtual bool				Available()const=0;
			virtual void				Next(T& variable)const=0;

			operator bool()const
			{
				return true;
			}
		};

/***********************************************************************
IEnumerable<T>支持
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

			const T& Current()const
			{
				return iterator->Current();
			}

			bool Available()const
			{
				return iterator->Available();
			}

			void Next(T& variable)const
			{
				if(iterator->Next())
				{
					variable=iterator->Current();
				}
			}
		};

		template<typename T>
		EnumerableForEachIterator<T> CreateForEachIterator(const IEnumerable<T>& enumerable)
		{
			return enumerable;
		}

/***********************************************************************
ForEach宏
***********************************************************************/

#define SCOPE_VARIABLE(TYPE, VARIABLE, VALUE)\
		if(bool __scope_variable_flag__=true)\
			for(TYPE VARIABLE = VALUE;__scope_variable_flag__;__scope_variable_flag__=false)

#define FOREACH(TYPE, VARIABLE, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		for(TYPE VARIABLE = __foreach_iterator__.Current();__foreach_iterator__.Available();__foreach_iterator__.Next(VARIABLE))

#define FOREACH_INDEXER(TYPE, VARIABLE, INDEXER, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		SCOPE_VARIABLE(vint, INDEXER, 0)\
		for(TYPE VARIABLE = __foreach_iterator__.Current();__foreach_iterator__.Available();__foreach_iterator__.Next(VARIABLE),INDEXER++)
	}
}

#endif