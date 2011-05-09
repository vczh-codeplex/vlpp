#include "BasicLanguageCodeGeneration.h"
#include "BasicLanguageResource.h"
#include "..\..\Collections\OperationEnumerable.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;
			using namespace collections;

/***********************************************************************
BasicCodegenInfo
***********************************************************************/

			BasicCodegenInfo::BasicCodegenInfo(BasicAnalyzer* _analyzer)
				:analyzer(_analyzer)
				,usedVariableSpace(0)
				,localFunctionCount(0)
			{
			}

			BasicTypeInfo* BasicCodegenInfo::GetTypeInfo(BasicTypeRecord* type)
			{
				return analyzer->GetTypeInfoManager()->GetTypeInfo(type);
			}
			
			BasicEnv* BasicCodegenInfo::GetEnv()
			{
				return analyzer->GetEnv();
			}

			BasicTypeManager* BasicCodegenInfo::GetTypeManager()
			{
				return analyzer->GetTypeManager();
			}

			BasicTypeInfoManager* BasicCodegenInfo::GetTypeInfoManager()
			{
				return analyzer->GetTypeInfoManager();
			}

			BasicAlgorithmConfiguration& BasicCodegenInfo::GetConfiguration()
			{
				return analyzer->GetConfiguration();
			}

			collections::IDictionary<BasicFunctionDeclaration*, vint>& BasicCodegenInfo::GetFunctions()
			{
				return functions.Wrap();
			}

			collections::IDictionary<BasicVariableDeclaration*, vint>& BasicCodegenInfo::GetGlobalVariableOffsets()
			{
				return globalVariableOffsets.Wrap();
			}

			collections::IDictionary<BasicVariableStatement*, BasicOffset>& BasicCodegenInfo::GetLocalVariableOffsets()
			{
				return localVariableOffsets.Wrap();
			}

			void BasicCodegenInfo::BeginFunction(BasicFunctionDeclaration* declaration, basicil::BasicIL* il, vint existInstructionCount)
			{
				usedVariableSpace=0;
				breakInsStack.Clear();
				breakInstructions.Clear();
				continueInsStack.Clear();
				continueInstructions.Clear();
				returnInstructions.Clear();

				currentFunctionDeclaration=declaration;
				currentFunctionGenericParameters.Clear();
				if(declaration)
				{
					BasicTypeRecord* type=analyzer->GetEnv()->GetFunctionType(declaration, false);
					for(vint i=0;i<type->ParameterCount();i++)
					{
						currentFunctionGenericParameters.Add(type->ParameterType(i));
					}
				}

				Ptr<FunctionEntry> entry=new FunctionEntry;
				entry->declaration=declaration;
				entry->startInstruction=il->instructions.Count()-existInstructionCount;
				entry->instructionCount=0;
				instanciatedGenericFunctionEntries.Add(entry);
			}

			void BasicCodegenInfo::EndFunction(vint returnIns, basicil::BasicIL* il, vint remainInstructionCount)
			{
				for(vint i=0;i<returnInstructions.Count();i++)
				{
					il->instructions[returnInstructions[i]].argument.int_value=returnIns;
				}
				returnInstructions.Clear();

				Ptr<FunctionEntry> entry=instanciatedGenericFunctionEntries[instanciatedGenericFunctionEntries.Count()-1];
				entry->instructionCount=il->instructions.Count()-entry->startInstruction+remainInstructionCount;
			}

			void BasicCodegenInfo::EnterSemanticScope(BasicScope* scope)
			{
				semanticScopes.Add(scope);
			}

			void BasicCodegenInfo::LeaveSemanticScope()
			{
				semanticScopes.RemoveAt(semanticScopes.Count()-1);
			}

			BasicScope* BasicCodegenInfo::GetSemanticScope()
			{
				return semanticScopes[semanticScopes.Count()-1];
			}

			void BasicCodegenInfo::AssociateReturn(vint instruction)
			{
				returnInstructions.Add(instruction);
			}

			void BasicCodegenInfo::EnterScope()
			{
			}

			void BasicCodegenInfo::LeaveScope()
			{
			}

			BasicOffset BasicCodegenInfo::UseVariable(BasicOffset size)
			{
				usedVariableSpace+=size;
				return -usedVariableSpace;
			}

			void BasicCodegenInfo::EnterLoop()
			{
				breakInsStack.Add(breakInstructions.Count());
				continueInsStack.Add(continueInstructions.Count());
			}

			void BasicCodegenInfo::LeaveLoop(vint breakIns, vint continueIns, basicil::BasicIL* il)
			{
				vint breakCount=breakInsStack[breakInsStack.Count()-1];
				breakInsStack.RemoveAt(breakInsStack.Count()-1);
				vint continueCount=continueInsStack[continueInsStack.Count()-1];
				continueInsStack.RemoveAt(continueInsStack.Count()-1);

				for(vint i=breakCount;i<breakInstructions.Count();i++)
				{
					il->instructions[breakInstructions[i]].argument.int_value=breakIns;
				}
				breakInstructions.RemoveRange(breakCount, breakInstructions.Count()-breakCount);

				for(vint i=continueCount;i<continueInstructions.Count();i++)
				{
					il->instructions[continueInstructions[i]].argument.int_value=continueIns;
				}
				continueInstructions.RemoveRange(continueCount, continueInstructions.Count()-continueCount);
			}

			void BasicCodegenInfo::AssociateBreak(vint instruction)
			{
				breakInstructions.Add(instruction);
			}

			void BasicCodegenInfo::AssociateContinue(vint instruction)
			{
				continueInstructions.Add(instruction);
			}

			BasicOffset BasicCodegenInfo::GetMaxVariableSpace()
			{
				return usedVariableSpace;
			}

			vint BasicCodegenInfo::RegisterGenericTarget(Ptr<GenericTarget> target)
			{
				for(vint i=0;i<instanciatedGenericTargets.Count();i++)
				{
					GenericTarget* currentTarget=instanciatedGenericTargets[i].Obj();
					if(currentFunctionDeclaration==currentTarget->ownerFunctionDeclaration && target->targetDeclaration==currentTarget->targetDeclaration && CompareEnumerable(target->genericParameters.Wrap(), currentTarget->genericParameters.Wrap())==0)
					{
						return i;
					}
				}
				target->ownerFunctionDeclaration=currentFunctionDeclaration;
				instanciatedGenericTargets.Add(target);
				return instanciatedGenericTargets.Count()-1;
			}

			vint BasicCodegenInfo::RegisterGenericInstanceTarget(Ptr<GenericInstanceTarget> target)
			{
				for(vint i=0;i<instanciatedGenericInstanceTargets.Count();i++)
				{
					GenericInstanceTarget* currentTarget=instanciatedGenericInstanceTargets[i].Obj();
					if(currentFunctionDeclaration==currentTarget->ownerFunctionDeclaration && target->targetDeclaration==currentTarget->targetDeclaration && target->functionName==currentTarget->functionName && target->type==currentTarget->type)
					{
						return i;
					}
				}
				target->ownerFunctionDeclaration=currentFunctionDeclaration;
				instanciatedGenericInstanceTargets.Add(target);
				return instanciatedGenericInstanceTargets.Count()-1;
			}

			vint BasicCodegenInfo::RegisterLinear(const BasicOffset& offset)
			{
				FunctionLinear linear;
				linear(offset.Constant());
				for(vint i=0;i<currentFunctionGenericParameters.Count();i++)
				{
					vint value=offset.Factor(currentFunctionGenericParameters[i]);
					linear(i, value);
				}
				vint index=instanciatedGenericLinears.IndexOf(linear);
				if(index==-1)
				{
					instanciatedGenericLinears.Add(linear);
					return instanciatedGenericLinears.Count()-1;
				}
				else
				{
					return index;
				}
			}

			ResourceHandle<BasicTypeRes> BasicCodegenInfo::GetTypeResource(BasicTypeRecord* type)
			{
				vint index=typeResources.Keys().IndexOf(type);
				return index==-1?ResourceHandle<BasicTypeRes>::Null():typeResources.Values()[index];
			}

			bool BasicCodegenInfo::SetTypeResource(BasicTypeRecord* type, ResourceHandle<BasicTypeRes> resource)
			{
				vint index=typeResources.Keys().IndexOf(type);
				if(index==-1)
				{
					typeResources.Add(type, resource);
					return true;
				}
				else
				{
					return false;
				}
			}

