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
		class LanguageHost;

		class LanguageAssembly : public Object, public IMetadataProvider
		{
			friend class LanguageHost;
			typedef collections::IReadonlyDictionary<WString, Ptr<ResourceStream>>	_ResourceMap;
		protected:
			Ptr<basicil::BasicIL>						il;
			LanguageHost*								host;
			int											instructionKey;
			Ptr<BasicLanguageMetadata>					basicLanguageMetadata;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);

			LanguageHost*								GetHost();
			const _ResourceMap&							GetResources();
			Ptr<ResourceStream>							GetResource(const WString& name);
			int											GetInstructionKey();

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
			basicil::BasicILStack::RunningResult		Run();
			int											GetForeignFunctionIndex();
			void*										GetForeignFunctionResultStore();
		};

		class LanguageHost : public Object
		{
		protected:
			Ptr<basicil::BasicILInterpretor>			interpretor;
			collections::List<Ptr<LanguageAssembly>>	loadedAssemblies;
		public:
			LanguageHost(int stackSize);

			bool										LoadAssembly(Ptr<LanguageAssembly> assembly);
			Ptr<LanguageState>							CreateState();
		};
	}
}

#endif