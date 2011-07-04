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
			};
		}
	}
}

#endif