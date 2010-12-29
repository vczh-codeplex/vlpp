/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
ScriptingUtility::ForeignFunction

Classes:
	LanguagePlugin								：外接函数报
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
			protected:
				basicil::BasicILInterpretor*	ownerInterpretor;
				basicil::BasicILLinker*			ownerLinker;

				virtual bool					RegisterForeignFunctions(basicil::BasicILRuntimeSymbol* symbol)=0;
			public:
				LanguagePlugin();
				~LanguagePlugin();

				basicil::BasicILInterpretor*	GetOwnerInterpretor();
				basicil::BasicILLinker*			GetOwnerLinker();
				bool							Install(basicil::BasicILInterpretor* interpretor);
				bool							Install(basicil::BasicILLinker* linker);
			};

			extern Ptr<LanguagePlugin>		CreateMemoryManagerPlugin();
		}
	}
}

#endif