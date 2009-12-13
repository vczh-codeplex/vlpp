/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_UNIT
#define VCZH_COMBINATOR_UNIT

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I>
		class _Unit : public Combinator<I, typename I::ElementType>
		{
			typedef typename I::ElementType		ElementType;
		protected:
			ElementType			baseline;
			WString				errorMessage;
		public:
			_Unit(const ElementType& _baseline, const WString _errorMessage=WString::Empty)
				:baseline(_baseline)
				,errorMessage(_errorMessage)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				if(input.Available() && input.Current()==baseline)
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