/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
StringResource::BasicErrorMessage

Classes:
	BasicErrorMessage									：字符串资源类
	
本文件使用Vczh String Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_BASICLANGUAGE_BASICERRORMESSAGE
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_BASICLANGUAGE_BASICERRORMESSAGE

#include "..\..\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicErrorMessage
			{
			public:
				static WString TypeNameNotExists(const WString& type);
				static WString FunctionAlreadyExists(const WString& name);
				static WString VariableAlreadyExists(const WString& name);
				static WString TypeAlreadyExists(const WString& name);
				static WString StructureMemberAlreadyExists(const WString& name);
				static WString VariableNotExists(const WString& name);
				static WString FailToCast(const WString& from, const WString& to);
				static WString VoidFunctionNotHaveResult();
				static WString GlobalNotHaveResult();
				static WString CannotInvokeNonFunctionValue(const WString& type);
				static WString ArgumentNumnerNotMatch();
				static WString ArgumentTypeNotMatch(const WString& index, const WString& from, const WString& to);
				static WString StructureMemberNotExists(const WString& name);
				static WString CannotConvertIndexToInt(const WString& from, const WString& to);
				static WString CannotSubscribe(const WString& type);
				static WString UnaryOperandShouldBeLeftValue();
				static WString UnaryTypeNotMatch(const WString& op, const WString& type);
				static WString BinaryLeftOperandShouldBeLeftValue(const WString& op);
				static WString BinaryTypeNotMatch(const WString& op, const WString& left, const WString& right);
				static WString ConditionCannotConvertToBool(const WString& from, const WString& boolean);
				static WString BreakShouldBeInLooping(const WString& breakStatement);
				static WString ContinueShouldBeInLooping(const WString& continueStatement);
				static WString InitializerTypeNotMatch(const WString& from, const WString& to);
				static WString ParameterCountNotMatch();
				static WString ParameterAlreadyExists(const WString& name);
				static WString StructureMemberCannotBeUndefinedType(const WString& name);
				static WString LeftOperandShouldBeStructure();
				static WString LeftOperandShouldBePointerToStructure();
				static WString PredeclaredStructureShouldBeDefined(const WString& name);
				static WString ExternalFunctionCannotHaveStatement(const WString& name);
				static WString LocalFunctionShouldHaveStatement(const WString& name);
				static WString ExternalVariableCannotHaveInitializer(const WString& name);
				static WString ExternalStructureShouldBeDefined(const WString& name);
				static WString GenericArgumentAlreadyExists(const WString& declaration, const WString& name);
				static WString GenericArgumentNotExists(const WString& declaration, const WString& name);
				static WString CannotUseUninstantiatedGenericType();
				static WString CannotUseUninstantiatedGenericVariable(const WString& name);
				static WString CannotUseUninstantiatedGenericFunction(const WString& name);
				static WString GenericArgumentNumberNotMatch();
				static WString GenericArgumentCannotApplyToNonGenericType();
				static WString ConceptFunctionAlreadyExists(const WString& name, const WString& function);
				static WString ConceptFunctionNotExists(const WString& name, const WString& function);
				static WString ConceptAlreadyExists(const WString& name);
				static WString ConceptNotExists(const WString& name);
				static WString InstanceTypeNotCorrect(const WString& name);
				static WString ConceptFunctionTypeNotMatches(const WString& name, const WString& function);
				static WString InstanceShouldHaveFunction(const WString& name, const WString& function);
				static WString InstanceShouldNotHaveFunction(const WString& name, const WString& function);
				static WString InstanceShouldBeDeclaredOnType(const WString& type, const WString& conceptName);
				static WString CannotThrowVoidValue();
				static WString ForeignFunctionCannotBeGeneric(const WString& name);
				static WString AttributeCannotApplyOnFunctionDeclaration(const WString& attribute, const WString& name);
				static WString AttributeCannotApplyOnStructureDeclaration(const WString& attribute, const WString& name);
				static WString AttributeCannotApplyOnVariableDeclaration(const WString& attribute, const WString& name);
				static WString AttributeCannotApplyOnTypeRenameDeclaration(const WString& attribute, const WString& name);
				static WString AttributeCannotApplyOnConceptDeclaration(const WString& attribute, const WString& name);
				static WString AttributeCannotApplyOnInstanceDeclaration(const WString& attribute, const WString& name);
				static WString PublicDeclarationCannotUseNonPublicType(const WString& name, const WString& type);
				static WString InstanceDeclaredOnPublicConceptShouldBePublic(const WString& concept);
				static WString InstanceDeclaredOnNonPublicConceptShouldBeNonPublic(const WString& concept);
				static WString ArrayElementCountShouldBeIntegerConstantExpression();
				static WString ArrayElementCountShouldBePositive();
				static WString ConstantInitializerShouldBeConstantExpression();
				static WString ConstantCannotBeGeneric();
				static WString ConstantCannotBeExternalSymbol();
				static WString ILExceptionStackOverflow();
				static WString ILExceptionDividByZero();
				static WString ILExceptionAccessViolation();
				static WString ILExceptionInstructionIndexOutOfRange();
				static WString ILExceptionUnknownInstruction();
				static WString ILExceptionBadInstructionArgument();
				static WString ILExceptionUnhandledException();
				static WString ILLinkerExceptionDuplicatedAssemblyName(const WString& assembly);
				static WString ILLinkerExceptionAssemblyNotExists(const WString& assembly);
				static WString ILLinkerExceptionDuplicatedSymbolName(const WString& assembly, const WString& symbol);
				static WString ILLinkerExceptionSymbolNotExists(const WString& assembly, const WString& symbol);
				static WString ILLinkerExceptionSymbolNotALabel(const WString& assembly, const WString& symbol);
				static WString ILLinkerExceptionDuplicatedInstance(const WString& name);
				static WString ILLinkerExceptionForeignFunctionNotExists(const WString& assembly, const WString& symbol);
			};
		}
	}
}

#endif