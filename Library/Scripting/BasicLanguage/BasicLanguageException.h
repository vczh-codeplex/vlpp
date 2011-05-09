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
#include "BasicLanguageTypeManager.h"

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
					TypeNameNotExists,										//name
					FunctionAlreadyExists,									//name
					VariableAlreadyExists,									//name
					TypeAlreadyExists,										//name
					StructureMemberAlreadyExists,							//member name
					VariableNotExists,										//name
					FailToCast,
					VoidFunctionNotHaveResult,
					GlobalNotHaveResult,
					CannotInvokeNonFunctionValue,
					ArgumentNumberNotMatch,
					ArgumentTypeNotMatch,									//parameter index
					StructureMemberNotExists,								//member name
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
					ParameterAlreadyExists,									//parameter index
					StructureMemberCannotBeUndefinedType,					//member name
					LeftOperandShouldBeStructure,
					LeftOperandShouldBePointerToStructure,
					PredeclaredStructureShouldBeDefined,
					ExternalFunctionCannotHaveStatement,					//name
					LocalFunctionShouldHaveStatement,						//name
					ExternalVariableCannotHaveInitializer,					//name
					ExternalStructureShouldBeDefined,						//name
					GenericArgumentAlreadyExists,							//name
					GenericArgumentNotExists,								//name
					CannotUseUninstanciatedGenericType,
					CannotUseUninstanciatedGenericVariable,					//name
					CannotUseUninstanciatedGenericFunction,					//name
					GenericArgumentNumberNotMatch,
					GenericArgumentCannotApplyToNonGenericType,
					ConceptFunctionAlreadyExists,							//name
					ConceptFunctionNotExists,								//conceptName, functionName
					ConceptAlreadyExists,									//name
					ConceptNotExists,										//name
					InstanceTypeNotCorrect,
					ConceptFunctionTypeNotMatches,							//name
					InstanceShouldHaveFunction,								//name
					InstanceShouldNotHaveFunction,							//name
					InstanceShouldBeDeclaredOnType,							//concept-name, [typeParameter for type information]
					CannotThrowVoidValue,
					ForeignFunctionCannotBeGeneric,							//name
					AttributeCannotApplyOnFunctionDeclaration,				//attribute-name, declaration-name
					AttributeCannotApplyOnStructureDeclaration,				//attribute-name, declaration-name
					AttributeCannotApplyOnVariableDeclaration,				//attribute-name, declaration-name
					AttributeCannotApplyOnTypeRenameDeclaration,			//attribute-name, declaration-name
					AttributeCannotApplyOnConceptDeclaration,				//attribute-name, declaration-name
					AttributeCannotApplyOnInstanceDeclaration,				//attribute-name, declaration-name
					PublicDeclarationCannotUseNonPublicType,				//declaration-name, [typeParameter for type information]
					InstanceDeclaredOnPublicConceptShouldBePublic,			//concept-name
					InstanceDeclaredOnNonPublicConceptShouldBeNonPublic,	//concept-name
					ArrayElementCountShouldBeIntegerConstantExpression,
					ArrayElementCountShouldBePositive,
					ConstantInitializerShouldBeConstantExpression,
					ConstantCannotBeGeneric,
					ConstantCannotBeExternalSymbol,
				};
			protected:
				BasicLanguageElement*											element;
				BasicTypeRecord*												typeParameter;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, BasicTypeRecord* _typeParameter=0);
				BasicLanguageCodeException(const BasicLanguageCodeException& exception);
				~BasicLanguageCodeException();

				BasicLanguageElement*											GetBasicLanguageElement()const;
				BasicTypeRecord*												GetTypeParameter()const;
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
				static Ptr<BasicLanguageCodeException>							GetStructureMemberNotExists(BasicOffsetofExpression* member);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberNotExists(BasicTypeofMemberType* member);
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
				static Ptr<BasicLanguageCodeException>							GetLeftOperandShouldBeStructure(BasicOffsetofExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetLeftOperandShouldBeStructure(BasicTypeofMemberType* type);
				static Ptr<BasicLanguageCodeException>							GetLeftOperandShouldBePointerToStructure(BasicMemberExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetPredeclaredStructureShouldBeDefined(BasicStructureDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalFunctionCannotHaveStatement(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetLocalFunctionShouldHaveStatement(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalVariableCannotHaveInitializer(BasicVariableDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetExternalStructureShouldBeDefined(BasicStructureDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentAlreadyExists(BasicDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentNotExists(BasicDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericType(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericVariable(BasicReferenceExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetCannotUseUninstanciatedGenericFunction(BasicReferenceExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentNumberNotMatch(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentNumberNotMatch(BasicInstanciatedExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentCannotApplyToNonGenericType(BasicType* type);
				static Ptr<BasicLanguageCodeException>							GetGenericArgumentCannotApplyToNonGenericType(BasicInstanciatedExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetConceptFunctionAlreadyExists(BasicConceptBaseDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetConceptFunctionNotExists(BasicInstanceFunctionExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetConceptAlreadyExists(BasicDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetConceptNotExists(BasicConceptInstanceDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetConceptNotExists(BasicDeclaration* declaration, const WString& conceptName);
				static Ptr<BasicLanguageCodeException>							GetInstanceTypeNotCorrect(BasicConceptInstanceDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetConceptFunctionTypeNotMatches(BasicConceptInstanceDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetInstanceShouldHaveFunction(BasicConceptInstanceDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetInstanceShouldNotHaveFunction(BasicConceptInstanceDeclaration* declaration, const WString& name);
				static Ptr<BasicLanguageCodeException>							GetInstanceShouldBeDeclaredOnType(BasicType* typeExpression, BasicTypeRecord* type, const WString& conceptName);
				static Ptr<BasicLanguageCodeException>							GetCannotThrowVoidValue(BasicThrowStatement* statement);
				static Ptr<BasicLanguageCodeException>							GetForeignFunctionCannotBeGeneric(BasicFunctionDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnFunctionDeclaration(BasicFunctionDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnStructureDeclaration(BasicStructureDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnVariableDeclaration(BasicVariableDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnTypeRenameDeclaration(BasicTypeRenameDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnConceptDeclaration(BasicConceptBaseDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetAttributeCannotApplyOnInstanceDeclaration(BasicConceptInstanceDeclaration* declaration, const WString& attributeName);
				static Ptr<BasicLanguageCodeException>							GetPublicDeclarationCannotUseNonPublicType(BasicDeclaration* declaration, BasicTypeRecord* type);
				static Ptr<BasicLanguageCodeException>							GetInstanceDeclaredOnPublicConceptShouldBePublic(BasicConceptInstanceDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetInstanceDeclaredOnNonPublicConceptShouldBeNonPublic(BasicConceptInstanceDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetArrayElementCountShouldBeIntegerConstantExpression(BasicExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetArrayElementCountShouldBePositive(BasicExpression* expression);
				static Ptr<BasicLanguageCodeException>							GetConstantInitializerShouldBeConstantExpression(BasicVariableDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetConstantCannotBeGeneric(BasicVariableDeclaration* declaration);
				static Ptr<BasicLanguageCodeException>							GetConstantCannotBeExternalSymbol(BasicVariableDeclaration* declaration);
			};
		}
	}
}

#endif