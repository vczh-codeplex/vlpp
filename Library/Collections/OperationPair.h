/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONPAIR
#define VCZH_COLLECTIONS_OPERATIONPAIR

#include "OperationEnumerable.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Pairwise
***********************************************************************/

		template<typename S, typename T>
		class PairwiseEnumerable : public EnumerableStore<S, 1>, public EnumerableStore<T, 2>, public virtual IEnumerable<Pair<S, T>>
		{
		protected:
			class Enumerator : public virtual IEnumerator<Pair<S, T>>
			{
			protected:
				IEnumerator<S>*					enumerator1;
				IEnumerator<T>*					enumerator2;
				Pair<S, T>						current;
			public:
				Enumerator(IEnumerator<S>* _enumerator1, IEnumerator<T>* _enumerator2, vint _index=0, bool _turned=false)
					:enumerator1(_enumerator1)
					,enumerator2(_enumerator2)
				{
					if(Available())
					{
						current=Pair<S, T>(enumerator1->Current(), enumerator2->Current());
					}
				}

				~Enumerator()
				{
					delete enumerator1;
					delete enumerator2;
				}

				IEnumerator<Pair<S, T>>* Clone()const
				{
					return new Enumerator(enumerator1->Clone(), enumerator2->Clone());
				}

				const Pair<S, T>& Current()const
				{
					return current;
				}

				vint Index()const
				{
					return enumerator1->Index();
				}

				bool Next()
				{
					if(enumerator1->Next() && enumerator2->Next())
					{
						current=Pair<S, T>(enumerator1->Current(), enumerator2->Current());
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Available()const
				{
					return enumerator1->Available() && enumerator2->Available();
				}

				void Reset()
				{
					enumerator1->Reset();
					enumerator2->Reset();
					if(Available())
					{
						current=Pair<S, T>(enumerator1->Current(), enumerator2->Current());
					}
				}
			};
		public:
			PairwiseEnumerable(const IEnumerable<S>& enumerable1, const IEnumerable<T>& enumerable2)
				:EnumerableStore<S, 1>(enumerable1)
				,EnumerableStore<T, 2>(enumerable2)
			{
			}

			IEnumerator<Pair<S, T>>* CreateEnumerator()const
			{
				return new Enumerator(EnumerableStore<S, 1>::CopyEnumerator(), EnumerableStore<T, 2>::CopyEnumerator());
			}
		};

		template<typename T>
		class PairwiseProcessor : public FreeEnumerableProcessor
		{
		protected:
			const IEnumerable<T>&				second;
		public:
			template<typename S>
			struct ResultTypeRetriver
			{
				typedef PairwiseEnumerable<S, T> ResultType;
			};

			PairwiseProcessor(const IEnumerable<T>& _second)
				:second(_second)
			{
			}

			template<typename S>
			PairwiseEnumerable<S, T> operator()(const IEnumerable<S>& first)const
			{
				return PairwiseEnumerable<S, T>(first, second);
			}
		};

		template<typename T>
		PairwiseProcessor<T> Pairwise(const IEnumerable<T>& second)
		{
			return PairwiseProcessor<T>(second);
		}
	}
}

#endif