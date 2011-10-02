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
									if(typedSymbol->_baseType)
									{
										typedResource->_basicType=symbolTypes[typedSymbol->_baseType];
									}
									GenerateArray(typedResource->_baseInterfaces, typedSymbol->_baseInterfaces);
								}
								else
								{
									ResourceRecord<ManagedSymbolExternalDeclarationRes> typedResource=resource.Cast<ManagedSymbolExternalDeclarationRes>();
									ResourceString name=resourceStream->CreateString(typedSymbol->foreignAssemblyName);
									typedResource->foreignAssemblyName=name;
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
								if(typedSymbol->implementedInterfaceType)
								{
									typedResource->implementedInterfaceType=symbolTypes[typedSymbol->implementedInterfaceType];
								}
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
								if(typedSymbol->returnType)
								{
									typedResource->returnType=symbolTypes[typedSymbol->returnType];
								}
								if(typedSymbol->implementedInterfaceType)
								{
									typedResource->implementedInterfaceType=symbolTypes[typedSymbol->implementedInterfaceType];
								}
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
			};
			using namespace resource_transformation;

			void ManagedLanguage_ImportSymbols(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager, const WString& assemblyName)
			{
			}
		}
	}
}