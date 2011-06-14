/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator

Functions:
***********************************************************************/

#ifndef VCZH_COMBINATOR_TUPLEPROCESSOR
#define VCZH_COMBINATOR_TUPLEPROCESSOR

#include "Expression.h"
#include "TokenCombinator.h"

namespace vl
{
	namespace combinator
	{
		namespace x
		{
			struct Unused{};

			template<
				typename T1=Unused,
				typename T2=Unused,
				typename T3=Unused,
				typename T4=Unused,
				typename T5=Unused,
				typename T6=Unused,
				typename T7=Unused,
				typename T8=Unused,
				typename T9=Unused,
				typename T10=Unused,
				typename T11=Unused,
				typename T12=Unused,
				typename Tx=Unused
			>
			struct tp{};

			template<typename T>
			struct opt{};

			template<typename T>
			struct rep{};

/***********************************************************************
Element Converter
***********************************************************************/

			template<typename T>
			struct TypeConverter
			{
				typedef T ResultType;
			};

			template<typename T>
			struct TypeConverter<opt<T>>
			{
				typedef ParsingList<typename TypeConverter<T>::ResultType> ResultType;
			};

			template<typename T>
			struct TypeConverter<rep<T>>
			{
				typedef ParsingList<typename TypeConverter<T>::ResultType> ResultType;
			};

			template<
				typename T1,
				typename T2,
				typename T3,
				typename T4,
				typename T5,
				typename T6,
				typename T7,
				typename T8,
				typename T9,
				typename T10,
				typename T11,
				typename T12,
				typename Tx
			>
			struct TypeConverter<tp<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, Tx>>
			{
				typedef typename tp<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, Tx>::ResultType ResultType;
			};

/***********************************************************************
Reference Container
***********************************************************************/

			template<typename T>
			struct ReferenceContainer
			{
				T* reference;

				ReferenceContainer()
					:reference(0)
				{
				}

				ReferenceContainer(T& _reference)
				{
					reference=&_reference;
				}
			};

			template<typename T, typename U>
			struct ReferenceContainer<ParsingPair<T, U>>
			{
				typedef ParsingPair<ReferenceContainer<T>, ReferenceContainer<U>> ReferenceType;
				ReferenceType reference;

				ReferenceContainer()
				{
				}

				ReferenceContainer(ReferenceType _reference)
					:reference(_reference)
				{
				}
			};

			template<typename T>
			struct ReferenceTuple
			{
				ReferenceContainer<T> container;

				ReferenceTuple()
				{
				}

				ReferenceTuple(ReferenceContainer<T> _container)
					:container(_container)
				{
				}

				template<typename U>
				ReferenceTuple<ParsingPair<T, U>> ref(U& reference)
				{
					typedef ReferenceContainer<ParsingPair<T, U>> ResultType;
					return ResultType(typename ResultType::ReferenceType(container, ReferenceContainer<U>(reference)));
				}

				template<typename U>
				ReferenceTuple<ParsingPair<T, U>> skip()
				{
					typedef ReferenceContainer<ParsingPair<T, U>> ResultType;
					return ResultType(typename ResultType::ReferenceType(container, ReferenceContainer<U>()));
				}

				template<typename U>
				ReferenceTuple<ParsingPair<T, U>> ref(const ReferenceTuple<U>& tuple)
				{
					typedef ReferenceContainer<ParsingPair<T, U>> ResultType;
					return ResultType(typename ResultType::ReferenceType(container, tuple.container));
				}
			};

			template<typename T>
			ReferenceTuple<T> ref(T& reference)
			{
				return ReferenceContainer<T>(reference);
			};

			template<typename T>
			ReferenceTuple<T> skip()
			{
				return ReferenceContainer<T>();
			};

/***********************************************************************
Reference Filler
***********************************************************************/

			template<typename T>
			struct ReferenceFiller
			{
				static void Fill(const ReferenceContainer<T>& reference, const T& value)
				{
					if(reference.reference)
					{
						*reference.reference=value;
					}
				}
			};

