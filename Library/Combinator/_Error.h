/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_ERROR
#define VCZH_COMBINATOR_ERROR

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O>
		class _Error : public Combinator<I, O>
		{
		protected:
			typename Combinator<I, O>::Ref							element;
			Func<ParsingResult<OutputType>(I&, GlobalInfoType&)>	recoverer;
		public:
			_Error(const typename Combinator<I, O>::Ref& _element, const Func<ParsingResult<OutputType>(I&, GlobalInfoType&)>& _recoverer)
				:element(_element)
				,recoverer(_recoverer)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				InputType& elementInput=input;
				GlobalInfoType elementGlobalInfo;
				ParsingResult<OutputType> result=element->Parse(elementInput, elementGlobalInfo);
				if(!result)
				{
					result=recoverer(input, elementGlobalInfo);
				}
				globalInfo.Append(elementGlobalInfo);
				return result;
			}
		};
	}
}

#endif