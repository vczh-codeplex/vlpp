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
					if(node->linking.HasLink() || node->genericDeclaration.HasGeneric())
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
				TypeUniqueStringRecorder(Ptr<ResourceStream> _resource, Ptr<BasicProgram> _program, BasicEnv* _env, const WString& _assemblyName, BasicFunctionDeclaration* function)
					:resource(_resource)
					,program(_program)
					,env(_env)
					,assemblyName(_assemblyName)
					,currentFunction(function)
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

				WString GetRegisteredType(BasicTypeRecord* structure)
				{
					BasicStructureDeclaration* declaration=map[structure];
					WString resultAssemblyName;
					WString resultSymbolName;
					if(declaration->linking.HasLink())
					{
						resultAssemblyName=declaration->linking.assemblyName;
						resultSymbolName=declaration->linking.symbolName;
					}
					else
					{
						resultAssemblyName=assemblyName;
						resultSymbolName=declaration->name;
					}
					return EscapeResourceName(resultAssemblyName)+L"::"+EscapeResourceName(resultSymbolName);
				}

				WString GetTypeName(BasicTypeRecord* type, List<BasicTypeRecord*>& argumentTypes)
				{
					argumentTypes.Clear();
					switch(type->GetType())
					{
					case BasicTypeRecord::Primitive:
						{
							switch(type->PrimitiveType())
							{
							case s8:
								return L"s8";
								break;
							case s16:
								return L"s16";
								break;
							case s32:
								return L"s32";
								break;
							case s64:
								return L"s64";
								break;
							case u8:
								return L"u8";
								break;
							case u16:
								return L"u16";
								break;
							case u32:
								return L"u32";
								break;
							case u64:
								return L"u64";
								break;
							case f32:
								return L"f32";
								break;
							case f64:
								return L"f64";
								break;
							case bool_type:
								return L"bool_type";
								break;
							case void_type:
								return L"void_type";
								break;
							case char_type:
								return L"char_type";
								break;
							case wchar_type:
								return L"wchar_type";
								break;
							default:
								CHECK_ERROR(false, L"vl::scripting::basiclanguage::TypeUniqueStringRecorder::GetTypeName#内部错误。");
								return L"";
							}
						}
						break;
					case BasicTypeRecord::Pointer:
						{
							argumentTypes.Add(type->ElementType());
							return L"pointer";
						}
						break;
					case BasicTypeRecord::Array:
						{
							argumentTypes.Add(type->ElementType());
							return L"array[itow(type->ElementCount())]";
						}
						break;
					case BasicTypeRecord::Function:
						{
							argumentTypes.Add(type->ReturnType());
							for(vint i=0;i<type->ParameterCount();i++)
							{
								argumentTypes.Add(type->ParameterType(i));
							}
							return L"function";
						}
						break;
					case BasicTypeRecord::Structure:
						{
							BasicGenericStructureProxyTypeRecord* proxy=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
							if(proxy)
							{
								vint count=proxy->GenericArgumentMap().Count();
								for(vint i=0;i<count;i++)
								{
									BasicTypeRecord* key=proxy->UninstanciatedStructureType()->ParameterType(i);
									argumentTypes.Add(proxy->GenericArgumentMap()[key]);
								}
								return GetRegisteredType(proxy->UninstanciatedStructureType());
							}
							else
							{
								return GetRegisteredType(type);
							}
						}
						break;
					default:
						CHECK_ERROR(false, L"vl::scripting::basiclanguage::TypeUniqueStringRecorder::GetTypeName#内部错误。");
						return L"";
					}
				}
			};

			ResourceHandle<BasicILGenericLinearRes> BasicLanguage_GenerateLinearResource(const BCP& argument, BasicCodegenInfo::FunctionLinear& linear)
			{
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
				return linearRecord;
			}

			ResourceString BasicLanguage_GenerateUniqueEntryID(const WString& programName, BasicDeclaration* declaration, const BCP& argument)
			{
				WString id=EscapeResourceName(programName)+L"::"+EscapeResourceName(declaration->name);
				return argument.exportResource->CreateString(id);
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
						entryResource->uniqueEntryID=BasicLanguage_GenerateUniqueEntryID(programName, entry->declaration, argument);
						entries.Add(entryResource);
					}
				}
				return argument.exportResource->CreateArrayRecord(entries.Wrap());
			}

			ResourceArrayHandle<BasicILGenericVariableEntryRes> BasicLanguage_GenerateVariableEntryResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				List<ResourceHandle<BasicILGenericVariableEntryRes>> entries;
				for(vint i=0;i<program->declarations.Count();i++)
				{
					Ptr<BasicVariableDeclaration> declaration=program->declarations[i].Cast<BasicVariableDeclaration>();
					if(declaration && declaration->genericDeclaration.HasGeneric() && !declaration->linking.HasLink())
					{
						ResourceRecord<BasicILGenericVariableEntryRes> entryResource=argument.exportResource->CreateRecord<BasicILGenericVariableEntryRes>();
						entryResource->genericArgumentCount=declaration->genericDeclaration.arguments.Count();
						entryResource->name=argument.exportResource->CreateString(declaration->name);
						entryResource->uniqueEntryID=BasicLanguage_GenerateUniqueEntryID(programName, declaration.Obj(), argument);

						BasicTypeRecord* variableType=argument.info->GetEnv()->GlobalScope()->variables.Items()[declaration->name].type;
						List<BasicTypeRecord*> genericParameters;
						Dictionary<BasicTypeRecord*, BasicTypeRecord*> genericParameterMap;
						for(vint i=0;i<variableType->ParameterCount();i++)
						{
							BasicTypeRecord* parameter=variableType->ParameterType(i);
							genericParameters.Add(parameter);
							genericParameterMap.Add(parameter, parameter);
						}
						variableType=argument.info->GetTypeManager()->Instanciate(variableType, genericParameterMap.Wrap());
						BasicOffset& offset=argument.info->GetTypeInfo(variableType)->size;

						BasicCodegenInfo::FunctionLinear linear;
						linear(offset.Constant());
						for(vint i=0;i<genericParameters.Count();i++)
						{
							vint value=offset.Factor(genericParameters[i]);
							linear(i, value);
						}
						entryResource->size=BasicLanguage_GenerateLinearResource(argument, linear);

						entries.Add(entryResource);
					}
				}
				return argument.exportResource->CreateArrayRecord(entries.Wrap());
			}

			ResourceHandle<BasicILGenericArgumentRes> BasicLanguage_GenerateGenericArgumentRes(BasicTypeRecord* type, BasicDeclaration* ownerDeclaration, TypeUniqueStringRecorder& recorder, const BCP& argument)
			{
				const BasicOffset& offset=argument.info->GetTypeInfo(type)->size;
						
				ResourceRecord<BasicILGenericLinearRes> linearRecord=argument.exportResource->CreateRecord<BasicILGenericLinearRes>();
				{
					linearRecord->constant=offset.Constant();
					vint count=ownerDeclaration->genericDeclaration.arguments.Count();
					ResourceArrayRecord<BasicILGenericFactorItemRes> factors=argument.exportResource->CreateArrayRecord<BasicILGenericFactorItemRes>(count);
					for(vint j=0;j<count;j++)
					{
						ResourceRecord<BasicILGenericFactorItemRes> factor=argument.exportResource->CreateRecord<BasicILGenericFactorItemRes>();
						WString genericArgumentName=ownerDeclaration->genericDeclaration.arguments[j];
						BasicTypeRecord* genericArgumentType=argument.info->GetTypeManager()->GetGenericArgumentType(genericArgumentName);
						factor->factor=offset.Factor(genericArgumentType);
						factors[j]=factor;
					}
					linearRecord->factors=factors;
				}

				ResourceRecord<BasicILGenericArgumentRes> argumentRecord=argument.exportResource->CreateRecord<BasicILGenericArgumentRes>();
				{
					argumentRecord->sizeArgument=linearRecord;
					List<BasicTypeRecord*> argumentTypes;
					if(type->GetType()==BasicTypeRecord::GenericArgument)
					{
						ResourceRecord<BasicILGenericNameRes> nameRes=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
						nameRes->constantString=ResourceString::Null();
						nameRes->isConstant=false;
						nameRes->stringArgumentIndex=ownerDeclaration->genericDeclaration.arguments.IndexOf(type->ArgumentName());
						argumentRecord->nameArgument=nameRes;
					}
					else
					{
						ResourceRecord<BasicILGenericNameRes> nameRes=argument.exportResource->CreateRecord<BasicILGenericNameRes>();
						nameRes->constantString=argument.exportResource->CreateString(recorder.GetTypeName(type, argumentTypes));
						nameRes->isConstant=true;
						nameRes->stringArgumentIndex=-1;
						argumentRecord->nameArgument=nameRes;
					}

					List<ResourceHandle<BasicILGenericArgumentRes>> subArguments;
					for(vint j=0;j<argumentTypes.Count();j++)
					{
						subArguments.Add(BasicLanguage_GenerateGenericArgumentRes(argumentTypes[j], ownerDeclaration, recorder, argument));
					}
					if(subArguments.Count()>0)
					{
						argumentRecord->subArgument=argument.exportResource->CreateArrayRecord(subArguments.Wrap());
					}
					else
					{
						argumentRecord->subArgument=ResourceArrayHandle<BasicILGenericArgumentRes>::Null();
					}
				}

				return argumentRecord;
			}

			ResourceArrayHandle<BasicILGenericTargetRes> BasicLanguage_GenerateTargetResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				List<ResourceHandle<BasicILGenericTargetRes>> targets;
				for(vint i=0;i<argument.info->instanciatedGenericTargets.Count();i++)
				{
					BasicCodegenInfo::GenericTarget* target=argument.info->instanciatedGenericTargets[i].Obj();
					TypeUniqueStringRecorder recorder(argument.exportResource, program, argument.info->GetEnv(), programName, target->ownerFunctionDeclaration);
					ResourceRecord<BasicILGenericTargetRes> targetResource=argument.exportResource->CreateRecord<BasicILGenericTargetRes>();
					WString assemblyName;
					WString symbolName;
					if(target->targetDeclaration->linking.HasLink())
					{
						assemblyName=target->targetDeclaration->linking.assemblyName;
						symbolName=target->targetDeclaration->linking.symbolName;
					}
					else
					{
						assemblyName=programName;
						symbolName=target->targetDeclaration->name;
					}

					List<ResourceHandle<BasicILGenericArgumentRes>> arguments;
					for(vint j=0;j<target->genericParameters.Count();j++)
					{
						BasicTypeRecord* type=target->genericParameters[j];
						arguments.Add(BasicLanguage_GenerateGenericArgumentRes(type, target->ownerFunctionDeclaration, recorder, argument));
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
					linears.Add(BasicLanguage_GenerateLinearResource(argument, linear));
				}
				return argument.exportResource->CreateArrayRecord(linears.Wrap());
			}

			ResourceHandle<BasicILGenericRes> BasicLanguage_GenerateGenericResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				ResourceRecord<BasicILGenericRes> genericRes=argument.exportResource->CreateRecord<BasicILGenericRes>();
				genericRes->functionEntries=BasicLanguage_GenerateFunctionEntryResource(programName, argument);
				genericRes->variableEntries=BasicLanguage_GenerateVariableEntryResource(program, programName, argument);
				genericRes->targets=BasicLanguage_GenerateTargetResource(program, programName, argument);
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