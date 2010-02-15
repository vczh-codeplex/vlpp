/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicLanguage

Classes:
	BasicLanguageCodeException					�������������
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
					TypeNameNotExists,				//name
					FunctionAlreadyExists,			//name
					VariableAlreadyExists,			//name
					TypeAlreadyExists,				//name
					StructureMemberAlreadyExists,	//member name
					VariableNotExists,				//name
					FailToCast,
					VoidFunctionNotHaveResult,
					GlobalNotHaveResult,
					CannotInvokeNonFunctionValue,
					ArgumentNumberNotMatch,
					ArgumentTypeNotMatch,			//parameter index
					StructureMemberNotExists,		//member name
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
					ParameterAlreadyExists,			//parameter index
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
				static Ptr<BasicLanguageCodeException>							GetFunctionAlreadyExists(BasicFunctionDeclaration* function);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicVariableDeclaration* variable);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicVariableStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicStructureDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicTypeRenameDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, int memberIndex);
				static Ptr<BasicLanguageCodeException>							GetVariableNotExists(BasicReferenceExpression* variable);
				static Ptr<BasicLanguageCodeException>							GetFailToCast(BasicCastingExpression* casting);
				static Ptr<BasicLanguageCodeException>							GetVoidFunctionNotHaveResult(BasicFunctionResultExpression* result);
				static Ptr<BasicLanguageCodeException>							GetGlobalNotHaveResult(BasicFunctionResultExpression* result);
				static Ptr<BasicLanguageCodeException>							GetCannotInvokeNonFunctionValue(BasicInvokeExpression* invoke);
				static Ptr<BasicLanguageCodeException>							GetArgumentNumberNotMatch(BasicInvokeExpression* invoke);
				static Ptr<BasicLanguageCodeException>							GetArgumentTypeNotMatch(BasicInvokeExpression* invoke, int parameterIndex);
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
				static Ptr<BasicLanguageCodeException>							GetParameterCountNotMatch(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetParameterAlreadyExists(BasicFunctionDeclaration* declaration, int parameterIndex);
			};
		}
	}
}

#endif