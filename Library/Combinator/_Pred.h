/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_PRED
#define VCZH_COMBINATOR_PRED

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I>
		class _Pred : public Combinator<I, typename I::ElementType>
		{
			typedef typename I::ElementType		ElementType;
		protected:
			Func<bool(const typename I::ElementType&)>	predicate;
			WString										errorMessage;
		public:
			_Pred(const Func<bool(const typename I::ElementType&)>& _predicate, const WString _errorMessage=WString::Empty)
				:predicate(_predicate)
				,errorMessage(_errorMessage)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				if(input.Available() && predicate(input.Current()))
				{
					ParsingResult<OutputType> result(input.Current());
					input.Next();
					return result;
				}
				else
				{
					globalInfo.errors.Add(new ErrorType(errorMessage, input));
					return ParsingResult<OutputType>();
				}
			}
		};
	}
}

#endif