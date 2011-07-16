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
			};
		}
	}
}

#endif