			template<>
			struct ReferenceFiller<regex::RegexToken>
			{
				static void Fill(const ReferenceContainer<regex::RegexToken>& reference, const regex::RegexToken& value)
				{
					if(reference.reference)
					{
						*reference.reference=value;
					}
				}

				static void Fill(const ReferenceContainer<WString>& reference, const regex::RegexToken& value)
				{
					if(reference.reference)
					{
						*reference.reference=WString(value.reading, value.length);
					}
				}
			};

			template<typename T>
			struct ReferenceFiller<ParsingList<T>>
			{
				static void Fill(const ReferenceContainer<bool>& reference, const ParsingList<T>& value)
				{
					if(reference.reference)
					{
						*reference.reference=value.Head();
					}
				}

				template<typename U>
				static void Fill(const ReferenceContainer<U>& reference, const ParsingList<T>& value)
				{
					if(value.Head())
					{
						ReferenceFiller<T>::Fill(reference, value.Head()->Value());
					}
				}
				
				template<typename U>
				static void Fill(const ReferenceContainer<collections::List<U>>& reference, const ParsingList<T>& value)
				{
					if(reference.reference)
					{
						Ptr<ParsingList<T>::Node> current=value.Head();
						while(current)
						{
							U item;
							ReferenceContainer<U> itemReference(item);
							ReferenceFiller<T>::Fill(itemReference, current->Value());
							reference.reference->Add(item);
							current=current->Next();
						}
					}
				}
			};

			template<typename T, typename U>
			struct ReferenceFiller<ParsingPair<T, U>>
			{
				template<typename V, typename W>
				static void Fill(const ReferenceContainer<ParsingPair<V, W>>& reference, const ParsingPair<T, U>& value)
				{
					ReferenceFiller<T>::Fill(reference.reference.First(), value.First());
					ReferenceFiller<U>::Fill(reference.reference.Second(), value.Second());
				}
			};

			template<typename T, typename U>
			void Fill(const ReferenceTuple<T>& tuple, const U& value)
			{
				ReferenceFiller<U>::Fill(tuple.container, value);
			}

/***********************************************************************
Type Tuple Converter
***********************************************************************/

			template<typename T1, typename T2>
			struct tp<T1, T2>
			{
				typedef ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3>
			struct tp<T1, T2, T3>
			{
				typedef ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4>
			struct tp<T1, T2, T3, T4>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5>
			struct tp<T1, T2, T3, T4, T5>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			struct tp<T1, T2, T3, T4, T5, T6>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			struct tp<T1, T2, T3, T4, T5, T6, T7>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			struct tp<T1, T2, T3, T4, T5, T6, T7, T8>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType>,
					typename TypeConverter<T8>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			struct tp<T1, T2, T3, T4, T5, T6, T7, T8, T9>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType>,
					typename TypeConverter<T8>::ResultType>,
					typename TypeConverter<T9>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
			struct tp<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType>,
					typename TypeConverter<T8>::ResultType>,
					typename TypeConverter<T9>::ResultType>,
					typename TypeConverter<T10>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
			struct tp<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType>,
					typename TypeConverter<T8>::ResultType>,
					typename TypeConverter<T9>::ResultType>,
					typename TypeConverter<T10>::ResultType>,
					typename TypeConverter<T11>::ResultType
				> ResultType;
			};

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
			struct tp<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>
			{
				typedef ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
					typename TypeConverter<T1>::ResultType,
					typename TypeConverter<T2>::ResultType>,
					typename TypeConverter<T3>::ResultType>,
					typename TypeConverter<T4>::ResultType>,
					typename TypeConverter<T5>::ResultType>,
					typename TypeConverter<T6>::ResultType>,
					typename TypeConverter<T7>::ResultType>,
					typename TypeConverter<T8>::ResultType>,
					typename TypeConverter<T9>::ResultType>,
					typename TypeConverter<T10>::ResultType>,
					typename TypeConverter<T11>::ResultType>,
					typename TypeConverter<T12>::ResultType
				> ResultType;
			};
		}
	}
}

#endif