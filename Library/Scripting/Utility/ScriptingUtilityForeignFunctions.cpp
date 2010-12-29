#include "ScriptingUtilityForeignFunctions.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;

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

/***********************************************************************
LanguageArgumentReader
***********************************************************************/

			LanguageArgumentReader::LanguageArgumentReader(void* _result, void* _arguments)
				:result((char*)_result)
				,arguments((char*)_arguments)
				,currentArgument((char*)_arguments)
				,stack(0)
			{
			}

			LanguageArgumentReader::LanguageArgumentReader(void* _result, void* _arguments, BasicILStack* _stack)
				:result((char*)_result)
				,arguments((char*)_arguments)
				,currentArgument((char*)_arguments)
				,stack(_stack)
			{
			}

			LanguageArgumentReader::~LanguageArgumentReader()
			{
				if(stack)
				{
					stack->GetEnv()->Reserve(-(vint)(sizeof(result)+BytesToPop()));
				}
			}

			vint LanguageArgumentReader::BytesToPop()
			{
				return currentArgument-arguments;
			}
		}
	}
}