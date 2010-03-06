/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
StringResource::ErrorMessages

Classes:
	ErrorMessages									：字符串资源类
	
本文件使用Vczh String Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_BASICLANGUAGE_ERRORMESSAGES
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_BASICLANGUAGE_ERRORMESSAGES

#include "..\.\..\..\Library\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class ErrorMessages
			{
			public:
				static WString VariableAlreadyExists(const WString& variable);
				static WString VoidFunctionDoesNotHaveResult();
			};
		}
	}
}

#endif