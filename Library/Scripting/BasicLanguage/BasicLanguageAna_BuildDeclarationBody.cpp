#include "BasicLanguageAnalyzer.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicLanguage_BuildDeclarationBody
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					BasicTypeRecord* functionType=argument.env->GetFunctionType(node, true);
					if(functionType->ParameterCount()==node->parameterNames.Count())
					{
						if(node->linking.HasLink())
						{
							if(node->statement)
							{
								argument.errors.Add(BasicLanguageCodeException::GetExternalFunctionCannotHaveStatement(node));
							}
							if(node->foreignFunction && node->genericDeclaration.HasGeneric())
							{
								argument.errors.Add(BasicLanguageCodeException::GetForeignFunctionCannotBeGeneric(node));
							}
						}
						else
						{
							if(!node->statement)
							{
								argument.errors.Add(BasicLanguageCodeException::GetLocalFunctionShouldHaveStatement(node));
							}
						}
						if(node->statement)
						{
							// if this is a generic function, the generic argument types have beed added in BuildGlobalScopePass1
							// and functionScope will be the previous created function scope
							BasicScope* functionScope=argument.env->CreateFunctionScope(argument.scope, node);
							for(vint i=0;i<node->parameterNames.Count();i++)
							{
								if(functionScope->variables.Items().Keys().Contains(node->parameterNames[i]))
								{
									argument.errors.Add(BasicLanguageCodeException::GetParameterAlreadyExists(node, i));
								}
								else
								{
									functionScope->variables.Add(node->parameterNames[i], BasicScope::Variable(i, functionType->ParameterType(i)));
								}
							}
							BP newArgument(argument, functionScope);
							BasicLanguage_CheckStatement(node->statement, newArgument);
						}
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetParameterCountNotMatch(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(node->linking.HasLink())
					{
						if(node->initializer)
						{
							argument.errors.Add(BasicLanguageCodeException::GetExternalVariableCannotHaveInitializer(node));
						}
					}
					// constant initializer is checked
					if(!node->constant && node->initializer)
					{
						BasicTypeRecord* initializerType=BasicLanguage_GetExpressionType(node->initializer, argument);
						try
						{
							BasicTypeRecord* variableType=BasicLanguage_GetTypeRecord(node->type, argument, false);
							if(initializerType && variableType)
							{
								if(!CanImplicitConvertTo(initializerType, variableType, node->initializer.Obj(), argument))
								{
									argument.errors.Add(BasicLanguageCodeException::GetInitializerTypeNotMatch(node));
								}
							}
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					if(node->linking.HasLink())
					{
						if(!node->defined)
						{
							argument.errors.Add(BasicLanguageCodeException::GetExternalStructureShouldBeDefined(node));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptBaseDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptInstanceDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildDeclarationBody(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody)

			void BasicLanguage_BuildDeclarationBody(Ptr<BasicProgram> program, BP& argument)
			{
				for(vint i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildDeclarationBody(program->declarations[i], argument);
				}
			}
		}
	}
}