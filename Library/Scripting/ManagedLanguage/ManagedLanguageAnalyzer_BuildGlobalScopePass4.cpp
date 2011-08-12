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
ManagedLanguage_BuildGlobalScope4_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					if(node->body)
					{
						ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
						MAP newArgument(argument, symbol);
						BuildLocalScope(node->body.Obj(), newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					MAP newArgument(argument, symbol);
					BuildLocalScope(node->body.Obj(), newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope4_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope4_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
					ManagedSymbolProperty* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolProperty>(node);
					MAP newArgument(argument, symbol);
					if(node->getter)
					{
						BuildLocalScope(node->getter.Obj(), newArgument);
					}
					if(node->setter)
					{
						BuildLocalScope(node->setter.Obj(), newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					if(node->body)
					{
						ManagedSymbolConverterOperator* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConverterOperator>(node);
						MAP newArgument(argument, symbol);
						BuildLocalScope(node->body.Obj(), newArgument);
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope4_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope4_Member(member, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope4_Declaration(declaration, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope4_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope4_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedDeclaration)
		}
	}
}