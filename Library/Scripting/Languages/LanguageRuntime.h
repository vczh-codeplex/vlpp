/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Language Provider

Classes:
  LanguageAssembly						：程序集
  LanguageState							：程序运行状态
  LanguageHost							：程序运行时
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME

#include "..\BasicIL\BasicILInterpretor.h"
#include "LanguageMetadata.h"
#include "BasicLanguageMetadata.h"

namespace vl
{
	namespace scripting
	{
		class LanguageAssembly;
		class LanguageState;
		class LanguageHost;
		class LanguageArguments;

		class ILanguageForeignFunction : public Interface
		{
		public:
			virtual void								Invoke(LanguageArguments& arguments)=0;
		};

/***********************************************************************
可执行对象
***********************************************************************/

		class LanguageAssembly : public Object, public IMetadataProvider
		{
			friend class LanguageHost;
			typedef collections::IReadonlyDictionary<WString, Ptr<ResourceStream>>	_ResourceMap;
		protected:
			Ptr<basicil::BasicIL>						il;
			LanguageHost*								host;
			vint										instructionKey;
			Ptr<BasicLanguageMetadata>					basicLanguageMetadata;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);
			LanguageAssembly(stream::IStream& stream);

			void										SaveToStream(stream::IStream& stream);
			void										LogInternalState(stream::TextWriter& writer, basicil::BasicIL::ICommentProvider* commentProvider=0);
			LanguageHost*								GetHost();
			const _ResourceMap&							GetResources();
			Ptr<ResourceStream>							GetResource(const WString& name);
			vint										GetInstructionKey();

			BasicLanguageMetadata*						GetBasicLanguageMetadata();
		};

		class LanguageState : public Object
		{
			friend class LanguageHost;
		protected:
			Ptr<basicil::BasicILStack>					stack;

			LanguageState(Ptr<basicil::BasicILStack> _stack);
		public:

			template<typename T>
			void* Push(const T& value)
			{
				stack->GetEnv()->Push<T>(value);
				return stack->GetEnv()->DereferenceStack(stack->GetEnv()->StackTop());
			}

			template<typename T>
			T Pop()
			{
				return stack->GetEnv()->Pop<T>();
			}

			bool										PrepareToRun(const BasicDeclarationInfo& function, void* returnPointer);
			void										PrepareToRun(Ptr<LanguageAssembly> assembly, vint instructionIndex, void* returnPointer);
			basicil::BasicILStack::RunningResult		Run();
			basicil::BasicILStack::RunningResult		RunInitialization(Ptr<LanguageAssembly> assembly);
			basicil::BasicILEnv*						GetStack();
		};

		class LanguageHost : public Object
		{
		protected:
			Ptr<basicil::BasicILInterpretor>			interpretor;
			collections::List<Ptr<LanguageAssembly>>	loadedAssemblies;
		public:
			LanguageHost(vint stackSize);

			bool										LoadAssembly(Ptr<LanguageAssembly> assembly);
			Ptr<LanguageState>							CreateState();
			void										LogInternalState(stream::TextWriter& writer);
			bool										RegisterForeignFunction(const WString& category, const WString& name, Ptr<ILanguageForeignFunction> function);
			bool										RegisterForeignFunction(const WString& category, const WString& name, void(*function)(void*, void*), vint argumentSize);
		};

/***********************************************************************
外部函数参数表
***********************************************************************/

		class LanguageArguments
		{
		protected:
			char*										result;
			char*										arguments;
			char*										currentArgument;
			LanguageHost*								host;
			LanguageState*								state;
		public:
			LanguageArguments(LanguageHost* _host, LanguageState* _state, void* _result, void* _arguments);
			~LanguageArguments();

			template<typename T>
			T NextArgument()
			{
				return NextArgumentRef<T>();
			}

			template<typename T>
			T& NextArgumentRef()
			{
				T& argument=*(T*)currentArgument;
				currentArgument+=sizeof(T);
				return argument;
			}

			template<typename T>
			T& Result()
			{
				return *(T*)result;
			}
		};
	}
}

#endif