/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
ScriptingUtility::ForeignFunction

Classes:
	LanguagePlugin								：外接函数报
***********************************************************************/

#ifndef VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS
#define VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS

#include "..\..\Basic.h"
#include "..\..\Exception.h"
#include "..\..\Pointer.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			class BasicILInterpretor;
			class BasicILLinker;
			class BasicILStack;
			class BasicILRuntimeSymbol;
		}

		namespace utility
		{
			class LanguagePlugin : public Object
			{
			protected:
				basicil::BasicILInterpretor*	ownerInterpretor;
				basicil::BasicILLinker*			ownerLinker;

				virtual bool					RegisterForeignFunctions(basicil::BasicILRuntimeSymbol* symbol)=0;
			public:
				LanguagePlugin();
				~LanguagePlugin();

				basicil::BasicILInterpretor*	GetOwnerInterpretor();
				basicil::BasicILLinker*			GetOwnerLinker();
				bool							Install(basicil::BasicILInterpretor* interpretor);
				bool							Install(basicil::BasicILLinker* linker);
			};

			class LanguageArgumentReader : public Object
			{
			protected:
				char*							result;
				char*							arguments;
				char*							currentArgument;
				basicil::BasicILStack*			stack;
			public:
				LanguageArgumentReader(void* _result, void* _arguments);
				LanguageArgumentReader(void* _result, void* _arguments, basicil::BasicILStack* stack);
				~LanguageArgumentReader();

				vint							BytesToPop();

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

			class LanguagePluginException : public Exception
			{
			public:
				LanguagePluginException(const WString& message);
			};

			extern Ptr<LanguagePlugin>		CreateMemoryManagerPlugin();
			extern Ptr<LanguagePlugin>		CreateUnitTestPlugin(void(*printer)(bool, wchar_t*));
		}
	}
}

#endif