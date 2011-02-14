/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
ScriptingUtility::ForeignFunction

Classes:
	LanguagePlugin								����Ӻ�����
***********************************************************************/

#ifndef VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS
#define VCZH_SCIPRTING_UTILITY_SCRIPTINGUTILITYFOREIGNFUNCTIONS

#include "..\..\Basic.h"
#include "..\..\Exception.h"
#include "..\..\Pointer.h"
#include "..\..\Threading.h"
#include "..\..\Collections\List.h"
#include "..\..\Entity\GcEntityCommon.h"

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
��������
***********************************************************************/

#define EXTRA_ARGUMENTS_DEFINITIONS vl::scripting::basicil::BasicILInterpretor*, vl::scripting::basicil::BasicILStack*, void*
#define EXTRA_ARGUMENTS vl::scripting::basicil::BasicILInterpretor* interpretor, vl::scripting::basicil::BasicILStack* stack, void* userData
#define EXTRA_PASS interpretor, stack, userData

			template<typename T>
			struct LightFunctionMaker
			{
			};

			template<typename R>
			struct LightFunctionMaker<R()>
			{
				template<R(f)()>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					*(R*)(result)=f();
					return 0;
				}

				template<R(f)(void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					*(R*)(result)=f(userData);
					return 0;
				}

				template<R(f)(EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					*(R*)(result)=f(EXTRA_PASS);
					return 0;
				}
			};

			template<typename P1, typename R>
			struct LightFunctionMaker<R(P1)>
			{
				template<R(f)(P1)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					*(R*)(result)=f(a1);
					return sizeof(P1);
				}
				
				template<R(f)(P1, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					*(R*)(result)=f(a1, userData);
					return sizeof(P1);
				}
				
				template<R(f)(P1, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					*(R*)(result)=f(a1, EXTRA_PASS);
					return sizeof(P1);
				}
			};

			template<typename P1, typename P2, typename R>
			struct LightFunctionMaker<R(P1, P2)>
			{
				template<R(f)(P1, P2)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					*(R*)(result)=f(a1, a2);
					return sizeof(P1)+sizeof(P2);
				}
				
				template<R(f)(P1, P2, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					*(R*)(result)=f(a1, a2, userData);
					return sizeof(P1)+sizeof(P2);
				}
				
				template<R(f)(P1, P2, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					*(R*)(result)=f(a1, a2, EXTRA_PASS);
					return sizeof(P1)+sizeof(P2);
				}
			};

			template<typename P1, typename P2, typename P3, typename R>
			struct LightFunctionMaker<R(P1, P2, P3)>
			{
				template<R(f)(P1, P2, P3)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					*(R*)(result)=f(a1, a2, a3);
					return sizeof(P1)+sizeof(P2)+sizeof(P3);
				}
				
				template<R(f)(P1, P2, P3, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					*(R*)(result)=f(a1, a2, a3, userData);
					return sizeof(P1)+sizeof(P2)+sizeof(P3);
				}
				
				template<R(f)(P1, P2, P3, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					*(R*)(result)=f(a1, a2, a3, EXTRA_PASS);
					return sizeof(P1)+sizeof(P2)+sizeof(P3);
				}
			};

			template<typename P1, typename P2, typename P3, typename P4, typename R>
			struct LightFunctionMaker<R(P1, P2, P3, P4)>
			{
				template<R(f)(P1, P2, P3, P4)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					*(R*)(result)=f(a1, a2, a3, a4);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4);
				}
				
				template<R(f)(P1, P2, P3, P4, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					*(R*)(result)=f(a1, a2, a3, a4, userData);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4);
				}
				
				template<R(f)(P1, P2, P3, P4, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					*(R*)(result)=f(a1, a2, a3, a4, EXTRA_PASS);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4);
				}
			};

			template<typename P1, typename P2, typename P3, typename P4, typename P5, typename R>
			struct LightFunctionMaker<R(P1, P2, P3, P4, P5)>
			{
				template<R(f)(P1, P2, P3, P4, P5)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					*(R*)(result)=f(a1, a2, a3, a4, a5);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5);
				}
				
				template<R(f)(P1, P2, P3, P4, P5, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					*(R*)(result)=f(a1, a2, a3, a4, a5, userData);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5);
				}
				
				template<R(f)(P1, P2, P3, P4, P5, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					*(R*)(result)=f(a1, a2, a3, a4, a5, EXTRA_PASS);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5);
				}
			};

			template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename R>
			struct LightFunctionMaker<R(P1, P2, P3, P4, P5, P6)>
			{
				template<R(f)(P1, P2, P3, P4, P5, P6)>
				static vint Function(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					P6 a6=*(P6*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5));
					*(R*)(result)=f(a1, a2, a3, a4, a5, a6);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5)+sizeof(P6);
				}
				
				template<R(f)(P1, P2, P3, P4, P5, P6, void*)>
				static vint Function2(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					P6 a6=*(P6*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5));
					*(R*)(result)=f(a1, a2, a3, a4, a5, a6, userData);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5)+sizeof(P6);
				}
				
				template<R(f)(P1, P2, P3, P4, P5, P6, EXTRA_ARGUMENTS_DEFINITIONS)>
				static vint Function3(void* result, void* argument, EXTRA_ARGUMENTS)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					P6 a6=*(P6*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5));
					*(R*)(result)=f(a1, a2, a3, a4, a5, a6, EXTRA_PASS);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5)+sizeof(P6);
				}
			};

#undef EXTRA_ARGUMENTS_DEFINITIONS
#undef EXTRA_ARGUMENTS
#undef EXTRA_PASS

/***********************************************************************
������
***********************************************************************/

#define REGISTER_LIGHT_FUNCTION(NAME, TYPE, FUNC)\
			symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L#NAME, BasicILLightFunction(&LightFunctionMaker<TYPE>::Function<FUNC>, this))

#define REGISTER_LIGHT_FUNCTION2(NAME, TYPE, FUNC)\
			symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L#NAME, BasicILLightFunction(&LightFunctionMaker<TYPE>::Function2<FUNC>, this))

#define REGISTER_LIGHT_FUNCTION3(NAME, TYPE, FUNC)\
			symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L#NAME, BasicILLightFunction(&LightFunctionMaker<TYPE>::Function3<FUNC>, this))

#define LANGUAGE_PLUGIN(TYPE) TYPE* plugin=(TYPE*)userData

/***********************************************************************
�������
***********************************************************************/

			extern Ptr<LanguagePlugin>		CreateMemoryManagerPlugin();
			extern Ptr<LanguagePlugin>		CreateUnitTestPlugin(void(*printer)(bool, wchar_t*));
			extern Ptr<LanguagePlugin>		CreateThreadingPlugin();
			extern Ptr<LanguagePlugin>		CreateStdlibPlugin();
			extern Ptr<LanguagePlugin>		CreateGcSingleThreadPlugin();
		}
	}
}

#endif