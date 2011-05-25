#include "ManagedLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_EXTENDED_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_EXTENDED_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_EXTENDED_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_DECLARATION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(MANAGED_EXTENDED_DECLARATION_TARGETS)

			ManagedPosition::ManagedPosition()
				:start(0)
				,lineStart(0)
				,lineIndex(0)
				,codeIndex(0)
			{
			}
		}
	}
}