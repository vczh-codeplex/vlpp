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

			void AssertAccessor(ManagedMember* node, ManagedSymbolDeclaration* containingType, declatt::Accessor accessor, const MAP& argument)
			{
				if(containingType->inheritation==declatt::Sealed)
				{
					switch(accessor)
					{
					case declatt::Protected:
					case declatt::ProtectedInternal:
						argument.errors.Add(ManagedLanguageCodeException::GetSealedTypeMemberIllegalAccessor(node));
						break;
					}
				}
				if(containingType->memberType==declatt::Static)
				{
					switch(accessor)
					{
					case declatt::Protected:
					case declatt::ProtectedInternal:
						argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeMemberIllegalAccessor(node));
						break;
					}
				}
			}

			void AssertInheritance(ManagedMember* node, ManagedSymbolDeclaration* containingType, declatt::Inheritation inheritance, const MAP& argument)
			{
				if(containingType->inheritation!=declatt::Abstract)
				{
					if(inheritance==declatt::Abstract)
					{
						argument.errors.Add(ManagedLanguageCodeException::GetNonAbstractTypeMemberIllegalInheritance(node));
					}
				}
				if(containingType->inheritation==declatt::Sealed)
				{
					switch(inheritance)
					{
					case declatt::Abstract:
					case declatt::Virtual:
						argument.errors.Add(ManagedLanguageCodeException::GetSealedTypeMemberIllegalInheritance(node));
						break;
					}
				}
				if(containingType->memberType==declatt::Static)
				{
					switch(inheritance)
					{
					case declatt::Abstract:
					case declatt::Override:
					case declatt::Virtual:
						argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeMemberIllegalInheritance(node));
						break;
					}
				}
			}

			void AssertMemberType(ManagedMember* node, ManagedSymbolDeclaration* containingType, declatt::MemberType memberType, const MAP& argument)
			{
				if(containingType->memberType==declatt::Static)
				{
					switch(memberType)
					{
					case declatt::Instance:
						argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeMemberIllegalMemberType(node));
						break;
					}
				}
			}

			template<typename T>
			ManagedAbstractItem FindOverrideTargetInternal(
				T* member,
				ManagedSymbolDeclaration* containingType,
				ManagedTypeSymbol* abstractType,
				bool(*checker)(T*, ManagedSymbolDeclaration*, T*, ManagedTypeSymbol*, const MAP&),
				const MAP& argument
				)
			{
				ManagedSymbolDeclaration* abstractSymbol=dynamic_cast<ManagedSymbolDeclaration*>(abstractType->GetSymbol());
				if(ManagedSymbolItemGroup* group=abstractSymbol->ItemGroup(member->GetName()))
				{
					FOREACH(ManagedSymbolItem*, item, group->Items())
					{
						T* abstractMember=dynamic_cast<T*>(item);
						if(abstractMember)
						{
							switch(abstractMember->inheritation)
							{
							case declatt::Virtual:
							case declatt::Abstract:
								{
									if(checker(member, containingType, abstractMember, abstractType, argument))
									{
										ManagedAbstractItem result;
										result.symbol=abstractMember;
										result.type=abstractType;
										return result;
									}
								}
								break;
							}
						}
					}
				}

				FOREACH(ManagedTypeSymbol*, baseType, abstractSymbol->baseTypes.Wrap())
				{
					if((abstractSymbol->GetSymbolType()==ManagedSymbolItem::Interface)==(baseType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::Interface))
					{
						baseType=argument.symbolManager->GetBaseType(baseType, abstractType);
						ManagedAbstractItem result=FindOverrideTargetInternal(member, containingType, baseType, checker, argument);
						if(result)
						{
							return result;
						}
					}
				}
				return ManagedAbstractItem();
			}

			bool FindOverrideTargetInternalChecker(ManagedSymbolMethod* member, ManagedSymbolDeclaration* containingType, ManagedSymbolMethod* abstractMember, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return true;
			}

			bool FindOverrideTargetInternalChecker(ManagedSymbolProperty* member, ManagedSymbolDeclaration* containingType, ManagedSymbolProperty* abstractMember, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return true;
			}

			bool FindOverrideTargetInternalChecker(ManagedSymbolConverterOperator* member, ManagedSymbolDeclaration* containingType, ManagedSymbolConverterOperator* abstractMember, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return true;
			}

			ManagedAbstractItem FindOverrideTarget(ManagedSymbolMethod* member, ManagedSymbolDeclaration* containingType, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return FindOverrideTargetInternal(member, containingType, abstractType, &FindOverrideTargetInternalChecker, argument);
			}

			ManagedAbstractItem FindOverrideTarget(ManagedSymbolProperty* member, ManagedSymbolDeclaration* containingType, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return FindOverrideTargetInternal(member, containingType, abstractType, &FindOverrideTargetInternalChecker, argument);
			}

			ManagedAbstractItem FindOverrideTarget(ManagedSymbolConverterOperator* member, ManagedSymbolDeclaration* containingType, ManagedTypeSymbol* abstractType, const MAP& argument)
			{
				return FindOverrideTargetInternal(member, containingType, abstractType, &FindOverrideTargetInternalChecker, argument);
			}

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
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalType(node));
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
							AssertMemberType(node, containingType, symbol->memberType, argument);
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
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalInheritation(node));
							}
							if(symbol->implementedInterfaceType)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalImplementedInterface(node));
							}
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
							AssertInheritance(node, containingType, symbol->inheritation, argument);
							AssertMemberType(node, containingType, symbol->memberType, argument);
							
							if(symbol->implementedInterfaceType)
							{
								if(symbol->memberType==declatt::Static)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalMemberType(node));
								}
								if(symbol->inheritation==declatt::Override)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalInheritation(node));
								}
								if(!containingType->_baseInterfaces.Contains(symbol->implementedInterfaceType))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetMemberImplementedInterfaceTypeNotExists(node));
								}
								if(!FindOverrideTarget(symbol, containingType, symbol->implementedInterfaceType, argument))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(node));
								}
							}
							else
							{
								switch(symbol->inheritation)
								{
								case declatt::Override:
									{
										ManagedTypeSymbol* thisType=GetTypeFromInsideScope(containingType, argument);
										ManagedAbstractItem overridedMember=FindOverrideTarget(symbol, containingType, thisType, argument);
										if(!overridedMember)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(node));
										}
										else
										{
											if(symbol->accessor!=dynamic_cast<ManagedSymbolMethod*>(overridedMember.symbol)->accessor)
											{
												argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetIllegalAccessor(node));
											}
											containingType->_abstractTargets.Remove(overridedMember);
										}
									}
									break;
								case declatt::Abstract:
									{
										ManagedTypeSymbol* thisType=GetTypeFromInsideScope(containingType, argument);
										ManagedAbstractItem abstractItem;
										abstractItem.type=thisType;
										abstractItem.symbol=symbol;
										containingType->_abstractTargets.Add(abstractItem);
									}
									break;
								}
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
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalType(node));
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node->declaration.Obj());
					switch(symbol->GetParentItem()->GetSymbolType())
					{
					case ManagedSymbolItem::Interface:
						{
							argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalType(node));
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
							AssertInheritance(node, containingType, symbol->inheritation, argument);
							AssertMemberType(node, containingType, symbol->memberType, argument);
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
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalInheritation(node));
							}
							if(symbol->implementedInterfaceType)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalImplementedInterface(node));
							}
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
							AssertInheritance(node, containingType, symbol->inheritation, argument);
							AssertMemberType(node, containingType, symbol->memberType, argument);
							
							if(symbol->implementedInterfaceType)
							{
								if(symbol->memberType==declatt::Static)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalMemberType(node));
								}
								if(symbol->inheritation==declatt::Override)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalInheritation(node));
								}
								if(!containingType->_baseInterfaces.Contains(symbol->implementedInterfaceType))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetMemberImplementedInterfaceTypeNotExists(node));
								}
								if(!FindOverrideTarget(symbol, containingType, symbol->implementedInterfaceType, argument))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(node));
								}
							}
							else
							{
								ManagedTypeSymbol* thisType=GetTypeFromInsideScope(containingType, argument);
								switch(symbol->inheritation)
								{
								case declatt::Override:
									{
										ManagedAbstractItem overridedMember=FindOverrideTarget(symbol, containingType, thisType, argument);
										if(!overridedMember)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(node));
										}
										else
										{
											if(symbol->accessor!=dynamic_cast<ManagedSymbolProperty*>(overridedMember.symbol)->accessor)
											{
												argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetIllegalAccessor(node));
											}
											containingType->_abstractTargets.Remove(overridedMember);
										}
									}
									break;
								case declatt::Abstract:
									{
										ManagedAbstractItem abstractItem;
										abstractItem.type=thisType;
										abstractItem.symbol=symbol;
										containingType->_abstractTargets.Add(abstractItem);
									}
									break;
								}
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
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalAccessor(node));
							}
							if(symbol->memberType!=declatt::Instance)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalMemberType(node));
							}
							if(symbol->inheritation!=declatt::Abstract)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceMemberIllegalInheritation(node));
							}
						}
						break;
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						{
							ManagedSymbolDeclaration* containingType=dynamic_cast<ManagedSymbolDeclaration*>(symbol->GetParentItem());
							AssertAccessor(node, containingType, symbol->accessor, argument);
							AssertInheritance(node, containingType, symbol->inheritation, argument);
							AssertMemberType(node, containingType, symbol->memberType, argument);

							
							ManagedTypeSymbol* thisType=GetTypeFromInsideScope(containingType, argument);
							switch(symbol->inheritation)
							{
							case declatt::Override:
								{
									ManagedAbstractItem overridedMember=FindOverrideTarget(symbol, containingType, thisType, argument);
									if(!overridedMember)
									{
										argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(node));
									}
									else
									{
										if(symbol->accessor!=dynamic_cast<ManagedSymbolConverterOperator*>(overridedMember.symbol)->accessor)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetMemberOverridedTargetIllegalAccessor(node));
										}
										containingType->_abstractTargets.Remove(overridedMember);
									}
								}
								break;
							case declatt::Abstract:
								{
									ManagedAbstractItem abstractItem;
									abstractItem.type=thisType;
									abstractItem.symbol=symbol;
									containingType->_abstractTargets.Add(abstractItem);
								}
								break;
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
								if(symbol->memberType==declatt::Static)
								{
									if(dynamic_cast<ManagedSymbolDeclaration*>(baseType->GetSymbol())->memberType!=declatt::Static)
									{
										argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeIllegalBaseClass(node));
									}
								}
								symbol->_baseType=baseType;
							}
							break;
						case ManagedSymbolItem::Interface:
							if(symbol->memberType==declatt::Static)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeIllegalBaseClass(node));
							}
							else if(!derivedInterfaces.Contains(baseType))
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
										baseType=GetRealType(argument.symbolManager->GetBaseType(baseType, derivedInterface), argument);
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
							if(symbol->memberType==declatt::Static)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetInterfaceIllegalMemberType(node));
							}
						}
						break;
					}

					if(symbol->memberType==declatt::Static)
					{
						switch(symbol->inheritation)
						{
						case declatt::Abstract:
						case declatt::Virtual:
						case declatt::Override:
							argument.errors.Add(ManagedLanguageCodeException::GetStaticTypeIllegalInheritance(node));
							break;
						}
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

					if(symbol->_baseType)
					{
						ManagedTypeSymbol* thisType=GetTypeFromInsideScope(symbol, argument);
						ManagedSymbolDeclaration* baseSymbol=dynamic_cast<ManagedSymbolDeclaration*>(symbol->_baseType->GetSymbol());
						FOREACH(ManagedAbstractItem, abstractItem, baseSymbol->_abstractTargets.Wrap())
						{
							ManagedAbstractItem newAbstractItem;
							newAbstractItem.symbol=abstractItem.symbol;
							newAbstractItem.type=argument.symbolManager->ReplaceGenericArguments(abstractItem.type, symbol->_baseType);
							symbol->_abstractTargets.Add(newAbstractItem);
						}
					}

					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope4_Member(member, argument);
					}

					if(symbol->GetSymbolType()!=ManagedSymbolItem::Interface)
					{
						if(symbol->inheritation!=declatt::Abstract && symbol->_abstractTargets.Count()>0)
						{
							for(vint i=0;i<symbol->_abstractTargets.Count();i++)
							{
								ManagedAbstractItem abstractItem=symbol->_abstractTargets[i];
								argument.errors.Add(ManagedLanguageCodeException::GetAbstractMemberNotImplemented(node, abstractItem.type, abstractItem.symbol));
							}
						}
						FOREACH(ManagedTypeSymbol*, interfaceType, symbol->_baseInterfaces.Wrap())
						{
						}
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