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
				static WString ILExceptionStackOverflow();
				static WString ILExceptionDividByZero();
				static WString ILExceptionAccessViolation();
				static WString ILExceptionInstructionIndexOutOfRange();
				static WString ILExceptionUnknownInstruction();
				static WString ILExceptionBadInstructionArgument();
			};
		}
	}
}

#endif