/***********************************************************************
BasicCodegenExtension
***********************************************************************/

			BasicTypeRecord* BasicCodegenExtension::PushValue(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::PushValue(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::RunSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::RunSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRef(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::PushRef(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::PushRefWithoutSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			bool BasicCodegenExtension::CanPushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::CanPushRefWithoutSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCode(BasicExtendedStatement* statement, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::GenerateCode(BasicExtendedStatement*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass3(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::GenerateCodePass3(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			ResourceHandle<BasicDeclarationRes> BasicCodegenExtension::GenerateResource(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_FAIL(L"BasicCodegenExtension::GenerateResource(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

/***********************************************************************
BasicCodegenParameter
***********************************************************************/
			
			BasicCodegenParameter::BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData, Ptr<ResourceStream> _resource, Ptr<ResourceStream> _exportResource)
				:info(_info)
				,il(_il)
				,globalData(_globalData)
				,codegenExtension(&defaultCodegenExtension)
				,resource(_resource)
				,exportResource(_exportResource)
				,currentLanguageElement(0)
			{
			}

			BasicCodegenParameter::BasicCodegenParameter(const BasicCodegenParameter& parameter)
				:info(parameter.info)
				,il(parameter.il)
				,globalData(parameter.globalData)
				,codegenExtension(parameter.codegenExtension)
				,resource(parameter.resource)
				,exportResource(parameter.exportResource)
				,currentLanguageElement(0)
			{
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode)const
			{
				il->InsUD(opcode, currentLanguageElement);
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::Argument argument)const
			{
				il->InsUD(opcode, argument, currentLanguageElement);
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1)const
			{
				il->InsUD(opcode, type1, currentLanguageElement);
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::Argument argument)const
			{
				il->InsUD(opcode, type1, argument, currentLanguageElement);
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::ValueType type2)const
			{
				il->InsUD(opcode, type1, type2, currentLanguageElement);
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, const BasicOffset& argument)const
			{
				if(argument.IsConstant())
				{
					il->InsUD(opcode, BasicIns::MakeInt(argument.Constant()), currentLanguageElement);
				}
				else
				{
					il->InsUDG(opcode, BasicIns::MakeInt(info->RegisterLinear(argument)), currentLanguageElement);
				}
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, const BasicOffset& argument)const
			{
				if(argument.IsConstant())
				{
					il->InsUD(opcode, type1, BasicIns::MakeInt(argument.Constant()), currentLanguageElement);
				}
				else
				{
					il->InsUDG(opcode, type1, BasicIns::MakeInt(info->RegisterLinear(argument)), currentLanguageElement);
				}
			}

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			void BasicLanguage_GenerateCode(Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				const_cast<BCP&>(argument).currentLanguageElement=program.Obj();
				argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(0));
				vint reserveVariablesIndex=argument.il->instructions.Count()-1;
				argument.info->BeginFunction(0, argument.il, 1);

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateLinkingSymbolTable(program->declarations[i], argument);
				}

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass1(program->declarations[i], argument);
				}
				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass2(program->declarations[i], argument);
				}

				argument.info->EndFunction(argument.il->instructions.Count(), argument.il, 2);
				{
					BasicOffset space=argument.info->GetMaxVariableSpace();
					if(space.IsConstant())
					{
						argument.il->instructions[reserveVariablesIndex].argument.int_value=argument.info->GetMaxVariableSpace().Constant();
						argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-argument.info->GetMaxVariableSpace().Constant()));
					}
					else
					{
						BasicIns& ins=argument.il->instructions[reserveVariablesIndex];
						ins.argumentType=BasicIns::linearArgument;
						ins.argument.int_value=argument.info->RegisterLinear(space);
						argument.Ins(BasicIns::stack_reserve, space);
					}
				}
				argument.Ins(BasicIns::ret, BasicIns::MakeInt(0));

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass3(program->declarations[i], argument);
				}

				for(vint i=0;i<argument.il->instructions.Count();i++)
				{
					BasicIns& ins=argument.il->instructions[i];
					switch(ins.opcode)
					{
					case BasicIns::codegen_callfunc:
						ins.opcode=BasicIns::call;
						ins.insKey=-1;
						ins.argument.int_value=argument.il->labels[ins.argument.int_value].instructionIndex;
						break;
					}
				}

				argument.il->globalData.Resize((vint)argument.globalData->Size());
				argument.globalData->SeekFromBegin(0);
				if(argument.globalData->Size()>0)
				{
					argument.globalData->Read(&(argument.il->globalData[0]), (vint)argument.globalData->Size());
				}

				ResourceRecord<BasicEntryRes> entry=argument.resource->CreateRecord<BasicEntryRes>();
				entry->declarations=BasicLanguage_GenerateDeclarationResource(program, argument);

				ResourceRecord<BasicILEntryRes> exportEntry=argument.exportResource->CreateRecord<BasicILEntryRes>();
				exportEntry->assemblyName=argument.exportResource->CreateString(programName);
				exportEntry->exports=BasicLanguage_GenerateExportResource(program, argument);
				exportEntry->linkings=BasicLanguage_GenerateLinkingResource(program, argument);
				exportEntry->foreigns=BasicLanguage_GenerateForeignResource(program, argument);
				exportEntry->genericSymbols=BasicLanguage_GenerateGenericResource(program, programName, argument);
			}

/***********************************************************************
BasicCodeGenerator
***********************************************************************/

			BasicCodeGenerator::BasicCodeGenerator(BasicAnalyzer* analyzer, BasicCodegenExtension* extension, const WString& _programName)
				:il(new BasicIL)
				,globalData(new MemoryStream)
				,codegenExtension(extension)
				,codegenInfo(new BasicCodegenInfo(analyzer))
				,program(analyzer->GetProgram())
				,programName(_programName)
			{
				Ptr<ResourceStream> resource=new ResourceStream;
				il->resources.Add(BasicILResourceNames::BasicLanguageInterfaces, resource);

				Ptr<ResourceStream> exportResource=new ResourceStream;
				il->resources.Add(BasicILResourceNames::ExportedSymbols, exportResource);
			}

			BasicCodeGenerator::~BasicCodeGenerator()
			{
			}

			Ptr<basicil::BasicIL> BasicCodeGenerator::GetIL()
			{
				return il;
			}

			void BasicCodeGenerator::GenerateCode()
			{
				BCP argument(
					codegenInfo.Obj(),
					il.Obj(),
					globalData.Obj(),
					il->resources[BasicILResourceNames::BasicLanguageInterfaces],
					il->resources[BasicILResourceNames::ExportedSymbols]
				);
				if(codegenExtension)
				{
					argument.codegenExtension=codegenExtension;
				}
				argument.info->EnterSemanticScope(argument.info->GetEnv()->GlobalScope());
				BasicLanguage_GenerateCode(program, programName, argument);
				argument.info->LeaveSemanticScope();
			}

/***********************************************************************
Header File Generator
***********************************************************************/

			bool BasicLanguage_GenerateHeaderDeclarationName(
				ResourceRecord<BasicDeclarationRes> declarationRes,
				WString assemblyName,
				Ptr<ResourceStream> resource,
				bool publicOnly,
				bool currentAssemblyOnly,
				const WString& prefix,
				WString& linkingAssemblyName,
				WString& linkingSymbolName,
				WString& declarationName,
				ICollection<WString>& referencedAssemblies)
			{
				if(publicOnly)
				{
					bool containsPublicAttribute=false;
					ResourceArrayRecord<BasicAttributeRes> attributesRes=resource->ReadArrayRecord(declarationRes->attributes);
					for(vint i=0;i<attributesRes.Count();i++)
					{
						WString attribute=resource->ReadString(attributesRes.Get(i)->attributeName);
						if(attribute==L"public")
						{
							containsPublicAttribute=true;
							break;
						}
					}
					if(!containsPublicAttribute)
					{
						return false;
					}
				}
				linkingAssemblyName=resource->ReadString(declarationRes->linkingAssemblyName);
				linkingSymbolName=resource->ReadString(declarationRes->linkingSymbolName);
				declarationName=resource->ReadString(declarationRes->name);
				if(linkingAssemblyName==L"" && linkingSymbolName==L"")
				{
					linkingAssemblyName=assemblyName;
					linkingSymbolName=declarationName;
					declarationName=prefix+declarationName;
				}
				else if(declarationRes->type!=BasicDeclarationRes::ForeignFunction)
				{
					if(currentAssemblyOnly)
					{
						return false;
					}
					if(!referencedAssemblies.Contains(linkingAssemblyName))
					{
						referencedAssemblies.Add(linkingAssemblyName);
					}
					declarationName=prefix+linkingSymbolName;
				}
				return true;
			}

			WString BasicLanguage_GenerateHeaderReferenceName(
				ResourceHandle<BasicTypeRes> typeResHandle,
				Ptr<ResourceStream> resource,
				const WString& prefix,
				Dictionary<vint, ResourceRecord<BasicDeclarationRes>>& declarationTypeMap,
				ICollection<WString>& referencedAssemblies)
			{
				vint index=declarationTypeMap.Keys().IndexOf(typeResHandle.Pointer());
				if(index==-1)
				{
					return L"";
				}
				else
				{
					ResourceRecord<BasicDeclarationRes> declarationRes=declarationTypeMap.Values()[index];
					WString linkingAssemblyName=resource->ReadString(declarationRes->linkingAssemblyName);
					WString linkingSymbolName=resource->ReadString(declarationRes->linkingSymbolName);
					WString declarationName=resource->ReadString(declarationRes->name);
					if(linkingAssemblyName==L"" && linkingSymbolName==L"")
					{
						return prefix+declarationName;
					}
					else
					{
						if(!referencedAssemblies.Contains(linkingAssemblyName))
						{
							referencedAssemblies.Add(linkingAssemblyName);
						}
						return prefix+linkingSymbolName;
					}
				}
			}

			BasicPrimitiveTypeEnum BasicLanguage_GenerateHeaderPrimitiveType(BasicTypeRes::PrimitiveTypeEnum inputType)
			{
				switch(inputType)
				{
				case BasicTypeRes::s8:
					return s8;
				case BasicTypeRes::s16:
					return s16;
				case BasicTypeRes::s32:
					return s32;
				case BasicTypeRes::s64:
					return s64;
				case BasicTypeRes::u8:
					return u8;
				case BasicTypeRes::u16:
					return u16;
				case BasicTypeRes::u32:
					return u32;
				case BasicTypeRes::u64:
					return u64;
				case BasicTypeRes::f32:
					return f32;
				case BasicTypeRes::f64:
					return f64;
				case BasicTypeRes::bool_type:
					return bool_type;
				case BasicTypeRes::void_type:
					return void_type;
				case BasicTypeRes::char_type:
					return char_type;
				case BasicTypeRes::wchar_type:
					return wchar_type;
				default:
					CHECK_FAIL(L"BasicLanguage_GenerateHeaderPrimitiveType(...)#遇到无法解释的资源类型。");
				}
			}

			Ptr<BasicType> BasicLanguage_GenerateHeaderType(
				ResourceHandle<BasicTypeRes> typeResHandle,
				Ptr<ResourceStream> resource,
				const WString& prefix,
				Dictionary<vint, ResourceRecord<BasicDeclarationRes>>& declarationTypeMap,
				ICollection<WString>& referencedAssemblies)
			{
				ResourceRecord<BasicTypeRes> typeRes=resource->ReadRecord(typeResHandle);
				Ptr<BasicType> type;
				switch(typeRes->type)
				{
				case BasicTypeRes::Primitive:
					{
						Ptr<BasicPrimitiveType> target=new BasicPrimitiveType;
						target->type=BasicLanguage_GenerateHeaderPrimitiveType(typeRes->primitiveType);
						type=target;
					}
					break;
				case BasicTypeRes::Pointer:
					{
						Ptr<BasicPointerType> target=new BasicPointerType;
						target->elementType=BasicLanguage_GenerateHeaderType(typeRes->elementType, resource, prefix, declarationTypeMap, referencedAssemblies);
						type=target;
					}
					break;
				case BasicTypeRes::Array:
					{
						Ptr<BasicNumericExpression> count=new BasicNumericExpression;
						count->implicitIntegerType=false;
						count->type=int_type;
						count->argument.int_value=typeRes->elementCount;

						Ptr<BasicArrayType> target=new BasicArrayType;
						target->elementType=BasicLanguage_GenerateHeaderType(typeRes->elementType, resource, prefix, declarationTypeMap, referencedAssemblies);
						target->count=count;
						type=target;
					}
					break;
				case BasicTypeRes::Function:
					{
						Ptr<BasicFunctionType> target=new BasicFunctionType;
						target->returnType=BasicLanguage_GenerateHeaderType(typeRes->elementType, resource, prefix, declarationTypeMap, referencedAssemblies);
						ResourceArrayRecord<BasicSubTypeRes> parameterTypesRes=resource->ReadArrayRecord(typeRes->subTypes);
						for(vint j=0;j<parameterTypesRes.Count();j++)
						{
							ResourceRecord<BasicSubTypeRes> member=parameterTypesRes.Get(j);
							target->parameterTypes.Add(BasicLanguage_GenerateHeaderType(member->type, resource, prefix, declarationTypeMap, referencedAssemblies));
						}
						type=target;
					}
					break;
				case BasicTypeRes::Concept:
				case BasicTypeRes::Structure:
					{
						Ptr<BasicReferenceType> target=new BasicReferenceType;
						target->name=BasicLanguage_GenerateHeaderReferenceName(typeResHandle, resource, prefix, declarationTypeMap, referencedAssemblies);
						type=target;
					}
					break;
				case BasicTypeRes::GenericArgument:
					{
						Ptr<BasicReferenceType> target=new BasicReferenceType;
						target->name=resource->ReadString(typeRes->genericArgumentName);
						type=target;
					}
					break;
				case BasicTypeRes::Instanciated:
					{
						Ptr<BasicInstanciatedGenericType> target=new BasicInstanciatedGenericType;
						target->elementType=BasicLanguage_GenerateHeaderType(typeRes->elementType, resource, prefix, declarationTypeMap, referencedAssemblies);
						ResourceArrayRecord<BasicSubTypeRes> argumentTypesRes=resource->ReadArrayRecord(typeRes->subTypes);
						for(vint j=0;j<argumentTypesRes.Count();j++)
						{
							ResourceRecord<BasicSubTypeRes> member=argumentTypesRes.Get(j);
							target->argumentTypes.Add(BasicLanguage_GenerateHeaderType(member->type, resource, prefix, declarationTypeMap, referencedAssemblies));
						}
						type=target;
					}
					break;
				default:
					CHECK_FAIL(L"BasicLanguage_GenerateHeaderType(...)#遇到无法解释的资源类型。");
				}
				return type;
			}

			WString BasicLanguage_GenerateHeaderReferencedConcept(
				WString conceptName,
				Ptr<ResourceStream> resource,
				const WString& prefix,
				Dictionary<WString, ResourceRecord<BasicDeclarationRes>>& conceptMap,
				ICollection<WString>& referencedAssemblies)
			{
				ResourceRecord<BasicDeclarationRes> declarationRes=conceptMap[conceptName];
				WString linkingAssemblyName=resource->ReadString(declarationRes->linkingAssemblyName);
				if(linkingAssemblyName!=L"" && !referencedAssemblies.Contains(linkingAssemblyName))
				{
					referencedAssemblies.Add(linkingAssemblyName);
				}
				return prefix+conceptName;
			}

			Ptr<BasicProgram> BasicLanguage_GenerateHeaderFile(
				WString assemblyName,
				Ptr<ResourceStream> resource,
				bool publicOnly,
				bool currentAssemblyOnly,
				const WString& prefix,
				ICollection<WString>& referencedAssemblies)
			{
				Ptr<BasicProgram> program=new BasicProgram;
				ResourceRecord<BasicEntryRes> entryRes=resource->ReadRootRecord<BasicEntryRes>();
				ResourceArrayRecord<BasicDeclarationRes> declarationsRes=resource->ReadArrayRecord<BasicDeclarationRes>(entryRes->declarations);
				Dictionary<vint, ResourceRecord<BasicDeclarationRes>> declarationTypeMap;
				Dictionary<WString, ResourceRecord<BasicDeclarationRes>> conceptMap;
				for(vint i=0;i<declarationsRes.Count();i++)
				{
					ResourceRecord<BasicDeclarationRes> declarationRes=declarationsRes.Get(i);
					if(declarationRes->type==BasicDeclarationRes::Structure)
					{
						declarationTypeMap.Add(declarationRes->declarationType.Pointer(), declarationRes);
					}
					else if(declarationRes->type==BasicDeclarationRes::Concept)
					{
						declarationTypeMap.Add(declarationRes->declarationType.Pointer(), declarationRes);
						conceptMap.Add(resource->ReadString(declarationRes->name), declarationRes);
					}
				}

				for(vint i=0;i<declarationsRes.Count();i++)
				{
					ResourceRecord<BasicDeclarationRes> declarationRes=declarationsRes.Get(i);
					WString linkingAssemblyName, linkingSymbolName, declarationName;
					if(BasicLanguage_GenerateHeaderDeclarationName(
						declarationRes,
						assemblyName,
						resource,
						publicOnly,
						currentAssemblyOnly,
						prefix,
						linkingAssemblyName,
						linkingSymbolName,
						declarationName,
						referencedAssemblies))
					{
						Ptr<BasicDeclaration> declaration;
						ResourceRecord<BasicTypeRes> typeRes=resource->ReadRecord(declarationRes->declarationType);
						switch(declarationRes->type)
						{
						case BasicDeclarationRes::Structure:
							{
								if(!declarationRes->genericArgumentNames || resource->ReadArrayRecord(declarationRes->genericArgumentNames).Count()==0)
								{
									Ptr<BasicStructureDeclaration> target=new BasicStructureDeclaration;
									target->defined=false;
									target->name=declarationName;
									program->declarations.Add(target);
								}
							}
							break;
						}
					}
				}

				for(vint i=0;i<declarationsRes.Count();i++)
				{
					ResourceRecord<BasicDeclarationRes> declarationRes=declarationsRes.Get(i);
					WString linkingAssemblyName, linkingSymbolName, declarationName;
					if(BasicLanguage_GenerateHeaderDeclarationName(
						declarationRes,
						assemblyName,
						resource,
						publicOnly,
						currentAssemblyOnly,
						prefix,
						linkingAssemblyName,
						linkingSymbolName,
						declarationName,
						referencedAssemblies))
					{
						Ptr<BasicDeclaration> declaration;
						ResourceRecord<BasicTypeRes> typeRes=resource->ReadRecord(declarationRes->declarationType);
						switch(declarationRes->type)
						{
						case BasicDeclarationRes::ForeignFunction:
						case BasicDeclarationRes::Function:
							{
								Ptr<BasicFunctionDeclaration> target=new BasicFunctionDeclaration;
								target->foreignFunction=declarationRes->type==BasicDeclarationRes::ForeignFunction;
								target->signatureType=BasicLanguage_GenerateHeaderType(typeRes, resource, prefix, declarationTypeMap, referencedAssemblies).Cast<BasicFunctionType>();
								ResourceArrayRecord<BasicParameterRes> parameterNamesRes=resource->ReadArrayRecord(declarationRes->parameterNames);
								for(vint j=0;j<parameterNamesRes.Count();j++)
								{
									target->parameterNames.Add(resource->ReadString(parameterNamesRes.Get(j)->name));
								}
								declaration=target;
							}
							break;
						case BasicDeclarationRes::Variable:
							{
								Ptr<BasicVariableDeclaration> target=new BasicVariableDeclaration;
								target->constant=false;
								target->type=BasicLanguage_GenerateHeaderType(typeRes, resource, prefix, declarationTypeMap, referencedAssemblies);
								declaration=target;
							}
							break;
						case BasicDeclarationRes::Constant:
							{
								Ptr<BasicVariableDeclaration> target=new BasicVariableDeclaration;
								target->constant=true;
								target->type=BasicLanguage_GenerateHeaderType(typeRes, resource, prefix, declarationTypeMap, referencedAssemblies);

								if(target->type.Cast<BasicPointerType>())
								{
									if(declarationRes->constantValue.d==0)
									{
										target->initializer=new BasicNullExpression;
									}
									else
									{
										Ptr<BasicNumericExpression> number=new BasicNumericExpression;
										number->implicitIntegerType=true;
										number->type=u64;
										number->argument.u64=declarationRes->constantValue.u;

										Ptr<BasicCastingExpression> casting=new BasicCastingExpression;
										casting->type=target->type;
										casting->operand=number;
										target->initializer=casting;
									}
								}
								else if(Ptr<BasicPrimitiveType> primitiveType=target->type.Cast<BasicPrimitiveType>())
								{
									Ptr<BasicNumericExpression> number=new BasicNumericExpression;
									number->implicitIntegerType=true;
									number->type=primitiveType->type;
									switch(primitiveType->type)
									{
									case s8:
										number->argument.s8=(signed __int8)declarationRes->constantValue.s;
										break;
									case s16:
										number->argument.s16=(signed __int16)declarationRes->constantValue.s;
										break;
									case s32:
										number->argument.s32=(signed __int32)declarationRes->constantValue.s;
										break;
									case s64:
										number->argument.s64=(signed __int64)declarationRes->constantValue.s;
										break;
									case u8:
										number->argument.u8=(unsigned __int8)declarationRes->constantValue.u;
										break;
									case u16:
										number->argument.u16=(unsigned __int16)declarationRes->constantValue.u;
										break;
									case u32:
										number->argument.u32=(unsigned __int32)declarationRes->constantValue.u;
										break;
									case u64:
										number->argument.u64=(unsigned __int64)declarationRes->constantValue.u;
										break;
									case f32:
										number->argument.f32=(float)declarationRes->constantValue.d;
										break;
									case f64:
										number->argument.f64=(double)declarationRes->constantValue.d;
										break;
									case char_type:
										number->argument.char_value=(char)declarationRes->constantValue.s;
										break;
									case wchar_type:
										number->argument.wchar_value=(wchar_t)declarationRes->constantValue.u;
										break;
									case bool_type:
										number->argument.bool_value=(declarationRes->constantValue.s!=0);
										break;
									default:
										CHECK_FAIL(L"BasicLanguage_GenerateHeaderFile(...)#遇到无法解释的常量类型。");
									}
									target->initializer=number;
								}
								else
								{
									CHECK_FAIL(L"BasicLanguage_GenerateHeaderFile(...)#遇到无法解释的常量类型。");
								}

								declaration=target;
							}
							break;
						case BasicDeclarationRes::Structure:
							{
								Ptr<BasicStructureDeclaration> target=new BasicStructureDeclaration;
								target->defined=true;
								ResourceArrayRecord<BasicSubTypeRes> parameterTypes=resource->ReadArrayRecord(typeRes->subTypes);
								for(vint j=0;j<parameterTypes.Count();j++)
								{
									ResourceRecord<BasicSubTypeRes> member=parameterTypes.Get(j);
									target->memberTypes.Add(BasicLanguage_GenerateHeaderType(member->type, resource, prefix, declarationTypeMap, referencedAssemblies));
									target->memberNames.Add(resource->ReadString(member->name));
								}
								declaration=target;
							}
							break;
						case BasicDeclarationRes::Concept:
							{
								Ptr<BasicConceptBaseDeclaration> target=new BasicConceptBaseDeclaration;
								ResourceArrayRecord<BasicParameterRes> argumentsRes=resource->ReadArrayRecord(declarationRes->genericArgumentNames);
								target->conceptType=resource->ReadString(argumentsRes.Get(0)->name);
								ResourceArrayRecord<BasicSubTypeRes> parameterTypes=resource->ReadArrayRecord(typeRes->subTypes);
								for(vint j=0;j<parameterTypes.Count();j++)
								{
									ResourceRecord<BasicSubTypeRes> member=parameterTypes.Get(j);
									Ptr<BasicConceptBaseDeclaration::FunctionConcept> function=new BasicConceptBaseDeclaration::FunctionConcept;
									function->name=resource->ReadString(member->name);
									function->signatureType=BasicLanguage_GenerateHeaderType(member->type, resource, prefix, declarationTypeMap, referencedAssemblies).Cast<BasicFunctionType>();
									target->functions.Add(function);
								}
								declaration=target;
							}
							break;
						case BasicDeclarationRes::Instance:
							{
								Ptr<BasicConceptInstanceDeclaration> target=new BasicConceptInstanceDeclaration;
								target->defined=false;
								target->instanceType=BasicLanguage_GenerateHeaderType(typeRes, resource, prefix, declarationTypeMap, referencedAssemblies);
								BasicLanguage_GenerateHeaderReferencedConcept(
									prefix+resource->ReadString(declarationRes->name),
									resource,
									prefix,
									conceptMap,
									referencedAssemblies);
								declaration=target;
							}
							break;
						case BasicDeclarationRes::TypeRename:
							{
								Ptr<BasicTypeRenameDeclaration> target=new BasicTypeRenameDeclaration;
								target->type=BasicLanguage_GenerateHeaderType(typeRes, resource, prefix, declarationTypeMap, referencedAssemblies);
								declaration=target;
							}
							break;
						default:
							CHECK_FAIL(L"BasicLanguage_GenerateHeaderFile(...)#遇到无法解释的资源类型。");
						}
						declaration->name=declarationName;
						if(declarationRes->type!=BasicDeclarationRes::Constant)
						{
							declaration->linking.assemblyName=linkingAssemblyName;
							declaration->linking.symbolName=linkingSymbolName;
						}
						if(declarationRes->type!=BasicDeclarationRes::Concept && declarationRes->genericArgumentNames)
						{
							ResourceArrayRecord<BasicParameterRes> argumentsRes=resource->ReadArrayRecord(declarationRes->genericArgumentNames);
							for(vint j=0;j<argumentsRes.Count();j++)
							{
								declaration->genericDeclaration.arguments.Add(resource->ReadString(argumentsRes.Get(j)->name));
							}
							if(declarationRes->genericArgumentConstraints)
							{
								ResourceArrayRecord<BasicConstraintRes> constraintsRes=resource->ReadArrayRecord(declarationRes->genericArgumentConstraints);
								for(vint j=0;j<constraintsRes.Count();j++)
								{
									ResourceRecord<BasicConstraintRes> constraintRes=constraintsRes.Get(j);
									Ptr<BasicGeneric::Constraint> constraint=new BasicGeneric::Constraint;
									constraint->argumentName=resource->ReadString(constraintRes->argumentName);
									constraint->conceptName=BasicLanguage_GenerateHeaderReferencedConcept(
										prefix+resource->ReadString(constraintRes->conceptName),
										resource,
										prefix,
										conceptMap,
										referencedAssemblies);
									declaration->genericDeclaration.constraints.Add(constraint);
								}
							}
						}
						program->declarations.Add(declaration);
					}
				}
				return program;
			}
		}
	}
}