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

		LanguageState::LanguageState(Ptr<basicil::BasicILStack> _stack)
			:stack(_stack)
		{
		}

		bool LanguageState::PrepareToRun(const BasicDeclarationInfo& function)
		{
			if(function.IsFunction())
			{
				stack->Reset(
					function.GetAddress(),
					function.GetMetadata()->GetMetadataProvider()->GetInstructionKey(),
					function.GetType().GetElementType().GetSize()
					);
				return true;
			}
			else
			{
				return false;
			}
		}

		basicil::BasicILStack::RunningResult LanguageState::Run()
		{
			return stack->Run();
		}

		int LanguageState::GetForeignFunctionIndex()
		{
			return stack->GetForeignFunctionIndex();
		}

		void* LanguageState::GetForeignFunctionResultStore()
		{
			return stack->GetForeignFunctionResult();
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

		Ptr<LanguageState> LanguageHost::CreateState()
		{
			return new LanguageState(new BasicILStack(interpretor.Obj()));
		}
	}
}