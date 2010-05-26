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

		LanguageAssembly::LanguageAssembly(stream::IStream& stream)
			:host(0)
			,instructionKey(-1)
		{
			il=new BasicIL;
			il->LoadFromStream(stream);
			basicLanguageMetadata=new BasicLanguageMetadata(this);
		}

		void LanguageAssembly::SaveToStream(stream::IStream& stream)
		{
			il->SaveToStream(stream);
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

		bool LanguageState::PrepareToRun(const BasicDeclarationInfo& function, void* returnPointer)
		{
			if(function.IsFunction())
			{
				BasicTypeInfo returnType=function.GetType().GetElementType();
				bool isVoid=returnType.IsPrimitive() && returnType.GetPrimitive()==BasicTypeRes::void_type;
				stack->ResetBuffer(
					function.GetAddress(),
					function.GetMetadata()->GetMetadataProvider()->GetInstructionKey(),
					(isVoid?0:returnPointer)
					);
				return true;
			}
			else
			{
				return false;
			}
		}

		void LanguageState::PrepareToRun(Ptr<LanguageAssembly> assembly, int instructionIndex, void* returnPointer)
		{
			stack->ResetBuffer(instructionIndex, assembly->GetInstructionKey(), returnPointer);
		}

		basicil::BasicILStack::RunningResult LanguageState::Run()
		{
			return stack->Run();
		}

		basicil::BasicILStack::RunningResult LanguageState::RunInitialization(Ptr<LanguageAssembly> assembly)
		{
			PrepareToRun(assembly, 0, 0);
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