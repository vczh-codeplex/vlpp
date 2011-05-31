/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Combinator::Combinator Library

Classes:
***********************************************************************/

#ifndef VCZH_COMBINATOR_DEF
#define VCZH_COMBINATOR_DEF

#include "Expression.h"

namespace vl
{
	namespace combinator
	{
		template<typename I, typename O, typename V>
		class _Let : public Combinator<I, V>
		{
		protected:
			typename Combinator<I, O>::Ref		pattern;
			V									value;
		public:
			_Let(const typename Combinator<I, O>::Ref& _pattern, const V& _value)
				:pattern(_pattern)
				,value(_value)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				InputType errorInput=input;
				ParsingResult<O> result=pattern->Parse(input, globalInfo);
				return result?ParsingResult<OutputType>(value):ParsingResult<OutputType>();
			}
		};

		template<typename I, typename O>
		class _Def : public Combinator<I, O>
		{
		protected:
			typename Combinator<I, O>::Ref		pattern;
			O									value;
		public:
			_Def(const typename Combinator<I, O>::Ref& _pattern, const O& _value)
				:pattern(_pattern)
				,value(_value)
			{
			}

			ParsingResult<OutputType> Parse(InputType& input, GlobalInfoType& globalInfo)const
			{
				InputType errorInput=input;
				ParsingResult<O> result=pattern->Parse(input, globalInfo);
				if(result)
				{
					return result;
				}
				else
				{
					globalInfo.errors.Clear();
					return ParsingResult<OutputType>(value);
				}
			}
		};
	}
}

#endif