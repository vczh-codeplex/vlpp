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
					if(node->linking.HasLink() && !argument.linkings.Contains(node->linking))
					{
						const_cast<BCP&>(argument).linkings.Add(node->linking);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(node->linking.HasLink() && !argument.linkings.Contains(node->linking))
					{
						const_cast<BCP&>(argument).linkings.Add(node->linking);
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
					int index=-1;
					if(node->linking.HasLink())
					{
						index=argument.linkings.IndexOf(node->linking);
					}
					else
					{
						index=argument.info->localFunctionCount++;
					}
					argument.info->GetFunctions().Add(node, index);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					int offset=-1;
					if(node->linking.HasLink())
					{
						offset=argument.linkings.IndexOf(node->linking);
					}
					else
					{
						BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->variables.Items()[node->name].type;
						int size=argument.info->GetTypeInfo(type)->size;
						char* data=new char[size];
						memset(data, 0, size);
						offset=(int)argument.globalData->Size();
						argument.globalData->Write(data, size);
						delete[] data;
					}
					argument.info->GetGlobalVariableOffsets().Add(node, offset);

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
						int functionStart=argument.il->instructions.Count();
						argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(0));
						int reserveVariablesIndex=argument.il->instructions.Count()-1;

						argument.info->BeginFunction();
						BasicLanguage_GenerateCode(node->statement, argument);
						argument.info->EndFunction(argument.il->instructions.Count(), argument.il);

						argument.il->instructions[reserveVariablesIndex].argument.int_value=argument.info->GetMaxVariableSpace();
						argument.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-argument.info->GetMaxVariableSpace()));
						BasicScope* functionScope=argument.info->GetEnv()->GetFunctionScope(node);
						BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(functionScope->OwnerDeclaration());
						int parameterSize=0;
						for(int i=0;i<functionType->ParameterCount();i++)
						{
							parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
						}
						argument.Ins(BasicIns::ret, BasicIns::MakeInt(parameterSize));

						BasicIL::Label label;
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