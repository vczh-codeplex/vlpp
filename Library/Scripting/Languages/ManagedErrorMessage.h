/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
StringResource::ManagedErrorMessage

Classes:
	ManagedErrorMessage									���ַ�����Դ��
	
���ļ�ʹ��Vczh String Resource Code Generator�����Զ�����
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
				static WString ExpressionCannotConvertToType(const WString& type);
				static WString IllegalAutoRefWithoutInitializer(const WString& name, const WString& autoRefType);
				static WString VariableAlreadyExists(const WString& name);
			};
		}
	}
}

#endif