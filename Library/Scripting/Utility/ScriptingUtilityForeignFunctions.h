/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
ScriptingUtility::ForeignFunction

Classes:
	LanguagePlugin								����Ӻ�����
***********************************************************************/

#ifndef VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS
#define VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS

#include "..\Languages\LanguageProvider.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			class LanguagePlugin : public Object
			{
			};

			extern Ptr<LanguagePlugin>		CreateMemoryManagerPlugin();
		}
	}
}

#endif