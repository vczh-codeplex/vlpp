/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
StringResource::NativeXErrorMessage

Classes:
	NativeXErrorMessage									���ַ�����Դ��
	
���ļ�ʹ��Vczh String Resource Code Generator�����Զ�����
***********************************************************************/

#ifndef VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_NATIVEX_NATIVEXERRORMESSAGE
#define VCZH_STRING_RESOURCE_CODE_FILE_VL_SCRIPTING_LANGUAGE_NATIVEX_NATIVEXERRORMESSAGE

#include "..\..\..\String.h"

using namespace vl;

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			class NativeXErrorMessage
			{
			public:
				static WString UnrecognizedToken(const WString& token);
				static WString UnitAlreadyExists(const WString& name);
				static WString UnitNotExists(const WString& name);
				static WString UnitCircularReferenced(const WString& name);
			};
		}
	}
}

#endif