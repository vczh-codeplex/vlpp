/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_USING
#define VCZH_COMBINATOR_USING

#include "Expression.h"
#include "..\Function.h"

namespace vl
{
	namespace combinator
	{

		template<typename I, typename O1, typename O2>
		class _Using : public Combinator<I, O2>
		{
		protected:
			typename Combinator<I, O1>::Ref			element;
			Func<O2(const O1&)>						converter;
		public:
			_Using(const typename Combinator<I, O1>::Ref& _element, const Func<O2(const O1&)>& _converter)
				:element(_element)
				,converter(_converter)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				InputType errorInput=input;
				ParsingResult<O1> result=element->Parse(input, globalInfo);
				if(result)
				{
					try
					{
						return ParsingResult<OutputType>(converter(result.Value()));
					}
					catch(const CombinatorResultError<O2>& error)
					{
						globalInfo.errors.Add(new CombinatorError<I>(error.Message(), errorInput));
						return error.Result();
					}
				}
				else
				{
					return ParsingResult<OutputType>();
				}
			}
		};
	}
}

#endif