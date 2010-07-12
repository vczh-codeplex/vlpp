#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicLanguage_GenerateLinkingSymbolTable
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateLinkingSymbolTable, BasicDeclaration, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(node->linking.HasLink() && !argument.info->linkings.Contains(node->linking))
					{
						if(!node->genericDeclaration.HasGeneric())
						{
							const_cast<BCP&>(argument).info->linkings.Add(node->linking);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(node->linking.HasLink() && !argument.info->linkings.Contains(node->linking))
					{
						if(!node->genericDeclaration.HasGeneric())
						{
							const_cast<BCP&>(argument).info->linkings.Add(node->linking);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateLinkingSymbolTable)

/***********************************************************************
BasicLanguage_GenerateCodePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					vint index=-1;
					if(node->linking.HasLink())
					{
						index=argument.info->linkings.IndexOf(node->linking);
					}
					else
					{
						index=argument.info->localFunctionCount++;
					}
					argument.info->GetFunctions().Add(node, index);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					TODO_FOR_GENERIC_VARIABLE_BEGIN
						BasicOffset offset=-1;
						if(node->linking.HasLink())
						{
							offset=argument.info->linkings.IndexOf(node->linking);
						}
						else
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->variables.Items()[node->name].type;
							BasicOffset size=argument.info->GetTypeInfo(type)->size;
							char* data=new char[size.Constant()];
							memset(data, 0, size.Constant());
							offset=(vint)argument.globalData->Size();
							argument.globalData->Write(data, size.Constant());
							delete[] data;
						}
						argument.info->GetGlobalVariableOffsets().Add(node, offset.Constant());
					TODO_FOR_GENERIC_VARIABLE_END

					if(node->initializer)
					{
						BasicLanguage_StoreToAddress(node->initializer.Obj(), node, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.codegenExtension->GenerateCodePass1(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1)

/***********************************************************************
BasicLanguage_GenerateCodePass2
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2, BasicDeclaration, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(node->statement)
					{
						vint functionStart=argument.il->instructions.Count();
						argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(0));
						vint reserveVariablesIndex=argument.il->instructions.Count()-1;

						argument.info->BeginFunction(node, argument.il, 1);
						BasicLanguage_GenerateCode(node->statement, argument);
						argument.info->EndFunction(argument.il->instructions.Count(), argument.il, 2);

						{
							BasicIns& ins=argument.il->instructions[reserveVariablesIndex];
							BasicOffset space=argument.info->GetMaxVariableSpace();
							if(space.IsConstant())
							{
								ins.argument.int_value=space.Constant();
							}
							else
							{
								ins.argumentType=BasicIns::linearArgument;
								ins.argument.int_value=argument.info->RegisterLinear(space);
							}
						}
						argument.Ins(BasicIns::stack_reserve, -argument.info->GetMaxVariableSpace());
						BasicScope* functionScope=argument.info->GetEnv()->GetFunctionScope(node);
						BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(functionScope->OwnerDeclaration(), true);
						BasicOffset parameterSize=0;
						for(vint i=0;i<functionType->ParameterCount();i++)
						{
							parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
						}
						argument.Ins(BasicIns::ret, parameterSize);

						BasicILLocalLabel label;
						label.instructionIndex=functionStart;
						argument.il->labels.Add(label);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.codegenExtension->GenerateCodePass2(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2)
		}
	}
}