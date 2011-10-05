#include "ManagedLanguageCodeGeneration.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
Symbol to Metadata
***********************************************************************/

			namespace resource_transformation
			{
				class ResourceGenerator
				{
				public:
					Dictionary<ManagedSymbolItem*, ResourceHandle<ManagedSymbolItemRes>>			symbolItems;
					Dictionary<ManagedTypeSymbol*, ResourceHandle<ManagedTypeSymbolRes>>			symbolTypes;
					Ptr<ResourceStream>																resourceStream;
					ManagedSymbolManager*															symbolManager;

				protected:

					void CollectType(ManagedTypeSymbol* typeSymbol)
					{
						if(typeSymbol && !symbolTypes.Keys().Contains(typeSymbol))
						{
							ResourceRecord<ManagedTypeSymbolRes> resource=resourceStream->CreateRecord<ManagedTypeSymbolRes>();
							resource->typeSymbol=ResourceHandle<ManagedSymbolItemRes>::Null();
							resource->parentType=ResourceHandle<ManagedTypeSymbolRes>::Null();
							resource->genericDeclaration=ResourceHandle<ManagedTypeSymbolRes>::Null();
							resource->genericArguments=ResourceArrayHandle<ManagedTypeSymbolRes>::Null();
							symbolTypes.Add(typeSymbol, resource);

							ManagedSymbolItem* symbol=typeSymbol->GetSymbol();
							while(symbol)
							{
								CollectSymbol(symbol, true);
								symbol=symbol->GetParentItem();
							}
							
							CollectType(typeSymbol->GetParentType());
							CollectType(typeSymbol->GetGenericDeclaration());
							FOREACH(ManagedTypeSymbol*, genericArgument, typeSymbol->GetGenericArguments())
							{
								CollectType(genericArgument);
							}
						}
					}

					template<typename TSymbol, typename TResource>
					void CollectSymbol(TSymbol* symbol, ManagedSymbolItemRes::SymbolType symbolType)
					{
						ResourceRecord<TResource> resource=resourceStream->CreateRecord<TResource>();
						ResourceHandle<TResource> resourceHandle=resource;
						symbolItems.Add(symbol, resourceHandle.Cast<ManagedSymbolItemRes>());

						resource->symbolType=symbolType;
						ResourceString name=resourceStream->CreateString(symbol->GetName());
						resource->name=name;
						resource->subItems=ResourceArrayHandle<ManagedSymbolItemRes>::Null();
					}

					void CollectSymbol(ManagedSymbolItem* symbol, bool forceAccept=false)
					{
						if(!symbolItems.Keys().Contains(symbol))
						{
							bool accepted=true;
							switch(symbol->GetSymbolType())
							{
							case ManagedSymbolItem::Global:
								{
									ManagedSymbolGlobal* typedSymbol=dynamic_cast<ManagedSymbolGlobal*>(symbol);
									CollectSymbol<ManagedSymbolGlobal, ManagedSymbolGlobalRes>(typedSymbol, ManagedSymbolItemRes::Global);
								}
								break;
							case ManagedSymbolItem::Namespace:
								{
									ManagedSymbolNamespace* typedSymbol=dynamic_cast<ManagedSymbolNamespace*>(symbol);
									CollectSymbol<ManagedSymbolNamespace, ManagedSymbolNamespaceRes>(typedSymbol, ManagedSymbolItemRes::Namespace);
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									ManagedSymbolTypeRename* typedSymbol=dynamic_cast<ManagedSymbolTypeRename*>(symbol);
									CollectSymbol<ManagedSymbolTypeRename, ManagedSymbolTypeRenameRes>(typedSymbol, ManagedSymbolItemRes::TypeRename);
									CollectType(typedSymbol->type);
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									bool externalDeclaration=false;
									ManagedSymbolDeclaration* typedSymbol=dynamic_cast<ManagedSymbolDeclaration*>(symbol);
									if(!typedSymbol->typeLanguageElement && !typedSymbol->enumerationLanguageElement)
									{
										accepted=false;
										externalDeclaration=true;
									}
									if(accepted || (externalDeclaration && forceAccept))
									{
										if(externalDeclaration)
										{
											CollectSymbol<ManagedSymbolDeclaration, ManagedSymbolDeclarationRes>(typedSymbol, ManagedSymbolItemRes::ExternalDeclaration);
										}
										else
										{
											CollectSymbol<ManagedSymbolDeclaration, ManagedSymbolDeclarationRes>(typedSymbol, ManagedSymbolItemRes::TypeDeclaration);
											FOREACH(ManagedTypeSymbol*, baseType, typedSymbol->baseTypes.Wrap())
											{
												CollectType(baseType);
											}
											CollectType(typedSymbol->_baseType);
											FOREACH(ManagedTypeSymbol*, baseInterface, typedSymbol->_baseInterfaces.Wrap())
											{
												CollectType(baseInterface);
											}
										}
									}
								}
								break;
							case ManagedSymbolItem::Field:
								{
									ManagedSymbolField* typedSymbol=dynamic_cast<ManagedSymbolField*>(symbol);
									CollectSymbol<ManagedSymbolField, ManagedSymbolFieldRes>(typedSymbol, ManagedSymbolItemRes::Field);
									CollectType(typedSymbol->type);
								}
								break;
							case ManagedSymbolItem::Property:
								{
									ManagedSymbolProperty* typedSymbol=dynamic_cast<ManagedSymbolProperty*>(symbol);
									CollectSymbol<ManagedSymbolProperty, ManagedSymbolPropertyRes>(typedSymbol, ManagedSymbolItemRes::Property);
									CollectType(typedSymbol->implementedInterfaceType);
									CollectType(typedSymbol->type);
								}
								break;
							case ManagedSymbolItem::PropertySetterValue:
								{
									ManagedSymbolPropertySetterValue* typedSymbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(symbol);
									CollectSymbol<ManagedSymbolPropertySetterValue, ManagedSymbolPropertySetterValueRes>(typedSymbol, ManagedSymbolItemRes::PropertySetterValue);
								}
								break;
							case ManagedSymbolItem::Method:
								{
									ManagedSymbolMethod* typedSymbol=dynamic_cast<ManagedSymbolMethod*>(symbol);
									CollectSymbol<ManagedSymbolMethod, ManagedSymbolMethodRes>(typedSymbol, ManagedSymbolItemRes::Method);
									CollectType(typedSymbol->implementedInterfaceType);
									CollectType(typedSymbol->returnType);
								}
								break;
							case ManagedSymbolItem::Constructor:
								{
									ManagedSymbolConstructor* typedSymbol=dynamic_cast<ManagedSymbolConstructor*>(symbol);
									CollectSymbol<ManagedSymbolConstructor, ManagedSymbolConstructorRes>(typedSymbol, ManagedSymbolItemRes::Constructor);
								}
								break;
							case ManagedSymbolItem::ConverterOperator:
								{
									ManagedSymbolConverterOperator* typedSymbol=dynamic_cast<ManagedSymbolConverterOperator*>(symbol);
									CollectSymbol<ManagedSymbolConverterOperator, ManagedSymbolConverterOperatorRes>(typedSymbol, ManagedSymbolItemRes::ConverterOperator);
									CollectType(typedSymbol->targetType);
								}
								break;
							case ManagedSymbolItem::GenericParameter:
								{
									ManagedSymbolGenericParameter* typedSymbol=dynamic_cast<ManagedSymbolGenericParameter*>(symbol);
									CollectSymbol<ManagedSymbolGenericParameter, ManagedSymbolGenericParameterRes>(typedSymbol, ManagedSymbolItemRes::GenericParameter);
									FOREACH(ManagedTypeSymbol*, typeConstraint, typedSymbol->typeConstraints.Wrap())
									{
										CollectType(typeConstraint);
									}
								}
								break;
							case ManagedSymbolItem::MethodParameter:
								{
									ManagedSymbolMethodParameter* typedSymbol=dynamic_cast<ManagedSymbolMethodParameter*>(symbol);
									CollectSymbol<ManagedSymbolMethodParameter, ManagedSymbolMethodParameterRes>(typedSymbol, ManagedSymbolItemRes::MethodParameter);
									CollectType(typedSymbol->type);
								}
								break;
							default:
								accepted=false;
							}
							if(accepted)
							{
								FOREACH(ManagedSymbolItemGroup*, group, symbol->ItemGroups().Values())
								{
									FOREACH(ManagedSymbolItem*, item, group->Items())
									{
										CollectSymbol(item);
									}
								}
							}
						}
					}

				protected:

					void GenerateTypeResource(ManagedTypeSymbol* typeSymbol, ResourceHandle<ManagedTypeSymbolRes> handle)
					{
						ResourceRecord<ManagedTypeSymbolRes> resource=resourceStream->ReadRecord(handle);
						resource->typeSymbol=symbolItems[typeSymbol->GetSymbol()];
						if(typeSymbol->GetParentType())
						{
							resource->parentType=symbolTypes[typeSymbol->GetParentType()];
						}
						if(typeSymbol->GetGenericDeclaration())
						{
							resource->genericDeclaration=symbolTypes[typeSymbol->GetGenericDeclaration()];
						}

						if(typeSymbol->GetGenericArguments().Count()>0)
						{
							ResourceArrayRecord<ManagedTypeSymbolRes> genericArguments=resourceStream->CreateArrayRecord<ManagedTypeSymbolRes>(typeSymbol->GetGenericArguments().Count());
							resource->genericArguments=genericArguments;
							for(vint i=0;i<typeSymbol->GetGenericArguments().Count();i++)
							{
								genericArguments[i]=symbolTypes[typeSymbol->GetGenericArguments()[i]];
							}
						}
					}

					void GenerateArray(ResourceArrayHandle<ManagedStringRes>& dest, List<WString>& source)
					{
						if(source.Count()>0)
						{
							ResourceArrayRecord<ManagedStringRes> resource=resourceStream->CreateArrayRecord<ManagedStringRes>(source.Count());
							dest=resource;
							for(vint i=0;i<source.Count();i++)
							{
								ResourceRecord<ManagedStringRes> stringResource=resourceStream->CreateRecord<ManagedStringRes>();
								ResourceString string=resourceStream->CreateString(source[i]);
								stringResource->stringValue=string;
								resource[i]=stringResource;
							}
						}
						else
						{
							dest=ResourceArrayHandle<ManagedStringRes>::Null();
						}
					}

					void GenerateArray(ResourceArrayHandle<ManagedTypeSymbolRes>& dest, List<ManagedTypeSymbol*>& source)
					{
						if(source.Count()>0)
						{
							ResourceArrayRecord<ManagedTypeSymbolRes> resource=resourceStream->CreateArrayRecord<ManagedTypeSymbolRes>(source.Count());
							dest=resource;
							for(vint i=0;i<source.Count();i++)
							{
								resource[i]=symbolTypes[source[i]];
							}
						}
						else
						{
							dest=ResourceArrayHandle<ManagedTypeSymbolRes>::Null();
						}
					}

					void GenerateResource(ManagedSymbolItem* symbol, ResourceHandle<ManagedSymbolItemRes> handle)
					{
						ResourceRecord<ManagedSymbolItemRes> resource=resourceStream->ReadRecord(handle);
						if(symbol->ItemGroups().Count()>0)
						{
							List<ManagedSymbolItem*> acceptedItems;
							FOREACH(ManagedSymbolItemGroup*, group, symbol->ItemGroups().Values())
							{
								FOREACH(ManagedSymbolItem*, item, group->Items())
								{
									if(symbolItems.Keys().Contains(item))
									{
										acceptedItems.Add(item);
									}
								}
							}

							ResourceArrayRecord<ManagedSymbolItemRes> subItems=resourceStream->CreateArrayRecord<ManagedSymbolItemRes>(acceptedItems.Count());
							resource->subItems=subItems;
							for(vint i=0;i<acceptedItems.Count();i++)
							{
								subItems[i]=symbolItems[acceptedItems[i]];
							}
						}
						
						switch(symbol->GetSymbolType())
						{
						case ManagedSymbolItem::TypeRename:
							{
								ManagedSymbolTypeRename* typedSymbol=dynamic_cast<ManagedSymbolTypeRename*>(symbol);
								ResourceRecord<ManagedSymbolTypeRenameRes> typedResource=resource.Cast<ManagedSymbolTypeRenameRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->type=symbolTypes[typedSymbol->type];
								GenerateArray(typedResource->orderedGenericParameterNames, typedSymbol->orderedGenericParameterNames);
							}
							break;
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								ManagedSymbolDeclaration* typedSymbol=dynamic_cast<ManagedSymbolDeclaration*>(symbol);
								if(resource->symbolType==ManagedSymbolItemRes::TypeDeclaration)
								{
									ResourceRecord<ManagedSymbolDeclarationRes> typedResource=resource.Cast<ManagedSymbolDeclarationRes>();
									switch(symbol->GetSymbolType())
									{
									case ManagedSymbolItem::Class:
										typedResource->declarationType=ManagedSymbolDeclarationRes::Class;
										break;
									case ManagedSymbolItem::Structure:
										typedResource->declarationType=ManagedSymbolDeclarationRes::Structure;
										break;
									case ManagedSymbolItem::Interface:
										typedResource->declarationType=ManagedSymbolDeclarationRes::Interface;
										break;
									}
									typedResource->accessor=typedSymbol->accessor;
									typedResource->inheritation=typedSymbol->inheritation;
									typedResource->memberType=typedSymbol->memberType;
									GenerateArray(typedResource->baseTypes, typedSymbol->baseTypes);
									GenerateArray(typedResource->orderedGenericParameterNames, typedSymbol->orderedGenericParameterNames);
									GenerateArray(typedResource->orderedDataMemberNames, typedSymbol->orderedDataMemberNames);
									typedResource->_basicType=typedSymbol->_baseType?symbolTypes[typedSymbol->_baseType]:ResourceHandle<ManagedTypeSymbolRes>::Null();
									GenerateArray(typedResource->_baseInterfaces, typedSymbol->_baseInterfaces);
								}
								else
								{
									ResourceRecord<ManagedSymbolExternalDeclarationRes> typedResource=resource.Cast<ManagedSymbolExternalDeclarationRes>();
									ResourceString name=resourceStream->CreateString(typedSymbol->foreignAssemblyName);
									typedResource->foreignAssemblyName=name;
									typedResource->genericParameterCount=typedSymbol->orderedGenericParameterNames.Count();
								}
							}
							break;
						case ManagedSymbolItem::Field:
							{
								ManagedSymbolField* typedSymbol=dynamic_cast<ManagedSymbolField*>(symbol);
								ResourceRecord<ManagedSymbolFieldRes> typedResource=resource.Cast<ManagedSymbolFieldRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->memberType=typedSymbol->memberType;
								typedResource->dataType=typedSymbol->dataType;
								typedResource->type=symbolTypes[typedSymbol->type];
							}
							break;
						case ManagedSymbolItem::Property:
							{
								ManagedSymbolProperty* typedSymbol=dynamic_cast<ManagedSymbolProperty*>(symbol);
								ResourceRecord<ManagedSymbolPropertyRes> typedResource=resource.Cast<ManagedSymbolPropertyRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->setterAccessor=typedSymbol->setterAccessor;
								typedResource->memberType=typedSymbol->memberType;
								typedResource->inheritation=typedSymbol->inheritation;
								typedResource->type=symbolTypes[typedSymbol->type];
								typedResource->implementedInterfaceType=typedSymbol->implementedInterfaceType?symbolTypes[typedSymbol->implementedInterfaceType]:ResourceHandle<ManagedTypeSymbolRes>::Null();
								typedResource->containsGetter=typedSymbol->containsGetter;
								typedResource->containsSetter=typedSymbol->containsSetter;
							}
							break;
						case ManagedSymbolItem::PropertySetterValue:
							{
								ManagedSymbolPropertySetterValue* typedSymbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(symbol);
								ResourceRecord<ManagedSymbolPropertySetterValueRes> typedResource=resource.Cast<ManagedSymbolPropertySetterValueRes>();
								typedResource->associatedProperty=symbolItems[typedSymbol->associatedProperty].Cast<ManagedSymbolPropertyRes>();
							}
							break;
						case ManagedSymbolItem::Method:
							{
								ManagedSymbolMethod* typedSymbol=dynamic_cast<ManagedSymbolMethod*>(symbol);
								ResourceRecord<ManagedSymbolMethodRes> typedResource=resource.Cast<ManagedSymbolMethodRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->memberType=typedSymbol->memberType;
								typedResource->inheritation=typedSymbol->inheritation;
								typedResource->returnType=typedSymbol->returnType?symbolTypes[typedSymbol->returnType]:ResourceHandle<ManagedTypeSymbolRes>::Null();
								typedResource->implementedInterfaceType=typedSymbol->implementedInterfaceType?symbolTypes[typedSymbol->implementedInterfaceType]:ResourceHandle<ManagedTypeSymbolRes>::Null();
								GenerateArray(typedResource->orderedGenericParameterNames, typedSymbol->orderedGenericParameterNames);
								GenerateArray(typedResource->orderedMethodParameterNames, typedSymbol->orderedMethodParameterNames);
							}
							break;
						case ManagedSymbolItem::Constructor:
							{
								ManagedSymbolConstructor* typedSymbol=dynamic_cast<ManagedSymbolConstructor*>(symbol);
								ResourceRecord<ManagedSymbolConstructorRes> typedResource=resource.Cast<ManagedSymbolConstructorRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->implicit=typedSymbol->implicit;
								GenerateArray(typedResource->orderedMethodParameterNames, typedSymbol->orderedMethodParameterNames);
							}
							break;
						case ManagedSymbolItem::ConverterOperator:
							{
								ManagedSymbolConverterOperator* typedSymbol=dynamic_cast<ManagedSymbolConverterOperator*>(symbol);
								ResourceRecord<ManagedSymbolConverterOperatorRes> typedResource=resource.Cast<ManagedSymbolConverterOperatorRes>();
								typedResource->accessor=typedSymbol->accessor;
								typedResource->memberType=typedSymbol->memberType;
								typedResource->inheritation=typedSymbol->inheritation;
								typedResource->implicit=typedSymbol->implicit;
								typedResource->targetType=symbolTypes[typedSymbol->targetType];
								GenerateArray(typedResource->orderedGenericParameterNames, typedSymbol->orderedGenericParameterNames);
							}
							break;
						case ManagedSymbolItem::GenericParameter:
							{
								ManagedSymbolGenericParameter* typedSymbol=dynamic_cast<ManagedSymbolGenericParameter*>(symbol);
								ResourceRecord<ManagedSymbolGenericParameterRes> typedResource=resource.Cast<ManagedSymbolGenericParameterRes>();
								typedResource->conversion=typedSymbol->conversion;
								typedResource->newConstraint=typedSymbol->newConstraint;
								GenerateArray(typedResource->typeConstraints, typedSymbol->typeConstraints);
							}
							break;
						case ManagedSymbolItem::MethodParameter:
							{
								ManagedSymbolMethodParameter* typedSymbol=dynamic_cast<ManagedSymbolMethodParameter*>(symbol);
								ResourceRecord<ManagedSymbolMethodParameterRes> typedResource=resource.Cast<ManagedSymbolMethodParameterRes>();
								typedResource->parameterType=typedSymbol->parameterType;
								typedResource->containsDefaultValue=typedSymbol->containsDefaultValue;
								typedResource->type=symbolTypes[typedSymbol->type];
							}
							break;
						}
					}

				public:
					ResourceGenerator(Ptr<ResourceStream> _resourceStream, ManagedSymbolManager* _symbolManager)
						:resourceStream(_resourceStream)
						,symbolManager(_symbolManager)
					{
					}

					void CollectResources()
					{
						CollectSymbol(symbolManager->Global());
					}

					void GenerateResources()
					{
						for(vint i=0;i<symbolItems.Count();i++)
						{
							GenerateResource(symbolItems.Keys()[i], symbolItems.Values()[i]);
						}
						for(vint i=0;i<symbolTypes.Count();i++)
						{
							GenerateTypeResource(symbolTypes.Keys()[i], symbolTypes.Values()[i]);
						}
					}
				};
			};
			using namespace resource_transformation;

			ResourceHandle<ManagedEntryRes> ManagedLanguage_GenerateResource(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager)
			{
				ResourceRecord<ManagedEntryRes> entryRes=resourceStream->CreateRecord<ManagedEntryRes>();

				ResourceGenerator resourceGenerator(resourceStream, symbolManager);
				resourceGenerator.CollectResources();
				resourceGenerator.GenerateResources();

				entryRes->globalNamespace=resourceGenerator.symbolItems[symbolManager->Global()];
				return entryRes;
			}

