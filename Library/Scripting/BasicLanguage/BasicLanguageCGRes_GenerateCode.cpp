#include "BasicLanguageCodeGeneration.h"
#include "BasicLanguageResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicLanguage_GenerateResource
***********************************************************************/

			ResourceHandle<BasicTypeRes> GenerateResource(BasicTypeRecord* type, const BCP& argument)
			{
				if(ResourceHandle<BasicTypeRes> typeResource=argument.info->GetTypeResource(type))
				{
					return typeResource;
				}
				else
				{
					ResourceRecord<BasicTypeRes> resource=argument.resource->CreateRecord<BasicTypeRes>();
					argument.info->SetTypeResource(type, resource);

					BasicTypeInfo* typeInfo=argument.info->GetTypeInfo(type);
					TODO_FOR_GENERIC_FUNCTION_BEGIN
						resource->size=typeInfo->size.Constant();
					TODO_FOR_GENERIC_FUNCTION_END
					resource->genericArgumentName=ResourceString::Null();
					switch(type->GetType())
					{
					case BasicTypeRecord::Primitive:
						{
							resource->type=BasicTypeRes::Primitive;
							resource->elementType=ResourceHandle<BasicTypeRes>::Null();
							resource->elementCount=-1;
							resource->subTypes=ResourceArrayHandle<BasicSubTypeRes>::Null();
							switch(type->PrimitiveType())
							{
							case s8:resource->primitiveType=BasicTypeRes::s8;break;
							case s16:resource->primitiveType=BasicTypeRes::s16;break;
							case s32:resource->primitiveType=BasicTypeRes::s32;break;
							case s64:resource->primitiveType=BasicTypeRes::s64;break;
							case u8:resource->primitiveType=BasicTypeRes::u8;break;
							case u16:resource->primitiveType=BasicTypeRes::u16;break;
							case u32:resource->primitiveType=BasicTypeRes::u32;break;
							case u64:resource->primitiveType=BasicTypeRes::u64;break;
							case f32:resource->primitiveType=BasicTypeRes::f32;break;
							case f64:resource->primitiveType=BasicTypeRes::f64;break;
							case bool_type:resource->primitiveType=BasicTypeRes::bool_type;break;
							case void_type:resource->primitiveType=BasicTypeRes::void_type;break;
							case char_type:resource->primitiveType=BasicTypeRes::char_type;break;
							case wchar_type:resource->primitiveType=BasicTypeRes::wchar_type;break;
							}
						}
						break;
					case BasicTypeRecord::Pointer:
						{
							ResourceHandle<BasicTypeRes> elementType=GenerateResource(type->ElementType(), argument);
							resource->type=BasicTypeRes::Pointer;
							resource->elementType=elementType;
							resource->elementCount=-1;
							resource->subTypes=ResourceArrayHandle<BasicSubTypeRes>::Null();
							resource->primitiveType=BasicTypeRes::void_type;
						}
						break;
					case BasicTypeRecord::Array:
						{
							ResourceHandle<BasicTypeRes> elementType=GenerateResource(type->ElementType(), argument);
							resource->type=BasicTypeRes::Array;
							resource->elementType=elementType;
							resource->elementCount=type->ElementCount();
							resource->subTypes=ResourceArrayHandle<BasicSubTypeRes>::Null();
							resource->primitiveType=BasicTypeRes::void_type;
						}
						break;
					case BasicTypeRecord::Function:
						{
							ResourceHandle<BasicTypeRes> elementType=GenerateResource(type->ReturnType(), argument);
							resource->type=BasicTypeRes::Function;
							resource->elementType=elementType;
							resource->elementCount=-1;
							resource->primitiveType=BasicTypeRes::void_type;

							ResourceArrayRecord<BasicSubTypeRes> subTypes=argument.resource->CreateArrayRecord<BasicSubTypeRes>(type->ParameterCount());
							resource->subTypes=subTypes;
							for(vint i=0;i<type->ParameterCount();i++)
							{
								ResourceHandle<BasicTypeRes> parameterType=GenerateResource(type->ParameterType(i), argument);
								ResourceRecord<BasicSubTypeRes> parameter=argument.resource->CreateRecord<BasicSubTypeRes>();
								parameter->name=ResourceString::Null();
								parameter->type=parameterType;
								parameter->offset=-1;
								subTypes.Set(i, parameter);
							}
						}
						break;
					case BasicTypeRecord::Structure:
						{
							resource->type=BasicTypeRes::Structure;
							resource->elementType=ResourceHandle<BasicTypeRes>::Null();
							resource->elementCount=-1;
							resource->primitiveType=BasicTypeRes::void_type;
							
							ResourceArrayRecord<BasicSubTypeRes> subTypes=argument.resource->CreateArrayRecord<BasicSubTypeRes>(type->MemberCount());
							resource->subTypes=subTypes;
							for(vint i=0;i<type->MemberCount();i++)
							{
								ResourceHandle<BasicTypeRes> memberType=GenerateResource(type->MemberType(i), argument);
								ResourceString memberName=argument.resource->CreateString(type->MemberName(i));
								ResourceRecord<BasicSubTypeRes> member=argument.resource->CreateRecord<BasicSubTypeRes>();
								member->name=memberName;
								member->type=memberType;
								TODO_FOR_GENERIC_FUNCTION_BEGIN
									member->offset=typeInfo->offsets[i].Constant();
								TODO_FOR_GENERIC_FUNCTION_END
								subTypes.Set(i, member);
							}
						}
						break;
					case BasicTypeRecord::GenericArgument:
						{
							resource->type=BasicTypeRes::GenericArgument;
							resource->elementType=ResourceHandle<BasicTypeRes>::Null();
							resource->elementCount=-1;
							resource->subTypes=ResourceArrayHandle<BasicSubTypeRes>::Null();
							resource->genericArgumentName=argument.resource->CreateString(type->ArgumentName());
						}
						break;
					case BasicTypeRecord::Generic:
						{
							return GenerateResource(type->ElementType(), argument);
						}
						break;
					}
					return resource;
				}
			}

			void BuildGenericResource(ResourceRecord<BasicDeclarationRes> resource, BasicDeclaration* node, const BCP& argument)
			{
				ResourceArrayRecord<BasicParameterRes> parameters=argument.resource->CreateArrayRecord<BasicParameterRes>(node->genericDeclaration.arguments.Count());
				resource->genericArgumentNames=parameters;
				for(vint i=0;i<node->genericDeclaration.arguments.Count();i++)
				{
					ResourceRecord<BasicParameterRes> parameter=argument.resource->CreateRecord<BasicParameterRes>();
					parameter->name=argument.resource->CreateString(node->genericDeclaration.arguments[i]);
					parameters.Set(i, parameter);
				}
			}

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GenerateResource, BasicDeclaration, BCP, ResourceHandle<BasicDeclarationRes>)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER
			
				ALGORITHM_FUNCTION_MATCH(BasicFunctionDeclaration)
				{
					ResourceRecord<BasicDeclarationRes> resource=argument.resource->CreateRecord<BasicDeclarationRes>();
					BuildGenericResource(resource, node, argument);
					ResourceString name=argument.resource->CreateString(node->name);
					BasicTypeRecord* type=argument.info->GetEnv()->GetFunctionType(node);
					ResourceHandle<BasicTypeRes> declarationType=GenerateResource(type, argument);

					resource->type=BasicDeclarationRes::Function;
					resource->declarationType=declarationType;
					resource->name=name;
					if(node->linking.HasLink())
					{
						resource->address=-1;
					}
					else
					{
						resource->address=argument.il->labels[argument.info->GetFunctions()[node]].instructionIndex;
					}

					ResourceArrayRecord<BasicParameterRes> parameters=argument.resource->CreateArrayRecord<BasicParameterRes>(node->parameterNames.Count());
					resource->parameterNames=parameters;
					for(vint i=0;i<node->parameterNames.Count();i++)
					{
						ResourceString name=argument.resource->CreateString(node->parameterNames[i]);
						ResourceRecord<BasicParameterRes> parameter=argument.resource->CreateRecord<BasicParameterRes>();
						parameter->name=name;
						parameters.Set(i, parameter);
					}

					if(node->linking.HasLink())
					{
						resource->linkingAssemblyName=argument.resource->CreateString(node->linking.assemblyName);
						resource->linkingSymbolName=argument.resource->CreateString(node->linking.symbolName);
					}
					else
					{
						resource->linkingAssemblyName=ResourceString::Null();
						resource->linkingSymbolName=ResourceString::Null();
					}

					return resource;
				}

				ALGORITHM_FUNCTION_MATCH(BasicVariableDeclaration)
				{
					ResourceRecord<BasicDeclarationRes> resource=argument.resource->CreateRecord<BasicDeclarationRes>();
					BuildGenericResource(resource, node, argument);
					ResourceString name=argument.resource->CreateString(node->name);
					BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->variables.Find(node->name).type;
					ResourceHandle<BasicTypeRes> declarationType=GenerateResource(type, argument);

					resource->type=BasicDeclarationRes::Variable;
					resource->declarationType=declarationType;
					resource->name=name;
					resource->parameterNames=ResourceArrayHandle<BasicParameterRes>::Null();
					resource->address=argument.info->GetGlobalVariableOffsets()[node];

					if(node->linking.HasLink())
					{
						resource->linkingAssemblyName=argument.resource->CreateString(node->linking.assemblyName);
						resource->linkingSymbolName=argument.resource->CreateString(node->linking.symbolName);
					}
					else
					{
						resource->linkingAssemblyName=ResourceString::Null();
						resource->linkingSymbolName=ResourceString::Null();
					}

					return resource;
				}

				ALGORITHM_FUNCTION_MATCH(BasicTypeRenameDeclaration)
				{
					return ResourceHandle<BasicDeclarationRes>::Null();
				}

				ALGORITHM_FUNCTION_MATCH(BasicStructureDeclaration)
				{
					if(node->defined)
					{
						ResourceRecord<BasicDeclarationRes> resource=argument.resource->CreateRecord<BasicDeclarationRes>();
						BuildGenericResource(resource, node, argument);
						ResourceString name=argument.resource->CreateString(node->name);
						BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->types.Find(node->name);
						ResourceHandle<BasicTypeRes> declarationType=GenerateResource(type, argument);

						resource->type=BasicDeclarationRes::Structure;
						resource->declarationType=declarationType;
						resource->name=name;
						resource->parameterNames=ResourceArrayHandle<BasicParameterRes>::Null();
						resource->address=-1;

						if(node->linking.HasLink())
						{
							resource->linkingAssemblyName=argument.resource->CreateString(node->linking.assemblyName);
							resource->linkingSymbolName=argument.resource->CreateString(node->linking.symbolName);
						}
						else
						{
							resource->linkingAssemblyName=ResourceString::Null();
							resource->linkingSymbolName=ResourceString::Null();
						}

						return resource;
					}
					else
					{
						return ResourceHandle<BasicDeclarationRes>::Null();
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedDeclaration)
				{
					return argument.codegenExtension->GenerateResource(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GenerateResource)

/***********************************************************************
BasicLanguage_GenerateExport
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GenerateExport, BasicDeclaration, BCP, ResourceHandle<BasicILExportRes>)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER
			
				ALGORITHM_FUNCTION_MATCH(BasicFunctionDeclaration)
				{
					if(node->linking.HasLink())
					{
						return ResourceHandle<BasicILExportRes>::Null();
					}
					else
					{
						ResourceRecord<BasicILExportRes> exportRes=argument.exportResource->CreateRecord<BasicILExportRes>();
						exportRes->address=argument.il->labels[argument.info->GetFunctions()[node]].instructionIndex;
						exportRes->name=argument.exportResource->CreateString(node->name);
						return exportRes;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicVariableDeclaration)
				{
					if(node->linking.HasLink())
					{
						return ResourceHandle<BasicILExportRes>::Null();
					}
					else
					{
						ResourceRecord<BasicILExportRes> exportRes=argument.exportResource->CreateRecord<BasicILExportRes>();
						exportRes->address=argument.info->GetGlobalVariableOffsets()[node];
						exportRes->name=argument.exportResource->CreateString(node->name);
						return exportRes;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicTypeRenameDeclaration)
				{
					return ResourceHandle<BasicILExportRes>::Null();
				}

				ALGORITHM_FUNCTION_MATCH(BasicStructureDeclaration)
				{
					return ResourceHandle<BasicILExportRes>::Null();
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedDeclaration)
				{
					return ResourceHandle<BasicILExportRes>::Null();
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GenerateExport)
		}
	}
}