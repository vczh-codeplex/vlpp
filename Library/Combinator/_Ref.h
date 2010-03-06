/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_REF
#define VCZH_COMBINATOR_REF

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O>
		class _Ref : public Combinator<I, O>
		{
		protected:
			const typename Combinator<I, O>::Ref&		element;
		public:
			_Ref(const typename Combinator<I, O>::Ref& _element)
				:element(_element)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				return element->Parse(input, globalInfo);
			}
		};
	}
}

#endif