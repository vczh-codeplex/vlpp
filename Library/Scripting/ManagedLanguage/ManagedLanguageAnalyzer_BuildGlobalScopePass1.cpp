#include "ManagedLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope1_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration)
		}
	}
}