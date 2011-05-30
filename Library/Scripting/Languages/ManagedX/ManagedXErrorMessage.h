/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
StringResource::ManagedXErrorMessage

Classes:
	ManagedXErrorMessage									���ַ�����Դ��
	
���ļ�ʹ��Vczh String Resource Code Generator�����Զ�����
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
				static WString NeedOpenDeclBrace();
				static WString NeedCloseDeclBrace();
				static WString NeedSemicolon();
			};
		}
	}
}

#endif