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
BasicLanguage_GenerateCodePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					argument.info->GetFunctions().Add(node);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->variables.Items()[node->name].type;
					int size=argument.info->GetTypeInfo(type)->size;
					char* data=new char[size];
					memset(data, 0, size);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, size);
					argument.info->GetGlobalVariableOffsets().Add(node, offset);
					delete[] data;

					if(node->initializer)
					{
						// TODO: Optimize for big type
						BasicLanguage_PushValue(node->initializer, argument, type);
						argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
						Code_Write(type, argument);
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

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(node->statement)
					{
						int functionStart=argument.il->instructions.Count();
						argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(0));
						int reserveVariablesIndex=argument.il->instructions.Count()-1;

						argument.info->BeginFunction();
						BasicLanguage_GenerateCode(node->statement, argument);
						argument.info->EndFunction(argument.il->instructions.Count(), argument.il);

						argument.il->instructions[reserveVariablesIndex].argument.int_value=argument.info->GetMaxVariableSpace();
						argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-argument.info->GetMaxVariableSpace()));
						BasicScope* functionScope=argument.info->GetEnv()->GetFunctionScope(node);
						BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(functionScope->OwnerDeclaration());
						int parameterSize=0;
						for(int i=0;i<functionType->ParameterCount();i++)
						{
							parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
						}
						argument.il->Ins(BasicIns::ret, BasicIns::MakeInt(parameterSize));

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