/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Combinator::Combinator

Functions:
	tch					�����ַ�ƥ��
	tchs				�����ַ�ѡ����ƥ��
	tstr				���ַ�����ȷƥ��
	trgx				��������ʽƥ��
***********************************************************************/

#ifndef VCZH_COMBINATOR_TOKENCOMBINATOR
#define VCZH_COMBINATOR_TOKENCOMBINATOR

#include "Combinator.h"
#include "..\Regex\Regex.h"

namespace vl
{
	namespace combinator
	{
		extern Node<EnumerableInput<regex::RegexToken>, wchar_t>			tch(wchar_t c);
		extern Node<EnumerableInput<regex::RegexToken>, wchar_t>			tchs(const WString& chars);
		extern Node<EnumerableInput<regex::RegexToken>, WString>			tstr(const WString& string);
		extern Node<EnumerableInput<regex::RegexToken>, WString>			trgx(const WString& code);
	}
}

#endif