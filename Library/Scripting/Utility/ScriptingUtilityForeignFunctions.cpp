#include "ScriptingUtilityForeignFunctions.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{

/***********************************************************************
LanguagePlugin
***********************************************************************/
			
			LanguagePlugin::LanguagePlugin()
				:ownerInterpretor(0)
				,ownerLinker(0)
			{
			}

			LanguagePlugin::~LanguagePlugin()
			{
			}

			basicil::BasicILInterpretor* LanguagePlugin::GetOwnerInterpretor()
			{
				return ownerInterpretor;
			}

			basicil::BasicILLinker* LanguagePlugin::GetOwnerLinker()
			{
				return ownerLinker;
			}

			bool LanguagePlugin::Install(basicil::BasicILInterpretor* interpretor)
			{
				if(ownerInterpretor || ownerLinker)
				{
					return false;
				}
				else
				{
					ownerInterpretor=interpretor;
					return RegisterForeignFunctions(interpretor->Symbols());
				}
			}

			bool LanguagePlugin::Install(basicil::BasicILLinker* linker)
			{
				if(ownerInterpretor || ownerLinker)
				{
					return false;
				}
				else
				{
					ownerLinker=linker;
					return RegisterForeignFunctions(linker->Symbols());
				}
			}
		}
	}
}