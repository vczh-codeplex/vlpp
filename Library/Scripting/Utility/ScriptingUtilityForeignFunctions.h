/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ScriptingUtility::ForeignFunction

Classes:
	LanguagePlugin								£ºÍâ½Óº¯Êý±¨
***********************************************************************/

#ifndef VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS
#define VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS

#include "..\..\Basic.h"
#include "..\..\Exception.h"
#include "..\..\Pointer.h"
#include "..\..\Threading.h"
#include "..\..\Collections\List.h"

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

/***********************************************************************
LanguagePlugin
***********************************************************************/

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

/***********************************************************************
LanguageArgumentReader
***********************************************************************/

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

/***********************************************************************
LanguagePluginException
***********************************************************************/

			class LanguagePluginException : public Exception
			{
			public:
				LanguagePluginException(const WString& message);
			};

/***********************************************************************
LanguageHandleList
***********************************************************************/

			template<typename T>
			class LanguageHandleList : public Object
			{
			protected:
				collections::List<T*>		handles;
				collections::List<vint>		freeList;
				CriticalSection				cs;
			public:
				LanguageHandleList()
				{
				}

				~LanguageHandleList()
				{
					for(vint i=0;i<handles.Count();i++)
					{
						if(handles[i])
						{
							delete handles[i];
						}
					}
				}

				T* GetHandle(vint index)
				{
					index--;
					if(index>=0 && index<handles.Count())
					{
						return handles[index];
					}
					else
					{
						return 0;
					}
				}

				vint Alloc(T* handle)
				{
					CriticalSection::Scope scope(cs);
					if(freeList.Count()>0)
					{
						vint index=freeList[freeList.Count()-1];
						freeList.RemoveAt(freeList.Count()-1);
						handles[index]=handle;
						return index+1;
					}
					else
					{
						handles.Add(handle);
						return handles.Count();
					}
				}

				bool Free(vint index)
				{
					index--;
					CriticalSection::Scope scope(cs);
					if(index>=0 && index<handles.Count() && handles[index])
					{
						delete handles[index];
						handles[index]=0;
						freeList.Add(index);
						return true;
					}
					else
					{
						return false;
					}
				}
			};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			template<typename T>
			struct LightFunctionMaker
			{
			};

			template<typename P1, typename R>
			struct LightFunctionMaker<R(P1)>
			{
				template<R(f)(P1)>
				static vint Function(void* result, void* argument)
				{
					P1 a1=*(P1*)((char*)argument+0);
					*(R*)(result)=f(a1);
					return sizeof(P1);
				}
			};

			template<typename P1, typename P2, typename R>
			struct LightFunctionMaker<R(P1, P2)>
			{
				template<R(f)(P1, P2)>
				static vint Function(void* result, void* argument)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					*(R*)(result)=f(a1, a2);
					return sizeof(P1);
				}
			};

			template<typename P1, typename P2, typename P3, typename R>
			struct LightFunctionMaker<R(P1, P2, P3)>
			{
				template<R(f)(P1, P2, P3)>
				static vint Function(void* result, void* argument)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					*(R*)(result)=f(a1, a2, a3);
					return sizeof(P1);
				}
			};

/***********************************************************************
¸¨Öúºê
***********************************************************************/

#define BEGIN_FOREIGN_FUNCTION(NAME, PLUGIN)\
			class NAME : public Object, public IBasicILForeignFunction\
			{\
			protected:\
				PLUGIN*			plugin;\
			public:\
				NAME(PLUGIN* _plugin)\
					:plugin(_plugin)\
				{\
				}\
				\
				void Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)\
				{\
					LanguageArgumentReader reader(result, arguments, stack);

#define END_FOREIGN_FUNCTION\
				}\
			};

#define REGISTER_FOREIGN_FUNCTION(NAME)\
			symbol->RegisterForeignFunction(L"SystemCoreForeignFunctions", L#NAME, new NAME(this))

#define REGISTER_LIGHT_FUNCTION(NAME, TYPE, FUNC)\
			symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L#NAME, &LightFunctionMaker<TYPE>::Function<FUNC>)

/***********************************************************************
²å¼þº¯Êý
***********************************************************************/

			extern Ptr<LanguagePlugin>		CreateMemoryManagerPlugin();
			extern Ptr<LanguagePlugin>		CreateUnitTestPlugin(void(*printer)(bool, wchar_t*));
			extern Ptr<LanguagePlugin>		CreateThreadingPlugin();
			extern Ptr<LanguagePlugin>		CreateStdlibPlugin();
		}
	}
}

#endif