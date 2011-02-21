/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Classes:
  LanguageAssembly						������
  LanguageState							����������״̬
  LanguageHost							����������ʱ
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGERUNTIME

#include "..\BasicIL\BasicILInterpretor.h"
#include "..\Utility\ScriptingUtilityForeignFunctions.h"
#include "LanguageMetadata.h"
#include "BasicILMetadata.h"
#include "BasicLanguageMetadata.h"

namespace vl
{
	namespace scripting
	{
		class LanguageAssembly;
		class LanguageState;
		class LanguageHost;
		class LanguageArguments;

/***********************************************************************
��ִ�ж���
***********************************************************************/

		class LanguageAssembly : public Object, public IMetadataProvider
		{
			friend class LanguageHost;
			friend class LanguageLinker;

			typedef collections::IReadonlyDictionary<WString, Ptr<ResourceStream>>	_ResourceMap;
		protected:
			Ptr<basicil::BasicIL>								il;
			LanguageHost*										host;
			vint												instructionKey;
			Ptr<BasicILMetadata>								basicILMetadata;
			Ptr<BasicLanguageMetadata>							basicLanguageMetadata;

		public:
			LanguageAssembly(Ptr<basicil::BasicIL> _il);
			LanguageAssembly(stream::IStream& stream);

			void												SaveToStream(stream::IStream& stream);
			void												LogInternalState(stream::TextWriter& writer, basicil::BasicIL::ICommentProvider* commentProvider=0);
			LanguageHost*										GetHost();
			const _ResourceMap&									GetResources();
			Ptr<ResourceStream>									GetResource(const WString& name);
			vint												GetInstructionKey();

			WString												GetAssemblyName();
			BasicLanguageMetadata*								GetBasicLanguageMetadata();
			BasicILMetadata*									GetBasicILMetadata();
		};

		class LanguageState : public Object
		{
			friend class LanguageHost;
		protected:
			Ptr<basicil::BasicILStack>							stack;

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

			bool												PrepareToRun(const BasicDeclarationInfo& function, void* returnPointer);
			void												PrepareToRun(Ptr<LanguageAssembly> assembly, vint instructionIndex, void* returnPointer);
			basicil::ILException::RunningResult					Run();
			basicil::ILException::RunningResult					RunInitialization(Ptr<LanguageAssembly> assembly);
			basicil::BasicILEnv*								GetStack();
		};

		class LanguageHost : public Object
		{
		protected:
			Ptr<basicil::BasicILInterpretor>					interpretor;
			collections::List<Ptr<LanguageAssembly>>			loadedAssemblies;
			collections::List<Ptr<utility::LanguagePlugin>>		plugins;
		public:
			LanguageHost(vint stackSize);

			bool												LoadAssembly(Ptr<LanguageAssembly> assembly);
			Ptr<LanguageState>									CreateState();
			bool												RegisterForeignFunction(const WString& category, const WString& name, vint(*function)(void*, void*));
			bool												RegisterPlugin(Ptr<utility::LanguagePlugin> plugin);
			void												LogInternalState(stream::TextWriter& writer);
		};

		class LanguageLinker : public Object
		{
		protected:
			Ptr<basicil::BasicILLinker>							linker;
			collections::List<Ptr<LanguageAssembly>>			loadedAssemblies;
			collections::List<Ptr<utility::LanguagePlugin>>		plugins;
		public:
			LanguageLinker();

			void												LoadAssembly(stream::IStream& stream);
			Ptr<LanguageAssembly>								Link();
			bool												RegisterForeignFunction(const WString& category, const WString& name, vint(*function)(void*, void*));
			bool												RegisterPlugin(Ptr<utility::LanguagePlugin> plugin);
			void												LogInternalState(stream::TextWriter& writer);
		};
	}
}

#endif