/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_ALT
#define VCZH_COMBINATOR_ALT

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O>
		class _Alt : public Combinator<I, O>
		{
		protected:
			typename Combinator<I, O>::Ref		first;
			typename Combinator<I, O>::Ref		second;
		public:
			_Alt(const typename Combinator<I, O>::Ref& _first, const typename Combinator<I, O>::Ref& _second)
				:first(_first)
				,second(_second)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				GlobalInfoType firstInfo;
				InputType firstInput=input;
				if(ParsingResult<OutputType> firstResult=first->Parse(firstInput, firstInfo))
				{
					input=firstInput;
					globalInfo.Append(firstInfo);
					return firstResult;
				}

				GlobalInfoType secondInfo;
				InputType secondInput=input;
				if(ParsingResult<OutputType> secondResult=second->Parse(secondInput, secondInfo))
				{
					input=secondInput;
					globalInfo.Append(secondInfo);
					return secondResult;
				}

				if(firstInput>=secondInput)
				{
					input=firstInput;
					globalInfo.Append(firstInfo);
				}
				else
				{
					input=secondInput;
					globalInfo.Append(secondInfo);
				}
				return ParsingResult<OutputType>();
			}
		};
	}
}

#endif