/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONENUMERABLE
#define VCZH_COLLECTIONS_OPERATIONENUMERABLE

#include "Interfaces.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Ëã·¨´®Áª
***********************************************************************/

		template<typename T, typename R>
		class EnumerableProcessor : public Object
		{
		public:
			virtual R operator()(const IEnumerable<T>& enumerable)const=0;
		};

		template<typename T, typename R>
		R operator>>(const IEnumerable<T>& enumerable, const EnumerableProcessor<T, R>& processor)
		{
			return processor(enumerable);
		}

		template<template<typename T> class R>
		class SequenceEnumerableProcessor : public Object
		{
		public:
			template<typename T>
			struct ResultTypeRetriver
			{
				typedef R<T> ResultType;
			};
		};

		class AggregateEnumerableProcessor : public Object
		{
		public:
			template<typename T>
			struct ResultTypeRetriver
			{
				typedef T ResultType;
			};
		};

		class FreeEnumerableProcessor : public Object
		{
		};

		template<typename T, typename P>
		typename P::ResultTypeRetriver<T>::ResultType operator>>(const IEnumerable<T>& enumerable, const P& processor)
		{
			return processor(enumerable);
		}

/***********************************************************************
µü´úÆ÷´æ´¢
***********************************************************************/

		template<typename T, int I=0>
		class EnumerableStore : public virtual Object
		{
		private:
			IEnumerator<T>*			enumerator;
		protected:
			IEnumerator<T>* CopyEnumerator()const
			{
				return enumerator->Clone();
			}
		public:
			EnumerableStore(const IEnumerable<T>& enumerable)
			{
				enumerator=enumerable.CreateEnumerator();
			}

			EnumerableStore(const EnumerableStore<T>& store)
			{
				enumerator=store->CopyEnumerator();
			}

			~EnumerableStore()
			{
				delete enumerator;
			}

			EnumerableStore<T>& operator=(const EnumerableStore<T>& store)
			{
				delete enumerator;
				enumerator=store->CopyEnumerator();
				return *this;
			}
		};

/***********************************************************************
µü´úÆ÷±È½Ï
***********************************************************************/

		template<typename T, typename U>
		int CompareEnumerable(const IEnumerable<T>& a, const IEnumerable<U>& b)
		{
			Ptr<IEnumerator<T>> ator=a.CreateEnumerator();
			Ptr<IEnumerator<U>> btor=b.CreateEnumerator();
			while(ator->Available() && btor->Available())
			{
				const T& ac=ator->Current();
				const U& bc=btor->Current();
				if(ac<bc)
				{
					return -1;
				}
				else if(ac>bc)
				{
					return 1;
				}
				ator->Next();
				btor->Next();
			}
			if(ator->Available())
			{
				return 1;
			}
			else if(btor->Available())
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
}

#endif