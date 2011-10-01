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
					Dictionary<ManagedSymbolItem*, ResourceHandle<ManagedSymbolItemRes>>	symbolItems;
					Dictionary<ManagedTypeSymbol*, ResourceHandle<ManagedTypeSymbolRes>>	symbolTypes;
					Ptr<ResourceStream>														resourceStream;
					ManagedSymbolManager*													symbolManager;

				protected:

					void CollectType(ManagedTypeSymbol* typeSymbol)
					{
					}

					template<typename TSymbol, typename TResource>
					void CollectSymbol(TSymbol* symbol, ManagedSymbolItemRes::SymbolType symbolType)
					{
						ResourceRecord<TResource> resource=resourceStream->CreateRecord<TResource>();
						ResourceHandle<TResource> resourceHandle=resource;
						symbolItems.Add(symbol, resourceHandle.Cast<ManagedSymbolItemRes>());

						resource->symbolType=symbolType;
						resource->parentGroup=ResourceHandle<ManagedSymbolItemGroupRes>::Null();
						resource->name=resourceStream->CreateString(symbol->GetName());
						resource->itemGroups=ResourceArrayHandle<ManagedSymbolItemGroupRes>::Null();
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
										if(forceAccept)
										{
											externalDeclaration=true;
										}
										else
										{
											accepted=false;
										}
									}
									if(accepted)
									{
										CollectSymbol<ManagedSymbolDeclaration, ManagedSymbolDeclarationRes>(typedSymbol, (externalDeclaration?ManagedSymbolItemRes::ExternalDeclaration:ManagedSymbolItemRes::TypeDeclaration));
										if(!externalDeclaration)
										{
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
										CollectSymbol(item, forceAccept);
									}
								}
							}
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