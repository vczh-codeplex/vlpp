/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONAGGREGATE
#define VCZH_COLLECTIONS_OPERATIONAGGREGATE

#include "OperationEnumerable.h"
#include "OperationSelect.h"
#include "OperationForEach.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Aggregate
***********************************************************************/

		template<typename T>
		class AggregateProcessor : public EnumerableProcessor<T, T>
		{
		protected:
			Func<T(T,T)>				selector;
			T							initial;
			bool						hasInitial;
		public:
			AggregateProcessor(const Func<T(T,T)>& _selector)
				:selector(_selector)
				,hasInitial(false)
			{
			}

			AggregateProcessor(const Func<T(T,T)>& _selector, const T& _initial)
				:selector(_selector)
				,initial(_initial)
				,hasInitial(true)
			{
			}

			T operator()(const IEnumerable<T>& enumerable)const
			{
				T result;
				IEnumerator<T>* enumerator=enumerable.CreateEnumerator();
				try
				{
					if(hasInitial)
					{
						result=initial;
					}
					else if(enumerator->Available())
					{
						result=enumerator->Current();
						enumerator->Next();
					}
					else
					{
						throw Error(L"AggregateProcessor<T>::operator(const IEnumerable<T>&)#ÈÝÆ÷Îª¿Õ²¢ÇÒÃ»ÓÐ³õÊ¼Öµ£¬Aggregate²Ù×÷Ê§°Ü¡£");
					}
					while(enumerator->Available())
					{
						result=selector(result, enumerator->Current());
						enumerator->Next();
					}
					delete enumerator;
				}
				catch(...)
				{
					delete enumerator;
					throw;
				}
				return result;
			}
		};

		template<typename T>
		AggregateProcessor<T> Aggregate(const Func<T(T,T)>& selector)
		{
			return AggregateProcessor<T>(selector);
		}

		template<typename T>
		AggregateProcessor<T> Aggregate(T(*selector)(T,T))
		{
			return AggregateProcessor<T>(selector);
		}

		template<typename T>
		AggregateProcessor<T> Aggregate(const Func<T(T,T)>& selector, const T& initial)
		{
			return AggregateProcessor<T>(selector, initial);
		}

		template<typename T>
		AggregateProcessor<T> Aggregate(T(*selector)(T,T), const T& initial)
		{
			return AggregateProcessor<T>(selector, initial);
		}

/***********************************************************************
All
***********************************************************************/

		template<typename T>
		class AllProcessor : public EnumerableProcessor<T, bool>
		{
		protected:
			Func<bool(T)>				selector;

			static bool Op(bool a, bool b)
			{
				return a && b;
			}
		public:
			AllProcessor(const Func<bool(T)>& _selector)
				:selector(_selector)
			{
			}

			bool operator()(const IEnumerable<T>& enumerable)const
			{
				return enumerable>>Select(selector)>>Aggregate(Op, true);
			}
		};

		template<typename T>
		AllProcessor<T> All(const Func<bool(T)>& selector)
		{
			return AllProcessor<T>(selector);
		}

		template<typename T>
		AllProcessor<T> All(bool(*selector)(T))
		{
			return AllProcessor<T>(selector);
		}

/***********************************************************************
Any
***********************************************************************/

		template<typename T>
		class AnyProcessor : public EnumerableProcessor<T, bool>
		{
		protected:
			Func<bool(T)>				selector;

			static bool Op(bool a, bool b)
			{
				return a || b;
			}
		public:
			AnyProcessor(const Func<bool(T)>& _selector)
				:selector(_selector)
			{
			}

			bool operator()(const IEnumerable<T>& enumerable)const
			{
				return enumerable>>Select(selector)>>Aggregate(Op, false);
			}
		};

		template<typename T>
		AnyProcessor<T> Any(const Func<bool(T)>& selector)
		{
			return AnyProcessor<T>(selector);
		}

		template<typename T>
		AnyProcessor<T> Any(bool(*selector)(T))
		{
			return AnyProcessor<T>(selector);
		}

/***********************************************************************
Max
***********************************************************************/

		class MaxProcessor : public AggregateEnumerableProcessor
		{
		protected:
			template<typename T>
			static T Op(T a, T b)
			{
				return a>b?a:b;
			}
		public:
			MaxProcessor()
			{
			}

			template<typename T>
			T operator()(const IEnumerable<T>& enumerable)const
			{
				return enumerable>>Aggregate(Op<T>);
			}
		};

		extern MaxProcessor Max();

/***********************************************************************
Min
***********************************************************************/

		class MinProcessor : public AggregateEnumerableProcessor
		{
		protected:
			template<typename T>
			static T Op(T a, T b)
			{
				return a<b?a:b;
			}
		public:
			MinProcessor()
			{
			}

			template<typename T>
			T operator()(const IEnumerable<T>& enumerable)const
			{
				return enumerable>>Aggregate(Op<T>);
			}
		};

		extern MinProcessor Min();

/***********************************************************************
First
***********************************************************************/

		class FirstProcessor : public FreeEnumerableProcessor
		{
		public:
			template<typename T>
			struct ResultTypeRetriver
			{
				typedef T ResultType;
			};

			FirstProcessor()
			{
			}

			template<typename T>
			T operator()(const IEnumerable<T>& enumerable)const
			{
				FOREACH(T, x, enumerable)
				{
					return x;
				}
				return T();
			}
		};

		extern FirstProcessor First();

/***********************************************************************
Count
***********************************************************************/

		class CountProcessor : public FreeEnumerableProcessor
		{
		public:
			template<typename T>
			struct ResultTypeRetriver
			{
				typedef vint ResultType;
			};

			CountProcessor()
			{
			}

			template<typename T>
			vint operator()(const IEnumerable<T>& enumerable)const
			{
				vint count=0;
				FOREACH(T, x, enumerable)
				{
					count++;
				}
				return count;
			}
		};

		extern CountProcessor Count();

/***********************************************************************
IsEmpty
***********************************************************************/

		class IsEmptyProcessor : public FreeEnumerableProcessor
		{
		public:
			template<typename T>
			struct ResultTypeRetriver
			{
				typedef bool ResultType;
			};

			IsEmptyProcessor()
			{
			}

			template<typename T>
			bool operator()(const IEnumerable<T>& enumerable)const
			{
				FOREACH(T, x, enumerable)
				{
					return false;
				}
				return true;
			}
		};

		extern IsEmptyProcessor IsEmpty();
	}
}

#endif