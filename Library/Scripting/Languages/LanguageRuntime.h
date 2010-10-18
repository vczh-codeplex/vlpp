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

		///	@Description
		///		[English]A runtime structure that contains executable instructions and metadata
		///		[Chinese]一个含有可执行指令与元数据的运行时结构
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
			///	@Description
			///		[English]Create a LanguageAssembly from a BasicIL
			///		[Chinese]从BasicIL结构创建LanguageAssembly
			///	@Parameter: _il
			///		[English]a BasicIL object that contains raw executable instructions and raw metadata
			///		[Chinese]一个存放着原始可执行指令与原始元数据的BasicIL对象
			LanguageAssembly(Ptr<basicil::BasicIL> _il);
			///	@Description
			///		[English]Create a LanguageAssembly from an IStream
			///		[Chinese]从IStream结构创建LanguageAssembly
			///	@Parameter: stream
			///		[English]An IStream that contains serialized LanguageAssembly data
			///		[Chinese]一个存放着序列化后的LanguageAssembly对象的IStream
			LanguageAssembly(stream::IStream& stream);

			///	@Description
			///		[English]Save LanguageAssembly to an IStream
			///		[Chinese]将LanguageAssembly保存成IStream
			///	@Parameter: stream
			///		[English]Target IStream
			///		[Chinese]目标IStream
			void										SaveToStream(stream::IStream& stream);
			///	@Description
			///		[English]Log internal state to a TextWriter
			///		[Chinese]将LanguageAssembly的内部状态保存到一个TextWriter里
			///	@Parameter: writer
			///		[English]Target TextWriter
			///		[Chinese]目标TextWriter
			///	@Parameter: commentProvider
			///		[English]Comment provider for log data
			///		[Chinese]为内部状态提供注释信息
			void										LogInternalState(stream::TextWriter& writer, basicil::BasicIL::ICommentProvider* commentProvider=0);
			///	@Description
			///		[English]Get the LanguageHost object that run this LanguageAssembly
			///		[Chinese]获得运行该LanguageAssembly的LanguageHost对象
			/// @Result
			///		[English]The LanguageHost object
			///		[Chinese]返回的LanguageHost对象
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