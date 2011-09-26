#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			void ManagedLanguage_BuildGlobalScope3_GenericParameter(ManagedLanguageElement* languageElement, ManagedSymbolItem* symbol, ManagedSymbolItem* scopeItem, const List<WString>& orderedGenericParameterNames, const MAP& argument)
			{
				FOREACH(WString, name, orderedGenericParameterNames.Wrap())
				{
					ManagedSymbolGenericParameter* genericParameter=dynamic_cast<ManagedSymbolGenericParameter*>(symbol->ItemGroup(name)->Items()[0]);
					FOREACH(ManagedTypeSymbol*, typeConstraint, genericParameter->typeConstraints.Wrap())
					{
						CheckType(languageElement, typeConstraint, scopeItem, scopeItem, argument);
						if(GetRealSymbol(typeConstraint->GetSymbol())->GetSymbolType()!=ManagedSymbolItem::Interface)
						{
							argument.errors.Add(ManagedLanguageCodeException::GetIllegalTypeConstraint(languageElement, genericParameter->GetName()));
						}
					}
				}
			}

/***********************************************************************
ManagedLanguage_BuildGlobalScope3_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					ManagedSymbolField* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolField>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					CheckType(node, symbol->type, scopeItem, symbol, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					CheckType(node, symbol->returnType, scopeItem, symbol, argument);
					FOREACH(WString, name, symbol->orderedMethodParameterNames.Wrap())
					{
						ManagedSymbolMethodParameter* methodParameter=dynamic_cast<ManagedSymbolMethodParameter*>(symbol->ItemGroup(name)->Items()[0]);
						CheckType(node, methodParameter->type, scopeItem, symbol, argument);
					}
					ManagedLanguage_BuildGlobalScope3_GenericParameter(node, symbol, scopeItem, symbol->orderedGenericParameterNames, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					FOREACH(WString, name, symbol->orderedMethodParameterNames.Wrap())
					{
						ManagedSymbolMethodParameter* methodParameter=dynamic_cast<ManagedSymbolMethodParameter*>(symbol->ItemGroup(name)->Items()[0]);
						CheckType(node, methodParameter->type, scopeItem, symbol, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedLanguage_BuildGlobalScope3_Declaration(node->declaration, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope3_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope3_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
					ManagedSymbolProperty* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolProperty>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					CheckType(node, symbol->type, scopeItem, symbol, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					ManagedSymbolConverterOperator* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConverterOperator>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					CheckType(node, symbol->targetType, scopeItem, symbol, argument);
					ManagedLanguage_BuildGlobalScope3_GenericParameter(node, symbol, scopeItem, symbol->orderedGenericParameterNames, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope3_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					ManagedSymbolItem* scopeItem=symbol;
					FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
					{
						CheckType(node, baseType, scopeItem, symbol, argument);
					}
					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope3_Member(member, argument);
					}
					ManagedLanguage_BuildGlobalScope3_GenericParameter(node, symbol, scopeItem, symbol->orderedGenericParameterNames, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope3_Declaration(declaration, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					ManagedSymbolItem* scopeItem=symbol;
					FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
					{
						CheckType(node, baseType, scopeItem, symbol, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					ManagedSymbolTypeRename* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolTypeRename>(node);
					ManagedSymbolItem* scopeItem=symbol->GetParentItem();
					switch(scopeItem->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						break;
					default:
						scopeItem=0;
					}
					CheckType(node, symbol->type, scopeItem, symbol, argument);
					ManagedLanguage_BuildGlobalScope3_GenericParameter(node, symbol, scopeItem, symbol->orderedGenericParameterNames, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration)
		}
	}
}