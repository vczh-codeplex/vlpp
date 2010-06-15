/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONCONCAT
#define VCZH_COLLECTIONS_OPERATIONCONCAT

#include "OperationEnumerable.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Concat
***********************************************************************/

		template<typename T>
		class ConcatEnumerable : public EnumerableStore<T, 1>, public EnumerableStore<T, 2>, public virtual IEnumerable<T>
		{
		protected:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*					enumerator1;
				IEnumerator<T>*					enumerator2;
				vint								index;
				bool							turned;
			public:
				Enumerator(IEnumerator<T>* _enumerator1, IEnumerator<T>* _enumerator2, vint _index=0, bool _turned=false)
					:enumerator1(_enumerator1)
					,enumerator2(_enumerator2)
					,index(_index)
					,turned(_turned)
				{
				}

				~Enumerator()
				{
					delete enumerator1;
					delete enumerator2;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(enumerator1->Clone(), enumerator2->Clone(), index, turned);
				}

				const T& Current()const
				{
					if(enumerator1->Available())
					{
						return enumerator1->Current();
					}
					else
					{
						return enumerator2->Current();
					}
				}

				vint Index()const
				{
					return index;
				}

				bool Next()
				{
					index++;
					if(enumerator1->Next())
					{
						return true;
					}
					else if(turned==false)
					{
						turned=true;
						return enumerator2->Available();
					}
					else
					{
						return enumerator2->Next();
					}
				}

				bool Available()const
				{
					return enumerator1->Available() || enumerator2->Available();
				}

				void Reset()
				{
					enumerator1->Reset();
					enumerator2->Reset();
					index=0;
				}
			};
		public:
			ConcatEnumerable(const IEnumerable<T>& enumerable1, const IEnumerable<T>& enumerable2)
				:EnumerableStore<T, 1>(enumerable1)
				,EnumerableStore<T, 2>(enumerable2)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(EnumerableStore<T, 1>::CopyEnumerator(), EnumerableStore<T, 2>::CopyEnumerator());
			}
		};

		template<typename T>
		class ConcatProcessor : public EnumerableProcessor<T, ConcatEnumerable<T>>
		{
		protected:
			const IEnumerable<T>&				second;
		public:
			ConcatProcessor(const IEnumerable<T>& _second)
				:second(_second)
			{
			}

			ConcatEnumerable<T> operator()(const IEnumerable<T>& first)const
			{
				return ConcatEnumerable<T>(first, second);
			}
		};

		template<typename T>
		ConcatProcessor<T> Concat(const IEnumerable<T>& second)
		{
			return ConcatProcessor<T>(second);
		}
	}
}

#endif