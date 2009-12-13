/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Combinator::Combinator

Functions:
	ch					�����ַ�ƥ��
	chs					�����ַ�ѡ����ƥ��
	str					���ַ�����ȷƥ��
	rgx					��������ʽƥ��
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