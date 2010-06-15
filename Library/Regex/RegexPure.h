/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Regex::RegexInterpretor

Classes:
	PureInterpretor					：正则表达式纯模拟器
***********************************************************************/

#ifndef VCZH_REGEX_REGEXPURE
#define VCZH_REGEX_REGEXPURE

#include "RegexAutomaton.h"

namespace vl
{
	namespace regex_internal
	{
		class PureResult
		{
		public:
			vint				start;
			vint				length;
			vint				finalState;
		};

		class PureInterpretor : public Object
		{
		protected:
			vint					charMap[1<<(8*sizeof(wchar_t))];	//char -> char set index
			vint**				transition;							//(state * char set index) -> state*
			bool*				finalState;							//state -> bool
			vint					stateCount;
			vint					charSetCount;
			vint					startState;
		public:
			PureInterpretor(Automaton::Ref dfa, CharRange::List& subsets);
			~PureInterpretor();

			bool				MatchHead(const wchar_t* input, const wchar_t* start, PureResult& result);
			bool				Match(const wchar_t* input, const wchar_t* start, PureResult& result);
		};
	}
}

#endif