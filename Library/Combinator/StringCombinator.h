/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Combinator::Combinator

Functions:
	ch					：单字符匹配
	chs					：多字符选择性匹配
	str					：字符串精确匹配
	rgx					：正则表达式匹配
***********************************************************************/

#ifndef VCZH_COMBINATOR_STRINGCOMBINATOR
#define VCZH_COMBINATOR_STRINGCOMBINATOR

#include "Combinator.h"

namespace vl
{
	namespace combinator
	{
		extern Node<StringInput<wchar_t>, wchar_t>		ch(wchar_t c);
		extern Node<StringInput<wchar_t>, wchar_t>		chs(const WString& chars);
		extern Node<StringInput<wchar_t>, WString>		str(const WString& string);
		extern Node<StringInput<wchar_t>, WString>		rgx(const WString& code);
	}
}

#endif