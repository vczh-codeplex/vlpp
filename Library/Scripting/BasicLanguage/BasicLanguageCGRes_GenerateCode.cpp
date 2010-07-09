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
			using namespace collections;

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
					if(typeInfo->size.IsConstant())
					{
						resource->size=typeInfo->size.Constant();
					}
					else
					{
						resource->size=-1;
					}
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
								if(type->MemberType(i)->GetType()==BasicTypeRecord::GenericArgument)
								{
									member->offset=-1;
								}
								else if(typeInfo->offsets[i].IsConstant())
								{
									member->offset=typeInfo->offsets[i].Constant();
								}
								else
								{
									member->offset=-1;
								}
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
					if(node->genericDeclaration.HasGeneric())
					{
						return ResourceHandle<BasicDeclarationRes>::Null();
					}
					else
					{
						ResourceRecord<BasicDeclarationRes> resource=argument.resource->CreateRecord<BasicDeclarationRes>();
						BuildGenericResource(resource, node, argument);
						ResourceString name=argument.resource->CreateString(node->name);
						BasicTypeRecord* type=argument.info->GetEnv()->GetFunctionType(node, true);
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
					if(node->linking.HasLink() || node->genericDeclaration.HasGeneric())
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

/***********************************************************************
BasicLanguage_Generate*Resource
***********************************************************************/

			WString EscapeResourceName(const WString& name)
			{
				Array<wchar_t> buffer(name.Length()*2+3);
				vint index=0;
				const wchar_t* reading=name.Buffer();
				wchar_t* writing=&buffer[0];
				*writing++=L'[';
				while(*reading)
				{
					switch(*reading)
					{
					case L':':case L'<':case L'>':case L'[':case L']':case L' ':case L'\\':
						*writing++=L'\\';
						*writing++=*reading;
						break;
					default:
						*writing++=*reading;
					}
					reading++;
				}
				*writing++=L']';
				*writing=0;
				return WString(&buffer[0]);
			}

			class TypeUniqueStringRecorder
			{
				typedef collections::Dictionary<BasicTypeRecord*, BasicStructureDeclaration*> _RegisteredTypeMap;
			protected:
				BasicFunctionDeclaration*						currentFunction;
				List<ResourceHandle<BasicILGenericNameRes>>		names;
				WString											currentString;
				_RegisteredTypeMap								map;

				Ptr<ResourceStream>								resource;
				Ptr<BasicProgram>								program;
				BasicEnv*										env;
				WString											assemblyName;

				void Submit()
				{
					if(currentString!=L"")
					{
						ResourceRecord<BasicILGenericNameRes> name=resource->CreateRecord<BasicILGenericNameRes>();
						name->constantString=resource->CreateString(currentString);
						name->isConstant=true;
						name->stringArgumentIndex=-1;
						names.Add(name);
						currentString=L"";
					}
				}
			public:
				TypeUniqueStringRecorder(Ptr<ResourceStream> _resource, Ptr<BasicProgram> _program, BasicEnv* _env, const WString& _assemblyName)
					:resource(_resource)
					,program(_program)
					,env(_env)
					,assemblyName(_assemblyName)
					,currentFunction(0)
				{
					for(vint i=0;i<program->declarations.Count();i++)
					{
						BasicStructureDeclaration* declaration=dynamic_cast<BasicStructureDeclaration*>(program->declarations[i].Obj());
						if(declaration)
						{
							BasicTypeRecord* type=env->GetStructureType(declaration);
							if(type)
							{
								map.Add(type, declaration);
							}
						}
					}
				}

				void Start(BasicFunctionDeclaration* function)
				{
					currentFunction=function;
					currentString=L"";
					names.Clear();
				}

				void AppendString(const WString& string)
				{
					currentString+=string;
				}

				void AppendParameter(BasicTypeRecord* genericArgument)
				{
					Submit();
					ResourceRecord<BasicILGenericNameRes> name=resource->CreateRecord<BasicILGenericNameRes>();
					name->constantString=ResourceString::Null();
					name->isConstant=false;
					name->stringArgumentIndex=currentFunction->genericDeclaration.arguments.IndexOf(genericArgument->ArgumentName());
					names.Add(name);
				}

				void AppendRegistered(BasicTypeRecord* structure)
				{
					BasicStructureDeclaration* declaration=map[structure];
					if(declaration->linking.HasLink())
					{
						AppendString(EscapeResourceName(declaration->linking.assemblyName));
						AppendString(L"::");
						AppendString(EscapeResourceName(declaration->linking.symbolName));
					}
					else
					{
						AppendString(EscapeResourceName(assemblyName));
						AppendString(L"::");
						AppendString(EscapeResourceName(declaration->name));
					}
				}

				ResourceArrayHandle<BasicILGenericNameRes> End()
				{
					Submit();
					return resource->CreateArrayRecord(names.Wrap());
				}
			};

			void GetTypeUniqueString(BasicTypeRecord* type, TypeUniqueStringRecorder& recorder)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Primitive:
					{
						switch(type->PrimitiveType())
						{
						case s8:
							recorder.AppendString(L"s8");
							break;
						case s16:
							recorder.AppendString(L"s16");
							break;
						case s32:
							recorder.AppendString(L"s32");
							break;
						case s64:
							recorder.AppendString(L"s64");
							break;
						case u8:
							recorder.AppendString(L"u8");
							break;
						case u16:
							recorder.AppendString(L"u16");
							break;
						case u32:
							recorder.AppendString(L"u32");
							break;
						case u64:
							recorder.AppendString(L"u64");
							break;
						case f32:
							recorder.AppendString(L"f32");
							break;
						case f64:
							recorder.AppendString(L"f64");
							break;
						case bool_type:
							recorder.AppendString(L"bool_type");
							break;
						case void_type:
							recorder.AppendString(L"void_type");
							break;
						case char_type:
							recorder.AppendString(L"char_type");
							break;
						case wchar_type:
							recorder.AppendString(L"wchar_type");
							break;
						}
					}
					break;
				case BasicTypeRecord::Pointer:
					{
						recorder.AppendString(L"pointer<");
						GetTypeUniqueString(type->ElementType(), recorder);
						recorder.AppendString(L">");
					}
					break;
				case BasicTypeRecord::Array:
					{
						recorder.AppendString(L"array<");
						GetTypeUniqueString(type->ElementType(), recorder);
						recorder.AppendString(L", ");
						recorder.AppendString(itow(type->ElementCount()));
						recorder.AppendString(L">");
					}
					break;
				case BasicTypeRecord::Function:
					{
						recorder.AppendString(L"function<");
						GetTypeUniqueString(type->ReturnType(), recorder);
						for(vint i=0;i<type->ParameterCount();i++)
						{
							recorder.AppendString(L", ");
							GetTypeUniqueString(type->ParameterType(i), recorder);
						}
						recorder.AppendString(L">");
					}
					break;
				case BasicTypeRecord::Structure:
					{
						BasicGenericStructureProxyTypeRecord* proxy=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
						if(proxy)
						{
							GetTypeUniqueString(proxy->UninstanciatedStructureType(), recorder);
							recorder.AppendString(L"<");
							vint count=proxy->GenericArgumentMap().Count();
							for(vint i=0;i<count;i++)
							{
								if(i)recorder.AppendString(L", ");
								BasicTypeRecord* key=proxy->UninstanciatedStructureType()->ParameterType(i);
								GetTypeUniqueString(proxy->GenericArgumentMap()[key], recorder);
							}
							recorder.AppendString(L">");
						}
						else
						{
							recorder.AppendRegistered(type);
						}
					}
					break;
				case BasicTypeRecord::GenericArgument:
					{
						recorder.AppendParameter(type);
					}
					break;
				case BasicTypeRecord::Generic:
					{
						recorder.AppendRegistered(type);
					}
					break;
				}
			}

			ResourceArrayHandle<BasicILGenericFunctionEntryRes> BasicLanguage_GenerateFunctionEntryResource(const WString& programName, const BCP& argument)
			{
				List<ResourceHandle<BasicILGenericFunctionEntryRes>> entries;
				for(vint i=0;i<argument.info->instanciatedGenericFunctionEntries.Count();i++)
				{
					Ptr<BasicCodegenInfo::FunctionEntry> entry=argument.info->instanciatedGenericFunctionEntries[i];
					if(entry->declaration && entry->declaration->genericDeclaration.HasGeneric() && !entry->declaration->linking.HasLink())
					{
						ResourceRecord<BasicILGenericFunctionEntryRes> entryResource=argument.exportResource->CreateRecord<BasicILGenericFunctionEntryRes>();
						entryResource->genericArgumentCount=entry->declaration->genericDeclaration.arguments.Count();
						entryResource->name=argument.exportResource->CreateString(entry->declaration->name);
						entryResource->startInstruction=entry->startInstruction;
						entryResource->instructionCount=entry->instructionCount;

						List<ResourceHandle<BasicILGenericNameRes>> names;
						ResourceRecord<BasicILGenericNameRes> currentName=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
						currentName->constantString=argument.exportResource->CreateString(EscapeResourceName(programName)+L"::"+EscapeResourceName(entry->declaration->name)+L"<");
						currentName->isConstant=true;
						currentName->stringArgumentIndex=-1;
						names.Add(currentName);

						for(vint j=0;j<entry->declaration->genericDeclaration.arguments.Count();j++)
						{
							currentName=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
							currentName->constantString=ResourceString::Null();
							currentName->isConstant=false;
							currentName->stringArgumentIndex=j;
							names.Add(currentName);
							
							if(j)
							{
								currentName=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
								currentName->constantString=argument.exportResource->CreateString(L", ");
								currentName->isConstant=true;
								currentName->stringArgumentIndex=-1;
								names.Add(currentName);
							}
						}
						
						currentName=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
						currentName->constantString=argument.exportResource->CreateString(L">");
						currentName->isConstant=true;
						currentName->stringArgumentIndex=-1;
						names.Add(currentName);

						entryResource->uniqueNameTemplate=argument.exportResource->CreateArrayRecord(names.Wrap());
						entries.Add(entryResource);
					}
				}
				return argument.exportResource->CreateArrayRecord(entries.Wrap());
			}

			ResourceArrayHandle<BasicILGenericFunctionTargetRes> BasicLanguage_GenerateFunctionTargetResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				TypeUniqueStringRecorder recorder(argument.exportResource, program, argument.info->GetEnv(), programName);
				List<ResourceHandle<BasicILGenericFunctionTargetRes>> targets;
				for(vint i=0;i<argument.info->instanciatedGenericFunctions.Count();i++)
				{
					BasicCodegenInfo::FunctionTarget* target=argument.info->instanciatedGenericFunctions[i].Obj();
					ResourceRecord<BasicILGenericFunctionTargetRes> targetResource=argument.exportResource->CreateRecord<BasicILGenericFunctionTargetRes>();
					WString assemblyName;
					WString symbolName;
					if(target->declaration->linking.HasLink())
					{
						assemblyName=target->declaration->linking.assemblyName;
						symbolName=target->declaration->linking.symbolName;
					}
					else
					{
						assemblyName=programName;
						symbolName=target->declaration->name;
					}

					List<ResourceHandle<BasicILGenericArgumentRes>> arguments;
					for(vint j=0;j<target->genericParameters.Count();j++)
					{
						BasicTypeRecord* type=target->genericParameters[j];
						const BasicOffset& offset=argument.info->GetTypeInfo(type)->size;
						
						ResourceRecord<BasicILGenericLinearRes> linearRecord=argument.exportResource->CreateRecord<BasicILGenericLinearRes>();
						{
							linearRecord->constant=offset.Constant();
							vint count=target->currentDeclaration->genericDeclaration.arguments.Count();
							ResourceArrayRecord<BasicILGenericFactorItemRes> factors=argument.exportResource->CreateArrayRecord<BasicILGenericFactorItemRes>(count);
							for(vint j=0;j<count;j++)
							{
								ResourceRecord<BasicILGenericFactorItemRes> factor=argument.exportResource->CreateRecord<BasicILGenericFactorItemRes>();
								WString genericArgumentName=target->currentDeclaration->genericDeclaration.arguments[j];
								BasicTypeRecord* genericArgumentType=argument.info->GetTypeManager()->GetGenericArgumentType(genericArgumentName);
								factor->factor=offset.Factor(genericArgumentType);
								factors[j]=factor;
							}
							linearRecord->factors=factors;
						}

						ResourceRecord<BasicILGenericArgumentRes> argumentRecord=argument.exportResource->CreateRecord<BasicILGenericArgumentRes>();
						{
							argumentRecord->sizeArgument=linearRecord;
							recorder.Start(target->currentDeclaration);
							GetTypeUniqueString(type, recorder);
							argumentRecord->nameArgument=recorder.End();
						}
						arguments.Add(argumentRecord);
					}

					targetResource->assemblyName=argument.exportResource->CreateString(assemblyName);
					targetResource->symbolName=argument.exportResource->CreateString(symbolName);
					targetResource->arguments=argument.exportResource->CreateArrayRecord(arguments.Wrap());
					targets.Add(targetResource);
				}
				return argument.exportResource->CreateArrayRecord(targets.Wrap());
			}

			ResourceArrayHandle<BasicILGenericLinearRes> BasicLanguage_GenerateLinearResource(const BCP& argument)
			{
				List<ResourceHandle<BasicILGenericLinearRes>> linears;
				for(vint i=0;i<argument.info->instanciatedGenericLinears.Count();i++)
				{
					BasicCodegenInfo::FunctionLinear& linear=argument.info->instanciatedGenericLinears[i];
					ResourceRecord<BasicILGenericLinearRes> linearRecord=argument.exportResource->CreateRecord<BasicILGenericLinearRes>();
					linearRecord->constant=linear.Constant();

					vint count=linear.Count();
					ResourceArrayRecord<BasicILGenericFactorItemRes> factors=argument.exportResource->CreateArrayRecord<BasicILGenericFactorItemRes>(count);
					for(vint j=0;j<count;j++)
					{
						ResourceRecord<BasicILGenericFactorItemRes> factor=argument.exportResource->CreateRecord<BasicILGenericFactorItemRes>();
						factor->factor=linear.Factor(j);
						factors[j]=factor;
					}

					linearRecord->factors=factors;
					linears.Add(linearRecord);
				}
				return argument.exportResource->CreateArrayRecord(linears.Wrap());
			}

			ResourceHandle<BasicILGenericRes> BasicLanguage_GenerateGenericResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				ResourceRecord<BasicILGenericRes> genericRes=argument.exportResource->CreateRecord<BasicILGenericRes>();
				genericRes->functionEntries=BasicLanguage_GenerateFunctionEntryResource(programName, argument);
				genericRes->functionTargets=BasicLanguage_GenerateFunctionTargetResource(program, programName, argument);
				genericRes->linears=BasicLanguage_GenerateLinearResource(argument);
				return genericRes;
			}

			ResourceArrayHandle<BasicDeclarationRes> BasicLanguage_GenerateDeclarationResource(const Ptr<BasicProgram> program, const BCP& argument)
			{
				List<ResourceHandle<BasicDeclarationRes>> declarations;
				for(vint i=0;i<program->declarations.Count();i++)
				{
					ResourceHandle<BasicDeclarationRes> declaration=BasicLanguage_GenerateResource(program->declarations[i], argument);
					if(declaration)
					{
						declarations.Add(declaration);
					}
				}
				return argument.resource->CreateArrayRecord(declarations.Wrap());
			}

			ResourceArrayHandle<BasicILExportRes> BasicLanguage_GenerateExportResource(const Ptr<BasicProgram> program, const BCP& argument)
			{
				List<ResourceHandle<BasicILExportRes>> exports;
				for(vint i=0;i<program->declarations.Count();i++)
				{
					ResourceHandle<BasicILExportRes> exportRes=BasicLanguage_GenerateExport(program->declarations[i], argument);
					if(exportRes)
					{
						exports.Add(exportRes);
					}
				}
				return argument.exportResource->CreateArrayRecord(exports.Wrap());
			}

			ResourceArrayHandle<BasicILLinkingRes> BasicLanguage_GenerateLinkingResource(const Ptr<BasicProgram> program, const BCP& argument)
			{
				List<ResourceHandle<BasicILLinkingRes>> linkings;
				ResourceRecord<BasicILLinkingRes> currentLinking;
				for(vint i=0;i<argument.info->linkings.Count();i++)
				{
					BasicLinking& linking=argument.info->linkings[i];
					ResourceRecord<BasicILLinkingRes> linkingRecord=argument.exportResource->CreateRecord<BasicILLinkingRes>();
					linkingRecord->assemblyName=argument.exportResource->CreateString(linking.assemblyName);
					linkingRecord->symbolName=argument.exportResource->CreateString(linking.symbolName);
					linkings.Add(linkingRecord);
				}
				return argument.exportResource->CreateArrayRecord(linkings.Wrap());
			}
		}
	}
}