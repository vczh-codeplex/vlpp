#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(BASIC_DECLARATION_TARGETS)

			BasicBinaryExpression::BasicBinaryExpression()
			{
				shortcut=true;
			}
		}
	}
}