/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_LOOP
#define VCZH_COMBINATOR_LOOP

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O>
		class _Loop : public Combinator<I, ParsingList<O>>
		{
		protected:
			typename Combinator<I, O>::Ref		element;
			int									min;
			int									max;
		public:
			_Loop(const typename Combinator<I, O>::Ref _element, int _min, int _max=-1)
				:element(_element)
				,min(_min)
				,max(_max)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				ParsingList<O> result;
				InputType elementInput=input;
				GlobalInfoType elementInfo;

				while(max==-1 || result.Count()<max)
				{
					ParsingResult<O> elementResult=element->Parse(elementInput, elementInfo);
					if(elementResult)
					{
						result.Append(elementResult.Value());
						input=elementInput;
						globalInfo.Append(elementInfo);
						elementInfo.errors.Clear();
					}
					else
					{
						break;
					}
				}

				if(result.Count()>=min)
				{
					globalInfo.CandidateAppend(elementInfo);
					return ParsingResult<OutputType>(result);
				}
				else
				{
					input=elementInput;
					globalInfo.Append(elementInfo);
					return ParsingResult<OutputType>();
				}
			}
		};
	}
}

#endif