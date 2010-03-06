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
		extern Node<TokenInput<regex::RegexToken>, regex::RegexToken>	tk(int token);
		extern Node<TokenInput<regex::RegexToken>, regex::RegexToken>	tk(const WString& token);
	}
}

#endif