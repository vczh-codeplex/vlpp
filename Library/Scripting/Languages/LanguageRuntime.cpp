#include "LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

/***********************************************************************
LanguageAssembly
***********************************************************************/

		class LanguageForeignFunction : public Object, public IBasicILForeignFunction
		{
		public:
			Ptr<ILanguageForeignFunction>		function;
			LanguageHost*						host;

			void Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)
			{
				LanguageArguments args(host, (LanguageState*)stack->GetUserData(), result, arguments);
				function->Invoke(args);
			}
		};

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
			return il->resources[name];
		}

		vint LanguageAssembly::GetInstructionKey()
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

		basicil::BasicILStack::RunningResult LanguageState::Run()
		{
			return stack->Run();
		}

		basicil::BasicILStack::RunningResult LanguageState::RunInitialization(Ptr<LanguageAssembly> assembly)
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

		void LanguageHost::LogInternalState(stream::TextWriter& writer)
		{
			interpretor->LogInternalState(writer);
		}

		bool LanguageHost::RegisterForeignFunction(const WString& category, const WString& name, Ptr<ILanguageForeignFunction> function)
		{
			Ptr<LanguageForeignFunction> proxy=new LanguageForeignFunction;
			proxy->host=this;
			proxy->function=function;
			return interpretor->RegisterForeignFunction(category, name, proxy);
		}

		bool LanguageHost::RegisterForeignFunction(const WString& category, const WString& name, void(*function)(void*, void*), vint argumentSize)
		{
			return interpretor->RegisterLightFunction(category, name, function, argumentSize);
		}

/***********************************************************************
LanguageAssembly
***********************************************************************/

		LanguageArguments::LanguageArguments(LanguageHost* _host, LanguageState* _state, void* _result, void* _arguments)
			:host(_host)
			,state(_state)
			,result((char*)_result)
			,arguments((char*)_arguments)
			,currentArgument((char*)_arguments)
		{
		}

		LanguageArguments::~LanguageArguments()
		{
			state->GetStack()->Reserve(-(vint)(sizeof(result)+(currentArgument-arguments)));
		}
	}
}