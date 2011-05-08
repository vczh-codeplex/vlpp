#include "BasicErrorMessageTranslator.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
BasicErrorMessageTranslator
***********************************************************************/

			Ptr<LanguageException> BasicErrorMessageTranslator::Translate(Ptr<BasicLanguageCodeException> error, vint codeIndex)
			{
				WString message;
				switch(error->GetExceptionCode())
				{
				case BasicLanguageCodeException::TypeNameNotExists:
					{
						message=BasicErrorMessage::TypeNameNotExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::FunctionAlreadyExists:
					{
						message=BasicErrorMessage::FunctionAlreadyExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::VariableAlreadyExists:
					{
						message=BasicErrorMessage::VariableAlreadyExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::TypeAlreadyExists:
					{
						message=BasicErrorMessage::TypeAlreadyExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::StructureMemberAlreadyExists:
					{
						message=BasicErrorMessage::StructureMemberAlreadyExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::VariableNotExists:
					{
						message=BasicErrorMessage::VariableNotExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::FailToCast:
					{
						BasicCastingExpression* expression=dynamic_cast<BasicCastingExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::FailToCast(ToString(GetExpressionType(expression->operand.Obj())), ToString(expression->type.Obj()));
					}
					break;
				case BasicLanguageCodeException::VoidFunctionNotHaveResult:
					{
						message=BasicErrorMessage::VoidFunctionNotHaveResult();
					}
					break;
				case BasicLanguageCodeException::GlobalNotHaveResult:
					{
						message=BasicErrorMessage::GlobalNotHaveResult();
					}
					break;
				case BasicLanguageCodeException::CannotInvokeNonFunctionValue:
					{
						BasicInvokeExpression* expression=dynamic_cast<BasicInvokeExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::CannotInvokeNonFunctionValue(ToString(GetExpressionType(expression->function.Obj())));
					}
					break;
				case BasicLanguageCodeException::ArgumentNumberNotMatch:
					{
						message=BasicErrorMessage::ArgumentNumnerNotMatch();
					}
					break;
				case BasicLanguageCodeException::ArgumentTypeNotMatch:
					{
						BasicInvokeExpression* expression=dynamic_cast<BasicInvokeExpression*>(error->GetBasicLanguageElement());
						vint index=wtoi(error->GetParameters()[0]);
						WString to=ToString(GetExpressionType(expression->function.Obj())->ParameterType(index));
						WString from=ToString(GetExpressionType(expression->arguments[index].Obj()));
						message=BasicErrorMessage::ArgumentTypeNotMatch(itow(index+1), from, to);
					}
					break;
				case BasicLanguageCodeException::StructureMemberNotExists:
					{
						message=BasicErrorMessage::StructureMemberNotExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::CannotConvertIndexToInt:
					{
						BasicSubscribeExpression* expression=dynamic_cast<BasicSubscribeExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::CannotConvertIndexToInt(ToString(GetExpressionType(expression->subscribe.Obj())), ToString(GetIntegerType()));
					}
					break;
				case BasicLanguageCodeException::CannotSubscribe:
					{
						BasicSubscribeExpression* expression=dynamic_cast<BasicSubscribeExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::CannotSubscribe(ToString(GetExpressionType(expression->operand.Obj())));
					}
					break;
				case BasicLanguageCodeException::UnaryOperandShouldBeLeftValue:
					{
						message=BasicErrorMessage::UnaryOperandShouldBeLeftValue();
					}
					break;
				case BasicLanguageCodeException::UnaryTypeNotMatch:
					{
						BasicUnaryExpression* expression=dynamic_cast<BasicUnaryExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::UnaryTypeNotMatch(OpToString(expression), ToString(GetExpressionType(expression->operand.Obj())));
					}
					break;
				case BasicLanguageCodeException::BinaryLeftOperandShouldBeLeftValue:
					{
						BasicBinaryExpression* expression=dynamic_cast<BasicBinaryExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::BinaryLeftOperandShouldBeLeftValue(OpToString(expression));
					}
					break;
				case BasicLanguageCodeException::BinaryTypeNotMatch:
					{
						BasicBinaryExpression* expression=dynamic_cast<BasicBinaryExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::BinaryTypeNotMatch(OpToString(expression), ToString(GetExpressionType(expression->leftOperand.Obj())), ToString(GetExpressionType(expression->rightOperand.Obj())));
					}
					break;
				case BasicLanguageCodeException::ConditionCannotConvertToBool:
					{
						BasicExpression* expression=dynamic_cast<BasicExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ConditionCannotConvertToBool(ToString(GetExpressionType(expression)), ToString(GetBooleanType()));
					}
					break;
				case BasicLanguageCodeException::BreakShouldBeInLooping:
					{
						message=BasicErrorMessage::BreakShouldBeInLooping(BreakStatementToString());
					}
					break;
				case BasicLanguageCodeException::ContinueShouldBeInLooping:
					{
						message=BasicErrorMessage::ContinueShouldBeInLooping(ContinueStatementToString());
					}
					break;
				case BasicLanguageCodeException::InitializerTypeNotMatch:
					{
						WString from, to;
						if(BasicVariableStatement* statement=dynamic_cast<BasicVariableStatement*>(error->GetBasicLanguageElement()))
						{
							from=ToString(statement->type.Obj());
							to=ToString(GetExpressionType(statement->initializer.Obj()));
						}
						else
						{
							BasicVariableDeclaration* declaration=dynamic_cast<BasicVariableDeclaration*>(error->GetBasicLanguageElement());
							from=ToString(declaration->type.Obj());
							to=ToString(GetExpressionType(declaration->initializer.Obj()));
						}
						message=BasicErrorMessage::InitializerTypeNotMatch(from, to);
					}
					break;
				case BasicLanguageCodeException::ParameterCountNotMatch:
					{
						message=BasicErrorMessage::ParameterCountNotMatch();
					}
					break;
				case BasicLanguageCodeException::ParameterAlreadyExists:
					{
						BasicFunctionDeclaration* declaration=dynamic_cast<BasicFunctionDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ParameterAlreadyExists(declaration->parameterNames[wtoi(error->GetParameters()[0])]);
					}
					break;
				case BasicLanguageCodeException::StructureMemberCannotBeUndefinedType:
					{
						message=BasicErrorMessage::StructureMemberCannotBeUndefinedType(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::LeftOperandShouldBeStructure:
					{
						message=BasicErrorMessage::LeftOperandShouldBeStructure();
					}
					break;
				case BasicLanguageCodeException::LeftOperandShouldBePointerToStructure:
					{
						message=BasicErrorMessage::LeftOperandShouldBePointerToStructure();
					}
					break;
				case BasicLanguageCodeException::PredeclaredStructureShouldBeDefined:
					{
						BasicStructureDeclaration* declaration=dynamic_cast<BasicStructureDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::PredeclaredStructureShouldBeDefined(declaration->name);
					}
					break;
				case BasicLanguageCodeException::ExternalFunctionCannotHaveStatement:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ExternalFunctionCannotHaveStatement(declaration->name);
					}
					break;
				case BasicLanguageCodeException::LocalFunctionShouldHaveStatement:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::LocalFunctionShouldHaveStatement(declaration->name);
					}
					break;
				case BasicLanguageCodeException::ExternalVariableCannotHaveInitializer:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ExternalVariableCannotHaveInitializer(declaration->name);
					}
					break;
				case BasicLanguageCodeException::ExternalStructureShouldBeDefined:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ExternalStructureShouldBeDefined(declaration->name);
					}
					break;
				case BasicLanguageCodeException::GenericArgumentAlreadyExists:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::GenericArgumentAlreadyExists(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::GenericArgumentNotExists:
					{
						BasicDeclaration* declaration=dynamic_cast<BasicDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::GenericArgumentNotExists(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::CannotUseUninstanciatedGenericType:
					{
						message=BasicErrorMessage::CannotUseUninstanciatedGenericType();
					}
					break;
				case BasicLanguageCodeException::CannotUseUninstanciatedGenericVariable:
					{
						BasicReferenceExpression* reference=dynamic_cast<BasicReferenceExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::CannotUseUninstanciatedGenericVariable(reference->name);
					}
					break;
				case BasicLanguageCodeException::CannotUseUninstanciatedGenericFunction:
					{
						BasicReferenceExpression* reference=dynamic_cast<BasicReferenceExpression*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::CannotUseUninstanciatedGenericFunction(reference->name);
					}
					break;
				case BasicLanguageCodeException::GenericArgumentNumberNotMatch:
					{
						message=BasicErrorMessage::GenericArgumentNumberNotMatch();
					}
					break;
				case BasicLanguageCodeException::GenericArgumentCannotApplyToNonGenericType:
					{
						message=BasicErrorMessage::GenericArgumentCannotApplyToNonGenericType();
					}
					break;
				case BasicLanguageCodeException::ConceptFunctionAlreadyExists:
					{
						BasicConceptBaseDeclaration* declaration=dynamic_cast<BasicConceptBaseDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ConceptFunctionAlreadyExists(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::ConceptFunctionNotExists:
					{
						message=BasicErrorMessage::ConceptFunctionNotExists(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::ConceptAlreadyExists:
					{
						message=BasicErrorMessage::ConceptAlreadyExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::ConceptNotExists:
					{
						message=BasicErrorMessage::ConceptNotExists(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::InstanceTypeNotCorrect:
					{
						BasicConceptInstanceDeclaration* declaration=dynamic_cast<BasicConceptInstanceDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::InstanceTypeNotCorrect(declaration->name);
					}
					break;
				case BasicLanguageCodeException::ConceptFunctionTypeNotMatches:
					{
						BasicConceptInstanceDeclaration* declaration=dynamic_cast<BasicConceptInstanceDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ConceptFunctionTypeNotMatches(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::InstanceShouldHaveFunction:
					{
						BasicConceptInstanceDeclaration* declaration=dynamic_cast<BasicConceptInstanceDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::InstanceShouldHaveFunction(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::InstanceShouldNotHaveFunction:
					{
						BasicConceptInstanceDeclaration* declaration=dynamic_cast<BasicConceptInstanceDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::InstanceShouldNotHaveFunction(declaration->name, error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::InstanceShouldBeDeclaredOnType:
					{
						message=BasicErrorMessage::InstanceShouldBeDeclaredOnType(ToString(error->GetTypeParameter()), error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::CannotThrowVoidValue:
					{
						message=BasicErrorMessage::CannotThrowVoidValue();
					}
					break;
				case BasicLanguageCodeException::ForeignFunctionCannotBeGeneric:
					{
						BasicFunctionDeclaration* declaration=dynamic_cast<BasicFunctionDeclaration*>(error->GetBasicLanguageElement());
						message=BasicErrorMessage::ForeignFunctionCannotBeGeneric(declaration->name);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnFunctionDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnFunctionDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnStructureDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnStructureDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnVariableDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnVariableDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnTypeRenameDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnTypeRenameDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnConceptDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnConceptDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::AttributeCannotApplyOnInstanceDeclaration:
					{
						message=BasicErrorMessage::AttributeCannotApplyOnInstanceDeclaration(error->GetParameters()[0], error->GetParameters()[1]);
					}
					break;
				case BasicLanguageCodeException::PublicDeclarationCannotUseNonPublicType:
					{
						WString attribute=error->GetParameters()[0];
						WString type=ToString(error->GetTypeParameter());
						message=BasicErrorMessage::PublicDeclarationCannotUseNonPublicType(attribute, type);
					}
					break;
				case BasicLanguageCodeException::InstanceDeclaredOnPublicConceptShouldBePublic:
					{
						message=BasicErrorMessage::InstanceDeclaredOnPublicConceptShouldBePublic(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::InstanceDeclaredOnNonPublicConceptShouldBeNonPublic:
					{
						message=BasicErrorMessage::InstanceDeclaredOnNonPublicConceptShouldBeNonPublic(error->GetParameters()[0]);
					}
					break;
				case BasicLanguageCodeException::ArrayElementCountShouldBeIntegerConstantExpression:
					{
						message=BasicErrorMessage::ArrayElementCountShouldBeIntegerConstantExpression();
					}
					break;
				case BasicLanguageCodeException::ArrayElementCountShouldBePositive:
					{
						message=BasicErrorMessage::ArrayElementCountShouldBePositive();
					}
					break;
				default:
					return 0;
				}
				return new LanguageException(
					message,
					error->GetBasicLanguageElement()->position.lineIndex,
					error->GetBasicLanguageElement()->position.lineStart,
					error->GetBasicLanguageElement()->position.start,
					codeIndex
					);
			}
		}
	}
}