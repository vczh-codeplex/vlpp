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
			int				start;
			int				length;
			int				finalState;
		};

		class PureInterpretor : public Object
		{
		protected:
			int					charMap[1<<(8*sizeof(wchar_t))];	//char -> char set index
			int**				transition;							//(state * char set index) -> state*
			bool*				finalState;							//state -> bool
			int					stateCount;
			int					charSetCount;
			int					startState;
		public:
			PureInterpretor(Automaton::Ref dfa, CharRange::List& subsets);
			~PureInterpretor();

			bool				MatchHead(const wchar_t* input, const wchar_t* start, PureResult& result);
			bool				Match(const wchar_t* input, const wchar_t* start, PureResult& result);
		};
	}
}

#endif