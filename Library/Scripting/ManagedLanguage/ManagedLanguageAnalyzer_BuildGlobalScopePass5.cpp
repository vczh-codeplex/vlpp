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
ManagedLanguage_BuildGlobalScope5_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					if(node->initializer)
					{
						// TODO: check constant/readonly
						ManagedSymbolField* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolField>(node);
						MAP newArgument(argument, symbol);
						GetType(node->initializer.Obj(), symbol->type, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					if(node->body)
					{
						ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
						argument.contextManager->PushFunction(symbol->returnType, node);
						MAP newArgument(argument, symbol);
						BuildLocalScope(node->body.Obj(), newArgument);
						argument.contextManager->PopStatement();
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					argument.contextManager->PushFunction(argument.contextManager->predefinedTypes.voidType, node);
					MAP newArgument(argument, symbol);
					BuildLocalScope(node->body.Obj(), newArgument);
					argument.contextManager->PopStatement();
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedLanguage_BuildGlobalScope5_Declaration(node->declaration, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope5_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope5_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedMember, ManagedExtendedMember, MAP)

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
						argument.contextManager->PushFunction(symbol->targetType, node);
						MAP newArgument(argument, symbol);
						BuildLocalScope(node->body.Obj(), newArgument);
						argument.contextManager->PopStatement();
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope5_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope5_Member(member, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope5_Declaration(declaration, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope5_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope5_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedDeclaration)
		}
	}
}