/***********************************************************************
Metadata to Symbol
***********************************************************************/

			namespace resource_transformation
			{
				class SymbolImporter
				{
				public:
					Dictionary<vint, ManagedSymbolItem*>											symbolItems;
					Dictionary<vint, ManagedTypeSymbol*>											symbolTypes;
					Ptr<ResourceStream>																resourceStream;
					ManagedSymbolManager*															symbolManager;
					WString																			importAssemblyName;
					bool																			includingInternal;

				protected:

					void CheckExternalDeclaration(ManagedSymbolItem* containerSymbol, ResourceArrayHandle<ManagedSymbolItemRes> handleArray, List<ResourceHandle<ManagedSymbolItemRes>>& unresolvedTypes)
					{
						if(handleArray)
						{
							ResourceArrayRecord<ManagedSymbolItemRes> resourceArray=resourceStream->ReadArrayRecord(handleArray);
							for(vint i=0;i<resourceArray.Count();i++)
							{
								ResourceRecord<ManagedSymbolItemRes> resource=resourceArray[i];
								ManagedSymbolItemGroup* itemGroup=0;
								if(containerSymbol)
								{
									WString name=resourceStream->ReadString(resource->name);
									itemGroup=containerSymbol->ItemGroup(name);
								}

								bool isContainerResource=true;
								ManagedSymbolItem* subContainerSymbol=0;
								switch(resource->symbolType)
								{
								case ManagedSymbolItemRes::ExternalDeclaration:
									{
										ResourceRecord<ManagedSymbolExternalDeclarationRes> typedResource=resource.Cast<ManagedSymbolExternalDeclarationRes>();
										WString foreignAssemblyName=resourceStream->ReadString(typedResource->foreignAssemblyName);
										if(itemGroup)
										{
											FOREACH(ManagedSymbolItem*, item, itemGroup->Items())
											{
												ManagedSymbolDeclaration* typedSymbol=dynamic_cast<ManagedSymbolDeclaration*>(item);
												if(typedSymbol)
												{
													if(typedSymbol->foreignAssemblyName==foreignAssemblyName && typedSymbol->orderedGenericParameterNames.Count()==typedResource->genericParameterCount)
													{
														subContainerSymbol=item;
														break;
													}
												}
											}
										}

										if(subContainerSymbol==0)
										{
											unresolvedTypes.Add(resource);
										}
									}
									break;
								case ManagedSymbolItemRes::Namespace:
									{
										if(itemGroup)
										{
											FOREACH(ManagedSymbolItem*, item, itemGroup->Items())
											{
												ManagedSymbolNamespace* typedSymbol=dynamic_cast<ManagedSymbolNamespace*>(item);
												if(typedSymbol)
												{
													subContainerSymbol=item;
													break;
												}
											}
										}
									}
									break;
								default:
									isContainerResource=false;
								}

								if(isContainerResource)
								{
									if(subContainerSymbol)
									{
										symbolItems.Add(resource.Pointer(), subContainerSymbol);
									}
									CheckExternalDeclaration(subContainerSymbol, resource->subItems, unresolvedTypes);
								}
							}
						}
					}

				protected:

					bool IsAccessorAcceptable(declatt::Accessor accessor)
					{
						switch(accessor)
						{
						case declatt::Public:
						case declatt::Protected:
							return true;
						case declatt::Internal:
						case declatt::ProtectedInternal:
							return includingInternal;
						default:
							return false;
						}
					}

					void CreateSymbol(ManagedSymbolItem* containerSymbol, ResourceArrayHandle<ManagedSymbolItemRes> handleArray)
					{
						ResourceArrayRecord<ManagedSymbolItemRes> resource=resourceStream->ReadArrayRecord(handleArray);
						if(handleArray)
						{
							ResourceArrayRecord<ManagedSymbolItemRes> resourceArray=resourceStream->ReadArrayRecord(handleArray);
							for(vint i=0;i<resourceArray.Count();i++)
							{
								ResourceRecord<ManagedSymbolItemRes> resource=resourceArray[i];
								ManagedSymbolItem* createdSymbol=0;
								switch(resource->symbolType)
								{
								case ManagedSymbolItemRes::Namespace:
									{
										if(!symbolItems.Keys().Contains(resource.Pointer()))
										{
											createdSymbol=new ManagedSymbolNamespace(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::TypeRename:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolTypeRenameRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolTypeRename(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::TypeDeclaration:
									{
										ResourceRecord<ManagedSymbolDeclarationRes> typedResource=resource.Cast<ManagedSymbolDeclarationRes>();
										if(IsAccessorAcceptable(typedResource->accessor))
										{
											ManagedSymbolItem::ManagedSymbolType symbolType=ManagedSymbolItem::Class;
											switch(typedResource->declarationType)
											{
											case ManagedSymbolDeclarationRes::Class:
												symbolType=ManagedSymbolItem::Class;
												break;
											case ManagedSymbolDeclarationRes::Structure:
												symbolType=ManagedSymbolItem::Structure;
												break;
											case ManagedSymbolDeclarationRes::Interface:
												symbolType=ManagedSymbolItem::Interface;
												break;
											}
											createdSymbol=new ManagedSymbolDeclaration(symbolManager, symbolType);
										}
									}
									break;
								case ManagedSymbolItemRes::Field:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolFieldRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolField(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::Property:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolPropertyRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolProperty(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::PropertySetterValue:
									{
										createdSymbol=new ManagedSymbolPropertySetterValue(symbolManager);
									}
									break;
								case ManagedSymbolItemRes::Method:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolMethodRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolMethod(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::Constructor:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolConstructorRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolConstructor(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::ConverterOperator:
									{
										if(IsAccessorAcceptable(resource.Cast<ManagedSymbolConverterOperatorRes>()->accessor))
										{
											createdSymbol=new ManagedSymbolConverterOperator(symbolManager);
										}
									}
									break;
								case ManagedSymbolItemRes::GenericParameter:
									{
										createdSymbol=new ManagedSymbolGenericParameter(symbolManager);
									}
									break;
								case ManagedSymbolItemRes::MethodParameter:
									{
										createdSymbol=new ManagedSymbolMethodParameter(symbolManager);
									}
									break;
								}
								if(createdSymbol)
								{
									WString name=resourceStream->ReadString(resource->name);
									createdSymbol->SetName(name);
									containerSymbol->Add(createdSymbol);
									symbolItems.Add(resource.Pointer(), createdSymbol);
									if(resource->subItems)
									{
										CreateSymbol(createdSymbol, resource->subItems);
									}
								}
							}
						}
					}

					ManagedTypeSymbol* GetType(ResourceHandle<ManagedTypeSymbolRes> item)
					{
						if(item)
						{
							ManagedTypeSymbol* type=0;
							vint typeIndex=symbolTypes.Keys().IndexOf(item.Pointer());
							if(typeIndex!=-1)
							{
								type=symbolTypes.Values()[typeIndex];
							}
							else
							{
								ResourceRecord<ManagedTypeSymbolRes> resource=resourceStream->ReadRecord(item);
								if(resource->genericDeclaration)
								{
									ManagedTypeSymbol* genericDeclaration=GetType(resource->genericDeclaration);
									List<ManagedTypeSymbol*> genericArguments;
									if(resource->genericArguments)
									{
										ResourceArrayRecord<ManagedTypeSymbolRes> genericArgumentResources=resourceStream->ReadArrayRecord(resource->genericArguments);
										for(vint i=0;i<genericArgumentResources.Count();i++)
										{
											ResourceRecord<ManagedTypeSymbolRes> genericArgument=genericArgumentResources[i];
											genericArguments.Add(GetType(genericArgument));
										}
									}
									type=symbolManager->GetInstiantiatedType(genericDeclaration, genericArguments.Wrap());
								}
								else
								{
									ManagedTypeSymbol* parentType=GetType(resource->parentType);
									ManagedSymbolItem* symbol=symbolItems[resource->typeSymbol.Pointer()];
									type=symbolManager->GetType(symbol, parentType);
								}
								symbolTypes.Add(resource.Pointer(), type);
							}
							return type;
						}
						else
						{
							return 0;
						}
					}

					void FillList(List<WString>& stringList, ResourceArrayHandle<ManagedStringRes> itemList)
					{
						if(itemList)
						{
							ResourceArrayRecord<ManagedStringRes> resourceList=resourceStream->ReadArrayRecord(itemList);
							for(vint i=0;i<resourceList.Count();i++)
							{
								ResourceRecord<ManagedStringRes> resource=resourceList[i];
								WString string=resourceStream->ReadString(resource->stringValue);
								stringList.Add(string);
							}
						}
					}

					void FillList(List<ManagedTypeSymbol*>& typeList, ResourceArrayHandle<ManagedTypeSymbolRes> itemList)
					{
						if(itemList)
						{
							ResourceArrayRecord<ManagedTypeSymbolRes> resourceList=resourceStream->ReadArrayRecord(itemList);
							for(vint i=0;i<resourceList.Count();i++)
							{
								ResourceRecord<ManagedTypeSymbolRes> resource=resourceList[i];
								ManagedTypeSymbol* type=GetType(resource);
								typeList.Add(type);
							}
						}
					}

					void FillSymbol(ManagedSymbolItem* containerSymbol, ResourceArrayHandle<ManagedSymbolItemRes> handleArray)
					{
						ResourceArrayRecord<ManagedSymbolItemRes> resource=resourceStream->ReadArrayRecord(handleArray);
						if(handleArray)
						{
							ResourceArrayRecord<ManagedSymbolItemRes> resourceArray=resourceStream->ReadArrayRecord(handleArray);
							for(vint i=0;i<resourceArray.Count();i++)
							{
								ResourceRecord<ManagedSymbolItemRes> resource=resourceArray[i];
								vint symbolIndex=symbolItems.Keys().IndexOf(resource.Pointer());
								if(symbolIndex!=-1)
								{
									ManagedSymbolItem* symbol=symbolItems.Values()[symbolIndex];
									switch(resource->symbolType)
									{
									case ManagedSymbolItemRes::TypeRename:
										{
											ResourceRecord<ManagedSymbolTypeRenameRes> typedResource=resource.Cast<ManagedSymbolTypeRenameRes>();
											ManagedSymbolTypeRename* typedSymbol=dynamic_cast<ManagedSymbolTypeRename*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->type=GetType(typedResource->type);
											FillList(typedSymbol->orderedGenericParameterNames, typedResource->orderedGenericParameterNames);
										}
										break;
									case ManagedSymbolItemRes::TypeDeclaration:
										{
											ResourceRecord<ManagedSymbolDeclarationRes> typedResource=resource.Cast<ManagedSymbolDeclarationRes>();
											ManagedSymbolDeclaration* typedSymbol=dynamic_cast<ManagedSymbolDeclaration*>(symbol);
											typedSymbol->foreignAssemblyName=importAssemblyName;
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->inheritation=typedResource->inheritation;
											typedSymbol->memberType=typedResource->memberType;
											FillList(typedSymbol->baseTypes, typedResource->baseTypes);
											FillList(typedSymbol->orderedGenericParameterNames, typedResource->orderedGenericParameterNames);
											FillList(typedSymbol->orderedDataMemberNames, typedResource->orderedDataMemberNames);
											typedSymbol->_baseType=GetType(typedResource->_basicType);
											FillList(typedSymbol->_baseInterfaces, typedResource->_baseInterfaces);
										}
										break;
									case ManagedSymbolItemRes::Field:
										{
											ResourceRecord<ManagedSymbolFieldRes> typedResource=resource.Cast<ManagedSymbolFieldRes>();
											ManagedSymbolField* typedSymbol=dynamic_cast<ManagedSymbolField*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->memberType=typedResource->memberType;
											typedSymbol->dataType=typedResource->dataType;
											typedSymbol->type=GetType(typedResource->type);
										}
										break;
									case ManagedSymbolItemRes::Property:
										{
											ResourceRecord<ManagedSymbolPropertyRes> typedResource=resource.Cast<ManagedSymbolPropertyRes>();
											ManagedSymbolProperty* typedSymbol=dynamic_cast<ManagedSymbolProperty*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->setterAccessor=typedResource->setterAccessor;
											typedSymbol->inheritation=typedResource->inheritation;
											typedSymbol->memberType=typedResource->memberType;
											typedSymbol->type=GetType(typedResource->type);
											typedSymbol->implementedInterfaceType=GetType(typedResource->implementedInterfaceType);
											typedSymbol->containsGetter=typedResource->containsGetter;
											typedSymbol->containsSetter=typedResource->containsSetter;
										}
										break;
									case ManagedSymbolItemRes::PropertySetterValue:
										{
											ResourceRecord<ManagedSymbolPropertySetterValueRes> typedResource=resource.Cast<ManagedSymbolPropertySetterValueRes>();
											ManagedSymbolPropertySetterValue* typedSymbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(symbol);
											typedSymbol->associatedProperty=dynamic_cast<ManagedSymbolProperty*>(symbolItems[typedResource->associatedProperty.Pointer()]);
										}
										break;
									case ManagedSymbolItemRes::Method:
										{
											ResourceRecord<ManagedSymbolMethodRes> typedResource=resource.Cast<ManagedSymbolMethodRes>();
											ManagedSymbolMethod* typedSymbol=dynamic_cast<ManagedSymbolMethod*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->inheritation=typedResource->inheritation;
											typedSymbol->memberType=typedResource->memberType;
											typedSymbol->returnType=GetType(typedResource->returnType);
											typedSymbol->implementedInterfaceType=GetType(typedResource->implementedInterfaceType);
											FillList(typedSymbol->orderedGenericParameterNames, typedResource->orderedGenericParameterNames);
											FillList(typedSymbol->orderedMethodParameterNames, typedResource->orderedMethodParameterNames);
										}
										break;
									case ManagedSymbolItemRes::Constructor:
										{
											ResourceRecord<ManagedSymbolConstructorRes> typedResource=resource.Cast<ManagedSymbolConstructorRes>();
											ManagedSymbolConstructor* typedSymbol=dynamic_cast<ManagedSymbolConstructor*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->implicit=typedResource->implicit;
											FillList(typedSymbol->orderedMethodParameterNames, typedResource->orderedMethodParameterNames);
										}
										break;
									case ManagedSymbolItemRes::ConverterOperator:
										{
											ResourceRecord<ManagedSymbolConverterOperatorRes> typedResource=resource.Cast<ManagedSymbolConverterOperatorRes>();
											ManagedSymbolConverterOperator* typedSymbol=dynamic_cast<ManagedSymbolConverterOperator*>(symbol);
											typedSymbol->accessor=typedResource->accessor;
											typedSymbol->inheritation=typedResource->inheritation;
											typedSymbol->memberType=typedResource->memberType;
											typedSymbol->targetType=GetType(typedResource->targetType);
											typedSymbol->implicit=typedResource->implicit;
											FillList(typedSymbol->orderedGenericParameterNames, typedResource->orderedGenericParameterNames);
										}
										break;
									case ManagedSymbolItemRes::GenericParameter:
										{
											ResourceRecord<ManagedSymbolGenericParameterRes> typedResource=resource.Cast<ManagedSymbolGenericParameterRes>();
											ManagedSymbolGenericParameter* typedSymbol=dynamic_cast<ManagedSymbolGenericParameter*>(symbol);
											typedSymbol->conversion=typedResource->conversion;
											typedSymbol->newConstraint=typedResource->newConstraint;
											FillList(typedSymbol->typeConstraints, typedResource->typeConstraints);
										}
										break;
									case ManagedSymbolItemRes::MethodParameter:
										{
											ResourceRecord<ManagedSymbolMethodParameterRes> typedResource=resource.Cast<ManagedSymbolMethodParameterRes>();
											ManagedSymbolMethodParameter* typedSymbol=dynamic_cast<ManagedSymbolMethodParameter*>(symbol);
											typedSymbol->parameterType=typedResource->parameterType;
											typedSymbol->containsDefaultValue=typedResource->containsDefaultValue;
											typedSymbol->type=GetType(typedResource->type);
										}
										break;
									}
									if(resource->subItems)
									{
										FillSymbol(symbol, resource->subItems);
									}
								}
							}
						}
					}

				public:
					SymbolImporter(Ptr<ResourceStream> _resourceStream, ManagedSymbolManager* _symbolManager, const WString& _importAssemblyName, bool _includingInternal)
						:resourceStream(_resourceStream)
						,symbolManager(_symbolManager)
						,importAssemblyName(_importAssemblyName)
						,includingInternal(_includingInternal)
					{
					}

					void CheckExternalDeclarations(List<ResourceHandle<ManagedSymbolItemRes>>& unresolvedTypes)
					{
						ResourceRecord<ManagedEntryRes> entryRes=resourceStream->ReadRootRecord<ManagedEntryRes>();
						ResourceRecord<ManagedSymbolItemRes> globalResource=resourceStream->ReadRecord(entryRes->globalNamespace);
						CheckExternalDeclaration(symbolManager->Global(), globalResource->subItems, unresolvedTypes);
					}

					void CreateSymbols()
					{
						ResourceRecord<ManagedEntryRes> entryRes=resourceStream->ReadRootRecord<ManagedEntryRes>();
						ResourceRecord<ManagedSymbolItemRes> globalResource=resourceStream->ReadRecord(entryRes->globalNamespace);
						CreateSymbol(symbolManager->Global(), globalResource->subItems);
						FillSymbol(symbolManager->Global(), globalResource->subItems);
					}
				};
			};
			using namespace resource_transformation;

			bool ManagedLanguage_ImportSymbols(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager, const WString& assemblyName, bool includingInternal, List<ResourceHandle<ManagedSymbolItemRes>>& unresolvedTypes)
			{
				unresolvedTypes.Clear();
				SymbolImporter symbolImporter(resourceStream, symbolManager, assemblyName, includingInternal);
				symbolImporter.CheckExternalDeclarations(unresolvedTypes);
				if(unresolvedTypes.Count()>0)
				{
					return false;
				}
				symbolImporter.CreateSymbols();
				return true;
			}
		}
	}
}