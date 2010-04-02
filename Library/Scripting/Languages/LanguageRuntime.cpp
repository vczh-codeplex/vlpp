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
			,instructionKey(-1)
		{
			basicLanguageMetadata=new BasicLanguageMetadata(this);
		}

		LanguageHost* LanguageAssembly::GetHost()
		{
			return host;
		}

		const LanguageAssembly::_ResourceMap& LanguageAssembly::GetResources()
		{
			return il->resources.Wrap();
		}

		Ptr<ResourceStream> LanguageAssembly::GetResource(const WString& name)
		{
			return il->resources[name];
		}

		int LanguageAssembly::GetInstructionKey()
		{
			return instructionKey;
		}

		BasicLanguageMetadata* LanguageAssembly::GetBasicLanguageMetadata()
		{
			return basicLanguageMetadata.Obj();
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
				assembly->instructionKey=interpretor->LoadIL(assembly->il.Obj());
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