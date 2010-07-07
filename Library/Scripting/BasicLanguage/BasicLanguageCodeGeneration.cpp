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
				vint index=typeInfos.Keys().IndexOf(type);
				if(index==-1)
				{
					BasicTypeInfo* info=new BasicTypeInfo;
					switch(type->GetType())
					{
					case BasicTypeRecord::Array:
						{
							BasicTypeInfo* element=GetTypeInfo(type->ElementType());
							info->size=element->size*type->ElementCount();
						}
						break;
					case BasicTypeRecord::Function:
					case BasicTypeRecord::Pointer:
						{
							info->size=sizeof(void*);
						}
						break;
					case BasicTypeRecord::Primitive:
						{
							switch(type->PrimitiveType())
							{
							case s8:case u8:case bool_type:case char_type:
								info->size=1;
								break;
							case s16:case u16:case wchar_type:
								info->size=2;
								break;
							case s32:case u32:case f32:
								info->size=4;
								break;
							case s64:case u64:case f64:
								info->size=8;
								break;
							default:
								info->size=1;
							}
						}
						break;
					case BasicTypeRecord::Structure:
						{
							BasicOffset offset=0;
							info->size=0;
							for(vint i=0;i<type->MemberCount();i++)
							{
								BasicTypeInfo* member=GetTypeInfo(type->MemberType(i));
								info->offsets.Add(offset);
								offset+=member->size;
							}
							if(offset==(vint)0)
							{
								info->size=1;
							}
							else
							{
								info->size=offset;
							}
						}
						break;
					case BasicTypeRecord::GenericArgument:
					case BasicTypeRecord::Generic:
						{
							info->size=1;
						}
						break;
					}
					typeInfos.Add(type, info);
					return info;
				}
				else
				{
					return typeInfos.Values()[index].Obj();
				}
			}
			
			BasicEnv* BasicCodegenInfo::GetEnv()
			{
				return analyzer->GetEnv();
			}

			BasicTypeManager* BasicCodegenInfo::GetTypeManager()
			{
				return analyzer->GetTypeManager();
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

			void BasicCodegenInfo::BeginFunction(BasicFunctionDeclaration* declaration)
			{
				usedVariableSpace=0;
				breakInsStack.Clear();
				breakInstructions.Clear();
				continueInsStack.Clear();
				continueInstructions.Clear();
				returnInstructions.Clear();

				if(declaration)
				{
					BasicTypeRecord* type=analyzer->GetEnv()->GetFunctionType(declaration);
					for(vint i=0;i<type->ParameterCount();i++)
					{
						currentFunctionGenericParameters.Add(type->ParameterType(i));
					}
				}
			}

			void BasicCodegenInfo::EndFunction(vint returnIns, basicil::BasicIL* il)
			{
				for(vint i=0;i<returnInstructions.Count();i++)
				{
					il->instructions[returnInstructions[i]].argument.int_value=returnIns;
				}
				returnInstructions.Clear();
				currentFunctionGenericParameters.Clear();
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

			vint BasicCodegenInfo::RegisterInstanciatedGenericFunction(Ptr<FunctionTarget> target)
			{
				for(vint i=0;i<instanciatedGenericFunctions.Count();i++)
				{
					FunctionTarget* currentTarget=instanciatedGenericFunctions[i].Obj();
					if(target->declaration==currentTarget->declaration && CompareEnumerable(target->genericParameters.Wrap(), currentTarget->genericParameters.Wrap())==0)
					{
						return i;
					}
				}
				instanciatedGenericFunctions.Add(target);
				return instanciatedGenericFunctions.Count()-1;
			}

			vint BasicCodegenInfo::RegisterLinear(const BasicOffset& offset)
			{
				FunctionLinear linear;
				linear(offset.Constant());
				for(vint i=0;i<currentFunctionGenericParameters.Count();i++)
				{
					vint value=offset.Factor(currentFunctionGenericParameters[i]);
					if(value!=0)
					{
						linear(i, value);
					}
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
				CHECK_ERROR(false, L"BasicCodegenExtension::PushValue(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::RunSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::RunSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRef(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushRef(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushRefWithoutSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			bool BasicCodegenExtension::CanPushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::CanPushRefWithoutSideEffect(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCode(BasicExtendedStatement* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCode(BasicExtendedStatement*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			ResourceHandle<BasicDeclarationRes> BasicCodegenExtension::GenerateResource(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateResource(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
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
				TODO_FOR_GENERIC_FUNCTION_BEGIN
					il->InsUD(opcode, BasicIns::MakeInt(argument.Constant()), currentLanguageElement);
				TODO_FOR_GENERIC_FUNCTION_END
			}

			void BasicCodegenParameter::Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, const BasicOffset& argument)const
			{
				TODO_FOR_GENERIC_FUNCTION_BEGIN
					il->InsUD(opcode, type1, BasicIns::MakeInt(argument.Constant()), currentLanguageElement);
				TODO_FOR_GENERIC_FUNCTION_END
			}

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			void BasicLanguage_GenerateCode(Ptr<BasicProgram> program, const WString& programName, const BCP& argument)
			{
				const_cast<BCP&>(argument).currentLanguageElement=program.Obj();
				argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(0));
				vint reserveVariablesIndex=argument.il->instructions.Count()-1;
				argument.info->BeginFunction(0);

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateLinkingSymbolTable(program->declarations[i], argument);
				}

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass1(program->declarations[i], argument);
				}

				argument.info->EndFunction(argument.il->instructions.Count(), argument.il);
				TODO_FOR_GENERIC_FUNCTION_BEGIN
					argument.il->instructions[reserveVariablesIndex].argument.int_value=argument.info->GetMaxVariableSpace().Constant();
					argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-argument.info->GetMaxVariableSpace().Constant()));
				TODO_FOR_GENERIC_FUNCTION_END
				argument.Ins(BasicIns::ret, BasicIns::MakeInt(0));

				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass2(program->declarations[i], argument);
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
				List<ResourceHandle<BasicDeclarationRes>> declarations;

				ResourceRecord<BasicILEntryRes> exportEntry=argument.exportResource->CreateRecord<BasicILEntryRes>();
				exportEntry->assemblyName=argument.exportResource->CreateString(programName);
				List<ResourceHandle<BasicILExportRes>> exports;
				List<ResourceHandle<BasicILLinkingRes>> linkings;
				exportEntry->genericSymbols=ResourceHandle<BasicILGenericRes>::Null();

				for(vint i=0;i<program->declarations.Count();i++)
				{
					ResourceHandle<BasicDeclarationRes> declaration=BasicLanguage_GenerateResource(program->declarations[i], argument);
					if(declaration)
					{
						declarations.Add(declaration);
					}

					ResourceHandle<BasicILExportRes> exportRes=BasicLanguage_GenerateExport(program->declarations[i], argument);
					if(exportRes)
					{
						exports.Add(exportRes);
					}
				}

				ResourceRecord<BasicILLinkingRes> currentLinking;
				for(vint i=0;i<argument.info->linkings.Count();i++)
				{
					BasicLinking& linking=argument.info->linkings[i];
					ResourceRecord<BasicILLinkingRes> linkingRecord=argument.exportResource->CreateRecord<BasicILLinkingRes>();
					linkingRecord->assemblyName=argument.exportResource->CreateString(linking.assemblyName);
					linkingRecord->symbolName=argument.exportResource->CreateString(linking.symbolName);
					linkings.Add(linkingRecord);
				}

				entry->declarations=argument.resource->CreateArrayRecord(declarations.Wrap());
				exportEntry->exports=argument.exportResource->CreateArrayRecord(exports.Wrap());
				exportEntry->linkings=argument.exportResource->CreateArrayRecord(linkings.Wrap());
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
				BasicLanguage_GenerateCode(program, programName, argument);
			}
		}
	}
}