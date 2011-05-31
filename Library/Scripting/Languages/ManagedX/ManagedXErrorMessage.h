/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
StringResource::ManagedXErrorMessage

Classes:
	ManagedXErrorMessage									：字符串资源类
	
本文件使用Vczh String Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_MANAGEDX_MANAGEDXERRORMESSAGE
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_MANAGEDX_MANAGEDXERRORMESSAGE

#include "..\..\..\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			class ManagedXErrorMessage
			{
			public:
				static WString UnrecognizedToken(const WString& token);
				static WString NeedId();
				static WString NeedColon();
				static WString NeedSemicolon();
				static WString NeedEq();
				static WString NeedLt();
				static WString NeedGt();
				static WString NeedOpenDeclBrace();
				static WString NeedCloseDeclBrace();
				static WString NeedOpenArrayBrace();
				static WString NeedCloseArrayBrace();
				static WString NeedOpenExpBrace();
				static WString NeedCloseExpBrace();
			};
		}
	}
}

#endif