#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedLanguage_GetTypeSymbol_Type
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedReferencedType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedMemberType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInstantiatedGenericType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedExtendedType)
				{
					return ManagedLanguage_GetTypeSymbol_ExtendedType(node, argument);
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_Type)

/***********************************************************************
ManagedLanguage_GetTypeSymbol_ExtendedType
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedArrayType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFunctionType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedEventType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedAutoReferType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedDynamicType)
				{
					return 0;
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member, ManagedMember, MAP)

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
					ManagedLanguage_BuildGlobalScope2_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration)
		}
	}
}