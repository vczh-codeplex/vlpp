#include "LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;
		using namespace basiclanguage;

/***********************************************************************
LanguageAssembly
***********************************************************************/

		LanguageAssembly::LanguageAssembly(Ptr<basicil::BasicIL> _il)
			:il(_il)
			,host(0)
			,instructionKey(-1)
		{
			basicLanguageMetadata=new BasicLanguageMetadata(this);
			basicILMetadata=new BasicILMetadata(this);
		}

		LanguageAssembly::LanguageAssembly(stream::IStream& stream)
			:host(0)
			,instructionKey(-1)
		{
			il=new BasicIL;
			il->LoadFromStream(stream);
			basicLanguageMetadata=new BasicLanguageMetadata(this);
			basicILMetadata=new BasicILMetadata(this);
		}

		void LanguageAssembly::SaveToStream(stream::IStream& stream)
		{
			il->SaveToStream(stream);
		}

		void LanguageAssembly::LogInternalState(stream::TextWriter& writer, basicil::BasicIL::ICommentProvider* commentProvider)
		{
			il->SaveAsString(writer, commentProvider);
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
			vint index=il->resources.Keys().IndexOf(name);
			return index==-1?0:il->resources.Values()[index];
		}

		vint LanguageAssembly::GetInstructionKey()
		{
			return instructionKey;
		}

		WString LanguageAssembly::GetAssemblyName()
		{
			Ptr<ResourceStream> exportedResource=GetResource(BasicILResourceNames::ExportedSymbols);
			ResourceRecord<BasicILEntryRes> entry=exportedResource->ReadRootRecord<BasicILEntryRes>();
			return exportedResource->ReadString(entry->assemblyName);
		}

		BasicLanguageMetadata* LanguageAssembly::GetBasicLanguageMetadata()
		{
			return basicLanguageMetadata.Obj();
		}

		BasicILMetadata* LanguageAssembly::GetBasicILMetadata()
		{
			return basicILMetadata.Obj();
		}

/***********************************************************************
LanguageHost
***********************************************************************/

		LanguageState::LanguageState(Ptr<basicil::BasicILStack> _stack)
			:stack(_stack)
		{
			stack->SetUserData(this);
		}

		bool LanguageState::PrepareToRun(const BasicDeclarationInfo& function, void* returnPointer)
		{
			if(function.IsFunction() && !function.IsLinkingSymbol())
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

		void LanguageState::PrepareToRun(Ptr<LanguageAssembly> assembly, vint instructionIndex, void* returnPointer)
		{
			stack->ResetBuffer(instructionIndex, assembly->GetInstructionKey(), returnPointer);
		}

		basicil::ILException::RunningResult LanguageState::Run()
		{
			return stack->Run();
		}

		basicil::ILException::RunningResult LanguageState::RunInitialization(Ptr<LanguageAssembly> assembly)
		{
			PrepareToRun(assembly, 0, 0);
			return stack->Run();
		}

		basicil::BasicILEnv* LanguageState::GetStack()
		{
			return stack->GetEnv();
		}

/***********************************************************************
LanguageHost
***********************************************************************/

		vint LanguageLightFunctionCallback(void* result, void* arguments, BasicILInterpretor* interpretor, BasicILStack* stack, void* userData)
		{
			typedef vint(__stdcall*Callback)(void*, void*);
			Callback callback=(Callback)userData;
			return callback(result, arguments);
		}

		vint LanguageLightFunctionCallback2(void* result, void* arguments, BasicILInterpretor* interpretor, BasicILStack* stack, void* userData)
		{
			LanguageHost::Context* context=(LanguageHost::Context*)userData;
			return context->function(result, arguments, context->userData);
		}

/***********************************************************************
LanguageHost
***********************************************************************/

		LanguageHost::LanguageHost(vint stackSize)
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

		bool LanguageHost::RegisterForeignFunction(const WString& category, const WString& name, vint(__stdcall*function)(void*, void*))
		{
			return interpretor->Symbols()->RegisterLightFunction(category, name, BasicILLightFunction(&LanguageLightFunctionCallback, (void*)function));
		}

		bool LanguageHost::RegisterForeignFunction(const WString& category, const WString& name, vint(__stdcall*function)(void*, void*, void*), void* userData)
		{
			Ptr<Context> context=new Context;
			context->function=function;
			context->userData=userData;
			foreignFunctionContexts.Add(context);
			return interpretor->Symbols()->RegisterLightFunction(category, name, BasicILLightFunction(&LanguageLightFunctionCallback2, (void*)context.Obj()));
		}

		bool LanguageHost::RegisterPlugin(Ptr<utility::LanguagePlugin> plugin)
		{
			plugins.Add(plugin);
			return plugin->Install(interpretor.Obj());
		}

		void LanguageHost::LogInternalState(stream::TextWriter& writer)
		{
			interpretor->LogInternalState(writer);
		}

/***********************************************************************
LanguageLinker
***********************************************************************/

		LanguageLinker::LanguageLinker()
		{
			linker=new BasicILLinker();
		}

		void LanguageLinker::LoadAssembly(stream::IStream& stream)
		{
			Ptr<LanguageAssembly> assembly=new LanguageAssembly(stream);
			linker->LoadIL(assembly->il.Obj());
			loadedAssemblies.Add(assembly);
		}

		Ptr<LanguageAssembly> LanguageLinker::Link()
		{
			return new LanguageAssembly(linker->Link());
		}

		bool LanguageLinker::RegisterForeignFunction(const WString& category, const WString& name, vint(__stdcall*function)(void*, void*))
		{
			return linker->Symbols()->RegisterLightFunction(category, name, BasicILLightFunction(&LanguageLightFunctionCallback, (void*)function));
		}

		bool LanguageLinker::RegisterForeignFunction(const WString& category, const WString& name, vint(__stdcall*function)(void*, void*, void*))
		{
			return linker->Symbols()->RegisterLightFunction(category, name, BasicILLightFunction(&LanguageLightFunctionCallback2, 0));
		}

		bool LanguageLinker::RegisterPlugin(Ptr<utility::LanguagePlugin> plugin)
		{
			plugins.Add(plugin);
			return plugin->Install(linker.Obj());
		}

		void LanguageLinker::LogInternalState(stream::TextWriter& writer)
		{
			linker->LogInternalState(writer);
		}
	}
}