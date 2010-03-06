/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
StringResource::ErrorMessages

Classes:
	ErrorMessages									���ַ�����Դ��
	
���ļ�ʹ��Vczh String Resource Code Generator�����Զ�����
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