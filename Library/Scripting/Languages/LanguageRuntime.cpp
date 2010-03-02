#include "LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

/***********************************************************************
LanguageAssembly
***********************************************************************/

		LanguageAssembly::LanguageAssembly(Ptr<basicil::BasicIL> _il)
			:il(_il)
			,host(0)
		{
		}

		Ptr<basicil::BasicIL> LanguageAssembly::GetIL()
		{
			return il;
		}

		LanguageHost* LanguageAssembly::GetHost()
		{
			return host;
		}

/***********************************************************************
LanguageHost
***********************************************************************/

		LanguageHost::LanguageHost(int stackSize)
		{
			interpretor=new BasicILInterpretor(stackSize);
		}

		bool LanguageHost::LoadAssembly(Ptr<LanguageAssembly> assembly)
		{
			if(assembly->host==this)
			{
				return true;
			}
			if(!loadedAssemblies.Contains(assembly.Obj()))
			{
				interpretor->LoadIL(assembly->il.Obj());
				loadedAssemblies.Add(assembly);
				assembly->host=this;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}