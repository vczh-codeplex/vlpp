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
��ִ�ж���
***********************************************************************/

		///	@Description
		///		[English]A runtime structure that contains executable instructions and metadata
		///		[Chinese]һ�����п�ִ��ָ����Ԫ���ݵ�����ʱ�ṹ
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
			///		[Chinese]��BasicIL�ṹ����LanguageAssembly
			///	@Parameter: _il
			///		[English]a BasicIL object that contains raw executable instructions and raw metadata
			///		[Chinese]һ�������ԭʼ��ִ��ָ����ԭʼԪ���ݵ�BasicIL����
			LanguageAssembly(Ptr<basicil::BasicIL> _il);
			///	@Description
			///		[English]Create a LanguageAssembly from an IStream
			///		[Chinese]��IStream�ṹ����LanguageAssembly
			///	@Parameter: stream
			///		[English]An IStream that contains serialized LanguageAssembly data
			///		[Chinese]һ����������л����LanguageAssembly�����IStream
			LanguageAssembly(stream::IStream& stream);

			///	@Description
			///		[English]Save LanguageAssembly to an IStream
			///		[Chinese]��LanguageAssembly�����IStream
			///	@Parameter: stream
			///		[English]Target IStream
			///		[Chinese]Ŀ��IStream
			void										SaveToStream(stream::IStream& stream);
			///	@Description
			///		[English]Log internal state to a TextWriter
			///		[Chinese]��LanguageAssembly���ڲ�״̬���浽һ��TextWriter��
			///	@Parameter: writer
			///		[English]Target TextWriter
			///		[Chinese]Ŀ��TextWriter
			///	@Parameter: commentProvider
			///		[English]Comment provider for log data
			///		[Chinese]Ϊ�ڲ�״̬�ṩע����Ϣ
			void										LogInternalState(stream::TextWriter& writer, basicil::BasicIL::ICommentProvider* commentProvider=0);
			///	@Description
			///		[English]Get the LanguageHost object that run this LanguageAssembly
			///		[Chinese]������и�LanguageAssembly��LanguageHost����
			/// @Result
			///		[English]The LanguageHost object
			///		[Chinese]���ص�LanguageHost����
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
�ⲿ����������
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