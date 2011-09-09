#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"
#include "..\..\Collections\OperationCopyFrom.h"

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
					ManagedSymbolField* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolField>(node);
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberType(node));
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							if(symbol->accessor!=declatt::Public)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberInheritation(node));
							}
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberType(node));
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedSymbolItem* symbol=argument.symbolManager->GetSymbol(node->declaration.Obj());
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberType(node));
						}
						break;
					}
					ManagedLanguage_BuildGlobalScope4_Declaration(node->declaration, argument);
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
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							if(symbol->accessor!=declatt::Public)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberInheritation(node));
							}
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					ManagedSymbolConverterOperator* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConverterOperator>(node);
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							if(symbol->accessor!=declatt::Public)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberInheritation(node));
							}
						}
						break;
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope4_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					// TODO: check TypeCannotDerivedFromItself

					List<ManagedTypeSymbol*> derivedInterfaces;
					FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
					{
						baseType=GetRealType(baseType, argument);
						switch(baseType->GetSymbol()->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
							if(symbol->_baseType)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetClassStructureTooMuchBaseClasses(node));
							}
							else if(symbol->GetSymbolType()==ManagedSymbolItem::Interface)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalBaseClass(node));
							}
							{
								symbol->_baseType=baseType;
							}
							break;
						case ManagedSymbolItem::Interface:
							if(!derivedInterfaces.Contains(baseType))
							{
								derivedInterfaces.Add(baseType);
							}
							break;
						case ManagedSymbolItem::GenericParameter:
							{
								switch(symbol->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
									argument.errors.Add(ManagedLanguageCodeException::GetClassIllegalBaseClass(node));
									break;
								case ManagedSymbolItem::Structure:
									argument.errors.Add(ManagedLanguageCodeException::GetStructureIllegalBaseClass(node));
									break;
								case ManagedSymbolItem::Interface:
									argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalBaseClass(node));
									break;
								}
							}
							break;
						}
					}

					{
						vint start=0;
						vint end=derivedInterfaces.Count();
						while(start<end)
						{
							for(vint i=start;i<end;i++)
							{
								ManagedTypeSymbol* derivedInterface=derivedInterfaces[i];
								FOREACH(ManagedTypeSymbol*, baseType, dynamic_cast<ManagedSymbolDeclaration*>(derivedInterface->GetSymbol())->baseTypes.Wrap())
								{
									if(GetRealSymbol(baseType->GetSymbol())->GetSymbolType()==ManagedSymbolItem::Interface)
									{
										baseType=GetRealType(argument.symbolManager->ReplaceGenericArguments(baseType, derivedInterface), argument);
										if(!derivedInterfaces.Contains(baseType))
										{
											derivedInterfaces.Add(baseType);
										}
									}
								}
							}
							start=end;
							end=derivedInterfaces.Count();
						}
						CopyFrom(symbol->_baseInterfaces.Wrap(), derivedInterfaces.Wrap());
					}

					switch(symbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
						{
							if(symbol->_baseType)
							{
								if(symbol->_baseType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::Structure)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetClassIllegalBaseClass(node));
								}
								else if(dynamic_cast<ManagedSymbolDeclaration*>(symbol->_baseType->GetSymbol())->inheritation==declatt::Sealed)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetClassIllegalBaseClass(node));
								}
							}
							if(symbol->inheritation==declatt::Virtual || symbol->inheritation==declatt::Override)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetClassStructureIllegalInheritation(node));
							}
						}
						break;
					case ManagedSymbolItem::Structure:
						{
							if(symbol->_baseType)
							{
								if(symbol->_baseType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::Structure)
								{
									if(dynamic_cast<ManagedSymbolDeclaration*>(symbol->_baseType->GetSymbol())->inheritation==declatt::Sealed)
									{
										argument.errors.Add(ManagedLanguageCodeException::GetClassIllegalBaseClass(node));
									}
									else
									{
										argument.errors.Add(ManagedLanguageCodeException::GetStructureIllegalBaseClass(node));
									}
								}
								else if(symbol->_baseType!=argument.contextManager->predefinedTypes.objectType)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetStructureIllegalBaseClass(node));
								}
							}
							if(symbol->inheritation==declatt::Virtual || symbol->inheritation==declatt::Override)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetClassStructureIllegalInheritation(node));
							}
						}
						break;
					case ManagedSymbolItem::Interface:
						{
							if(symbol->inheritation==declatt::Virtual || symbol->inheritation==declatt::Override || symbol->inheritation==declatt::Sealed)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalInheritation(node));
							}
						}
						break;
					}

					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Global:
					case ManagedSymbolItem::Namespace:
						{
							if(symbol->accessor==declatt::Protected || symbol->accessor==declatt::ProtectedInternal)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetTypeIllegalAccessor(node));
							}
						}
						break;
					}

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