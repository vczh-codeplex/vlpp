/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONSET
#define VCZH_COLLECTIONS_OPERATIONSET

#include "OperationEnumerable.h"
#include "OperationConcat.h"
#include "OperationSequence.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Interset/Except
***********************************************************************/

		template<typename T, bool interset>
		class IntersetExceptEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		protected:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*			enumerator;
				SortedList<T>			reference;
				int						index;

				void GoNearest()
				{
					while(enumerator->Available())
					{
						if(reference.Contains(enumerator->Current())==interset)
						{
							break;
						}
						else
						{
							enumerator->Next();
						}
					}
				}
			public:
				Enumerator(IEnumerator<T>* _enumerator, const IEnumerable<T>& _reference)
					:enumerator(_enumerator)
					,index(0)
				{
					CopyFrom(reference.Wrap(), _reference);
					GoNearest();
				}

				Enumerator(const Enumerator& _enumerator)
				{
					enumerator=_enumerator.enumerator->Clone();
					CopyFrom(reference.Wrap(), _enumerator.reference.Wrap());
					index=_enumerator.index;
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(*this);
				}

				const T& Current()const
				{
					return enumerator->Current();
				}

				int Index()const
				{
					return index;
				}

				bool Next()
				{
					index++;
					enumerator->Next();
					GoNearest();
					return Available();
				}

				bool Available()const
				{
					return enumerator->Available();
				}

				void Reset()
				{
					enumerator->Reset();
					index=0;
				}
			};
		protected:
			const IEnumerable<T>&				reference;
		public:
			IntersetExceptEnumerable(const IEnumerable<T>& enumerable, const IEnumerable<T>& _reference)
				:EnumerableStore<T>(enumerable)
				,reference(_reference)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), reference);
			}
		};

		template<typename T, bool interset>
		class IntersetExceptProcessor : public EnumerableProcessor<T, IntersetExceptEnumerable<T, interset>>
		{
		protected:
			const IEnumerable<T>&				second;
		public:
			IntersetExceptProcessor(const IEnumerable<T>& _second)
				:second(_second)
			{
			}

			IntersetExceptEnumerable<T, interset> operator()(const IEnumerable<T>& first)const
			{
				return IntersetExceptEnumerable<T, interset>(first, second);
			}
		};

		template<typename T>
		IntersetExceptProcessor<T, true> Interset(const IEnumerable<T>& second)
		{
			return IntersetExceptProcessor<T, true>(second);
		}

		template<typename T>
		IntersetExceptProcessor<T, false> Except(const IEnumerable<T>& second)
		{
			return IntersetExceptProcessor<T, false>(second);
		}

/***********************************************************************
Union
***********************************************************************/

		template<typename T>
		class UnionProcessor : public EnumerableProcessor<T, DistinctEnumerable<T>>
		{
		protected:
			const IEnumerable<T>&				second;
		public:
			UnionProcessor(const IEnumerable<T>& _second)
				:second(_second)
			{
			}

			DistinctEnumerable<T> operator()(const IEnumerable<T>& first)const
			{
				return first>>Concat(second)>>Distinct();
			}
		};

		template<typename T>
		UnionProcessor<T> Union(const IEnumerable<T>& second)
		{
			return UnionProcessor<T>(second);
		}
	}
}

#endif