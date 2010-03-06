/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Combinator::Combinator

Functions:
	tch					：单字符匹配
	tchs				：多字符选择性匹配
	tstr				：字符串精确匹配
	trgx				：正则表达式匹配
***********************************************************************/

#ifndef VCZH_COMBINATOR_TOKENCOMBINATOR
#define VCZH_COMBINATOR_TOKENCOMBINATOR

#include "Combinator.h"
#include "..\Regex\Regex.h"

namespace vl
{
	namespace combinator
	{
		extern Node<TokenInput<regex::RegexToken>, regex::RegexToken>	tk(int token);
		extern Node<TokenInput<regex::RegexToken>, regex::RegexToken>	tk(const WString& token);
	}
}

#endif