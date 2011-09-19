/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
StringResource::ManagedErrorMessage

Classes:
	ManagedErrorMessage									：字符串资源类
	
本文件使用Vczh String Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_MANAGEDLANGUAGE_MANAGEDERRORMESSAGE
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_MANAGEDLANGUAGE_MANAGEDERRORMESSAGE

#include "..\..\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedErrorMessage
			{
			public:
				static WString SymbolAlreadyDefined(const WString& name);
				static WString NamespaceNotExists(const WString& name);
				static WString FullPathTypeNotExists(const WString& name);
				static WString FullPathTypeDuplicated(const WString& name);
				static WString ScopedTypeNotExists(const WString& name);
				static WString ScopedTypeDuplicated(const WString& name);
				static WString IllegalAutoRefer(const WString& autoRefType);
				static WString CannotDirectReferTypeRenameAsToplevel(const WString& name);
				static WString IllegalNestedDeclaration();
				static WString TypeInvisible(const WString& name);
				static WString TypeInvisibleOutSideOfAssembly(const WString& name);
				static WString CannotUseUninstantiatedGenericType(const WString& name);
				static WString GenericTypeArgumentCountNotMatches(const WString& name);
				static WString GenericTypeTypeConstraintNotSatisfied(const WString& name, const WString& index);
				static WString GenericTypeNewConstraintNotSatisfied(const WString& name, const WString& index);
				static WString TypeCannotDerivedFromItself(const WString& name);
				static WString ClassIllegalBaseClass(const WString& name);
				static WString StructureIllegalBaseClass(const WString& name);
				static WString InterfaceIllegalBaseClass(const WString& name);
				static WString StaticTypeIllegalBaseClass(const WString& name);
				static WString ClassStructureTooMuchBaseClasses(const WString& name);
				static WString ClassStructureIllegalInheritation(const WString& name);
				static WString InterfaceIllegalInheritation(const WString& name);
				static WString InterfaceIllegalMemberType(const WString& name);
				static WString StaticTypeIllegalInheritance(const WString& name);
				static WString TypeIllegalAccessor(const WString& name);
				static WString InterfaceMemberIllegalType(const WString& name, const WString& member);
				static WString InterfaceMemberIllegalAccessor(const WString& name, const WString& member);
				static WString InterfaceMemberIllegalMemberType(const WString& name, const WString& member);
				static WString InterfaceMemberIllegalInheritation(const WString& name, const WString& member);
				static WString InterfaceMemberIllegalImplementedInterface(const WString& name, const WString& member);
				static WString SealedTypeMemberIllegalAccessor(const WString& name, const WString& member);
				static WString SealedTypeMemberIllegalInheritance(const WString& name, const WString& member);
				static WString StaticTypeMemberIllegalAccessor(const WString& name, const WString& member);
				static WString StaticTypeMemberIllegalInheritance(const WString& name, const WString& member);
				static WString StaticTypeMemberIllegalMemberType(const WString& name, const WString& member);
				static WString NonAbstractTypeMemberIllegalInheritance(const WString& name, const WString& member);
				static WString InterfaceImplementedMemberIllegalMemberType(const WString& name, const WString& member);
				static WString InterfaceImplementedMemberIllegalInheritation(const WString& name, const WString& member);
				static WString MemberImplementedInterfaceTypeNotExists(const WString& name, const WString& member);
				static WString MemberOverridedTargetNotExists(const WString& name, const WString& member);
				static WString MemberOverridedTargetIllegalAccessor(const WString& name, const WString& member);
				static WString ExpressionResolvedToDuplicatedTargets();
				static WString ExpressionResolvedToDuplicatedTargets(const WString& type);
				static WString ExpressionResolvingFailed(const WString& name);
				static WString ExpressionCannotConvertToType(const WString& type);
				static WString IllegalAutoRefWithoutInitializer(const WString& name, const WString& autoRefType);
				static WString VariableAlreadyExists(const WString& name);
				static WString IllegalBreak();
				static WString IllegalContinue();
				static WString IllegalThrow();
				static WString ExceptionTypeShouldDerivedFromException(const WString& type, const WString& exceptionType);
				static WString IllegalThis(const WString& expr);
				static WString IllegalBase(const WString& expr);
				static WString ExpressionIsNotValue(const WString& expr);
				static WString MatchedMethodNotExists(const WString& expr);
				static WString MatchedMethodTooMuch(const WString& expr);
				static WString IllegalNull(const WString& expr);
			};
		}
	}
}

#endif