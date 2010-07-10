/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
	BasicLanguageCodeException					£ºÓïÒå·ÖÎö´íÎó
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXCEPTION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXCEPTION

#include "..\..\Exception.h"
#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicLanguageCodeException : public Exception
			{
			public:
				enum ExceptionCode
				{
					TypeNameNotExists,						//name
					FunctionAlreadyExists,					//name
					VariableAlreadyExists,					//name
					TypeAlreadyExists,						//name
					StructureMemberAlreadyExists,			//member name
					VariableNotExists,						//name
					FailToCast,
					VoidFunctionNotHaveResult,
					GlobalNotHaveResult,
					CannotInvokeNonFunctionValue,
					ArgumentNumberNotMatch,
					ArgumentTypeNotMatch,					//parameter index
					StructureMemberNotExists,				//member name
					CannotConvertIndexToInt,
					CannotSubscribe,
					UnaryOperandShouldBeLeftValue,
					UnaryTypeNotMatch,
					BinaryLeftOperandShouldBeLeftValue,
					BinaryTypeNotMatch,
					ConditionCannotConvertToBool,
					BreakShouldBeInLooping,
					ContinueShouldBeInLooping,
					InitializerTypeNotMatch,
					ParameterCountNotMatch,
					ParameterAlreadyExists,					//parameter index
					StructureMemberCannotBeUndefinedType,	//member name
					LeftOperandShouldBeStructure,
					LeftOperandShouldBePointerToStructure,
					PredeclaredStructureShouldBeDefined,
					ExternalFunctionCannotHaveStatement,	//name
					LocalFunctionShouldHaveStatement,		//name
					ExternalVariableCannotHaveInitializer,	//name
					ExternalStructureShouldBeDefined,		//name
					GenericArgumentAlreadyExists,			//name
					CannotUseUninstanciatedGenericType,
					CannotUseUninstanciatedGenericVariable,	//name
					CannotUseUninstanciatedGenericFunction,	//name
					GenericArgumentNumberNotMatch,
					GenericArgumentCannotApplyToNonGenericType,
				};
			protected:
				BasicLanguageElement*											element;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters);
				BasicLanguageCodeException(const BasicLanguageCodeException& exception);
				~BasicLanguageCodeException();

				BasicLanguageElement*											GetBasicLanguageElement()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<BasicLanguageCodeException>							GetTypeNameNotExists(BasicReferenceType* type);
				static Ptr<BasicLanguageCodeException>							GetFunctionAlreadyExists(BasicDeclaration* function);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicDeclaration* variable);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicVariableStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, vint memberIndex);
				static Ptr<BasicLanguageCodeException>							GetVariableNotExists(BasicReferenceExpression* variable);
				static Ptr<BasicLanguageCodeException>							GetFailToCast(BasicCastingExpression* casting);
				static Ptr<BasicLanguageCodeException>							GetVoidFunctionNotHaveResult(BasicFunctionResultExpression* result);
				static Ptr<BasicLanguageCodeException>							GetGlobalNotHaveResult(BasicFunctionResultExpression* result);
				static Ptr<BasicLanguageCodeException>							GetCannotInvokeNonFunctionValue(BasicInvokeExpression* invoke);
				static Ptr<BasicLanguageCodeException>							GetArgumentNumberNotMatch(BasicInvokeExpression* invoke);
				static Ptr<BasicLanguageCodeException>							GetArgumentTypeNotMatch(BasicInvokeExpression* invoke, vint parameterIndex);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberNotExists(BasicMemberExpression* member);
				static Ptr<BasicLanguageCodeException>							GetCannotConvertIndexToInt(BasicSubscribeExpression* subscribe);
				static Ptr<BasicLanguageCodeException>							GetCannotSubscribe(BasicSubscribeExpression* subscribe);
				static Ptr<BasicLanguageCodeException>							GetUnaryOperandShouldBeLeftValue(BasicUnaryExpression* unary);
				static Ptr<BasicLanguageCodeException>							GetUnaryTypeNotMatch(BasicUnaryExpression* unary);
				static Ptr<BasicLanguageCodeException>							GetBinaryLeftOperandShouldBeLeftValue(BasicBinaryExpression* binary);
				static Ptr<BasicLanguageCodeException>							GetBinaryTypeNotMatch(BasicBinaryExpression* binary);
				static Ptr<BasicLanguageCodeException>							GetConditionCannotConvertToBool(BasicExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetBreakShouldBeInLooping(BasicBreakStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetContinueShouldBeInLooping(BasicContinueStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetInitializerTypeNotMatch(BasicVariableStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetInitializerTypeNotMatch(BasicVariableDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetParameterCountNotMatch(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetParameterAlreadyExists(BasicFunctionDeclaration* declaration, vint parameterIndex);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberCannotBeUndefinedType(BasicStructureDeclaration* declaration, vint memberIndex);
				static Ptr<BasicLanguageCodeException>							GetLeftOperandShouldBeStructure(BasicMemberExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetLeftOperandShouldBePointerToStructure(BasicMemberExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetPredeclaredStructureShouldBeDefined(BasicStructureDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalFunctionCannotHaveStatement(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetLocalFunctionShouldHaveStatement(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalVariableCannotHaveInitializer(BasicVariableDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalStructureShouldBeDefined(BasicStructureDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentAlreadyExists(BasicDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericType(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericVariable(BasicReferenceExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericFunction(BasicReferenceExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentNumberNotMatch(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentNumberNotMatch(BasicInstanciatedExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentCannotApplyToNonGenericType(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentCannotApplyToNonGenericType(BasicInstanciatedExpression* expression);
			};
		}
	}
}

#endif