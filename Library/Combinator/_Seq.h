/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_SEQ
#define VCZH_COMBINATOR_SEQ

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O1, typename O2>
		class _Seq : public Combinator<I, ParsingPair<O1, O2>>
		{
		protected:
			typename Combinator<I, O1>::Ref		first;
			typename Combinator<I, O2>::Ref		second;
		public:
			_Seq(const typename Combinator<I, O1>::Ref& _first, const typename Combinator<I, O2>::Ref& _second)
				:first(_first)
				,second(_second)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				if(ParsingResult<O1> firstResult=first->Parse(input, globalInfo))
				{
					if(ParsingResult<O2> secondResult=second->Parse(input, globalInfo))
					{
						return ParsingResult<OutputType>(ParsingPair<O1, O2>(firstResult.Value(), secondResult.Value()));
					}
				}
				return ParsingResult<OutputType>();
			}
		};

		template<typename I, typename O1, typename O2>
		class _Left : public Combinator<I, O1>
		{
		protected:
			typename Combinator<I, O1>::Ref		first;
			typename Combinator<I, O2>::Ref		second;
		public:
			_Left(const typename Combinator<I, O1>::Ref& _first, const typename Combinator<I, O2>::Ref& _second)
				:first(_first)
				,second(_second)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				if(ParsingResult<O1> firstResult=first->Parse(input, globalInfo))
				{
					if(ParsingResult<O2> secondResult=second->Parse(input, globalInfo))
					{
						return firstResult;
					}
				}
				return ParsingResult<OutputType>();
			}
		};

		template<typename I, typename O1, typename O2>
		class _Right : public Combinator<I, O2>
		{
		protected:
			typename Combinator<I, O1>::Ref		first;
			typename Combinator<I, O2>::Ref		second;
		public:
			_Right(const typename Combinator<I, O1>::Ref& _first, const typename Combinator<I, O2>::Ref& _second)
				:first(_first)
				,second(_second)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				if(ParsingResult<O1> firstResult=first->Parse(input, globalInfo))
				{
					if(ParsingResult<O2> secondResult=second->Parse(input, globalInfo))
					{
						return secondResult;
					}
				}
				return ParsingResult<OutputType>();
			}
		};
	}
}

#endif