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

			BasicPosition::BasicPosition()
				:start(0)
				,lineStart(0)
				,lineIndex(0)
			{
			}

			BasicBinaryExpression::BasicBinaryExpression()
			{
				shortcut=true;
			}
		}
	}
}