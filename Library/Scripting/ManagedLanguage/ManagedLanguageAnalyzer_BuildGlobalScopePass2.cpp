#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;
			
			void ManagedLanguage_BuildGlobalScope2_GenericParameter(ManagedSymbolItem* symbol, ManagedGenericInfo* genericInfo, const MAP& argument)
			{
				FOREACH(Ptr<ManagedGenericInfo::Argument>, genericParameter, genericInfo->arguments.Wrap())
				{
					ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(genericParameter->name);
					CHECK_ERROR(
						group && group->Items().Count()==1 && group->Items()[0]->GetSymbolType()==ManagedSymbolItem::GenericParameter,
						L"ManagedLanguage_BuildGlobalScope2_GenericParameter(ManagedSymbolItem*, ManagedGenericInfo*, const MAP&)#BuildGlobalScope1可能出现问题。"
						);
					ManagedSymbolGenericParameter* symbol=dynamic_cast<ManagedSymbolGenericParameter*>(group->Items()[0]);
					symbol->conversion=genericParameter->conversion;
					symbol->newConstraint=genericParameter->newConstraint;

					FOREACH(Ptr<ManagedType>, constraint, genericParameter->typeConstraints.Wrap())
					{
						ManagedTypeSymbol* type=GetTypeSymbol(constraint, argument);
						if(type)
						{
							symbol->typeConstraints.Add(type);
						}
					}
				}
			}

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					ManagedSymbolField* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolField>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
					symbol->returnType=GetTypeSymbol(node->returnType, argument);
					if(node->implementedInterfaceType)
					{
						symbol->implementedInterfaceType=GetTypeSymbol(node->implementedInterfaceType, argument);
					}
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, &node->genericInfo, MAP(argument, symbol));

					FOREACH(Ptr<ManagedParameter>, parameter, node->parameters.Wrap())
					{
						ManagedSymbolMethodParameter* parameterSymbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethodParameter>(parameter.Obj());

						parameterSymbol->parameterType=parameter->parameterType;
						parameterSymbol->containsDefaultValue=parameter->defaultValue;
						parameterSymbol->type=GetTypeSymbol(parameter->type, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					symbol->implicit=node->implicit;

					FOREACH(Ptr<ManagedParameter>, parameter, node->parameters.Wrap())
					{
						ManagedSymbolMethodParameter* parameterSymbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethodParameter>(parameter.Obj());

						parameterSymbol->parameterType=parameter->parameterType;
						parameterSymbol->containsDefaultValue=parameter->defaultValue;
						parameterSymbol->type=GetTypeSymbol(parameter->type, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedLanguage_BuildGlobalScope2_Declaration(node->declaration, argument);
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
					ManagedSymbolProperty* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolProperty>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
					if(node->implementedInterfaceType)
					{
						symbol->implementedInterfaceType=GetTypeSymbol(node->implementedInterfaceType, argument);
					}
					symbol->containsGetter=node->getter;
					symbol->containsSetter=node->setter;
					if(symbol->containsSetter)
					{
						ManagedSymbolPropertySetterValue* setterValue=new ManagedSymbolPropertySetterValue(argument.symbolManager);
						setterValue->SetName(node->setterParameter);
						setterValue->associatedProperty=symbol;
						symbol->Add(setterValue);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					ManagedSymbolConverterOperator* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConverterOperator>(node);
					symbol->implicit=node->implicit;
					symbol->targetType=GetTypeSymbol(node->targetType, argument);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, &node->genericInfo, MAP(argument, symbol));
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					MAP newArgument(argument, symbol);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, &node->genericInfo, newArgument);
					EnsureSymbolBaseTypesCompleted(symbol, argument);

					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope2_Member(member, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					MAP newArgument(argument, argument.symbolManager->GetSymbol(node));
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope2_Declaration(declaration, newArgument);
					}
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
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					EnsureSymbolBaseTypesCompleted(symbol, argument);

					FOREACH(Ptr<ManagedEnumItem>, enumItem, node->items.Wrap())
					{
						ManagedSymbolField* field=dynamic_cast<ManagedSymbolField*>(symbol->ItemGroup(enumItem->name)->Items()[0]);

						field->type=argument.symbolManager->GetType(symbol);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					ManagedSymbolTypeRename* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolTypeRename>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, &node->genericInfo, MAP(argument, symbol));
					if(symbol->type->GetSymbol()->GetSymbolType()==ManagedSymbolItem::TypeRename)
					{
						argument.errors.Add(ManagedLanguageCodeException::GetCannotDirectReferTypeRenameAsToplevel(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
					ManagedSymbolUsingNamespace* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolUsingNamespace>(node);
					EnsureUsingNamespaceSymbolCompleted(symbol, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration)
		}
	}
}