/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONSELECT
#define VCZH_COLLECTIONS_OPERATIONSELECT

#include "OperationEnumerable.h"
#include "..\Function.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Select
***********************************************************************/

		template<typename T, typename K>
		class SelectEnumerable : public EnumerableStore<T>, public virtual IEnumerable<K>
		{
		private:
			class Enumerator : public virtual IEnumerator<K>
			{
			protected:
				IEnumerator<T>*		enumerator;
				Func<K(T)>			selector;
				K					current;
			public:
				Enumerator(IEnumerator<T>* _enumerator, const Func<K(T)>& _selector)
					:enumerator(_enumerator)
					,selector(_selector)
				{
					if(enumerator->Available())
					{
						current=selector(enumerator->Current());
					}
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<K>* Clone()const
				{
					return new Enumerator(enumerator->Clone(), selector);
				}

				const K& Current()const
				{
					return current;
				}

				vint Index()const
				{
					return enumerator->Index();
				}

				bool Next()
				{
					if(enumerator->Next())
					{
						current=selector(enumerator->Current());
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Available()const
				{
					return enumerator->Available();
				}

				void Reset()
				{
					enumerator->Reset();
				}
			};
		protected:
			Func<K(T)>				selector;
		public:
			SelectEnumerable(const IEnumerable<T>& enumerable, const Func<K(T)>& _selector)
				:EnumerableStore<T>(enumerable)
				,selector(_selector)
			{
			}

			IEnumerator<K>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), selector);
			}
		};

		template<typename T, typename K>
		class SelectProcessor : public EnumerableProcessor<T, SelectEnumerable<T, K>>
		{
		protected:
			Func<K(T)>				selector;
		public:
			SelectProcessor(const Func<K(T)>& _selector)
				:selector(_selector)
			{
			}

			SelectEnumerable<T, K> operator()(const IEnumerable<T>& enumerable)const
			{
				return SelectEnumerable<T, K>(enumerable, selector);
			}
		};

		template<typename T, typename K>
		SelectProcessor<T, K> Select(const Func<K(T)>& selector)
		{
			return SelectProcessor<T, K>(selector);
		}

		template<typename T, typename K>
		SelectProcessor<T, K> Select(K(*selector)(T))
		{
			return SelectProcessor<T, K>(selector);
		}
	}
}

#endif