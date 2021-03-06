#include "ManagedLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedType, MANAGED_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExtendedType, MANAGED_EXTENDED_TYPE_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExpression, MANAGED_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExtendedExpression, MANAGED_EXTENDED_EXPRESSION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedStatement, MANAGED_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExtendedStatement, MANAGED_EXTENDED_STATEMENT_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedMember, MANAGED_MEMBER_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExtendedMember, MANAGED_EXTENDED_MEMBER_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedDeclaration, MANAGED_DECLARATION_TARGETS)
			DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ManagedExtendedDeclaration, MANAGED_EXTENDED_DECLARATION_TARGETS)

			ManagedPosition::ManagedPosition()
				:start(0)
				,lineStart(0)
				,lineIndex(0)
				,codeIndex(0)
			{
			}

/***********************************************************************
ManagedGenericInfo
***********************************************************************/

			bool ManagedGenericInfo::HasGeneric()
			{
				return arguments.Count()>0;
			}
		}
	}
}