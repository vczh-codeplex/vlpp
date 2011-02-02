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
#include "..\BasicIL\BasicILRuntimeSymbol.h"

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
��������
***********************************************************************/

			template<typename T>
			struct LightFunctionMaker
			{
			};

			template<typename R>
			struct LightFunctionMaker<R()>
			{
				template<R(f)()>
				static vint Function(void* result, void* argument)
				{
					*(R*)(result)=f();
					return 0;
				}
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
					return sizeof(P1)+sizeof(P2);
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
					return sizeof(P1)+sizeof(P2)+sizeof(P3);
				}
			};

			template<typename P1, typename P2, typename P3, typename P4, typename R>
			struct LightFunctionMaker<R(P1, P2, P3, P4)>
			{
				template<R(f)(P1, P2, P3, P4)>
				static vint Function(void* result, void* argument)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					*(R*)(result)=f(a1, a2, a3, a4);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4);
				}
			};

			template<typename P1, typename P2, typename P3, typename P4, typename P5, typename R>
			struct LightFunctionMaker<R(P1, P2, P3, P4, P5)>
			{
				template<R(f)(P1, P2, P3, P4, P5)>
				static vint Function(void* result, void* argument)
				{
					P1 a1=*(P1*)((char*)argument+0);
					P2 a2=*(P2*)((char*)argument+sizeof(P1));
					P3 a3=*(P3*)((char*)argument+sizeof(P1)+sizeof(P2));
					P4 a4=*(P4*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3));
					P5 a5=*(P5*)((char*)argument+sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4));
					*(R*)(result)=f(a1, a2, a3, a4, a5);
					return sizeof(P1)+sizeof(P2)+sizeof(P3)+sizeof(P4)+sizeof(P5);
				}
			};

/***********************************************************************
������
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
				void Invoke(vl::scripting::basicil::BasicILInterpretor* interpretor, vl::scripting::basicil::BasicILStack* stack, void* result, void* arguments)\
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
�����ռ���ͳһ�ӿ�
***********************************************************************/

			template<typename _GcImpl, typename _Gc, typename _Callback, bool _MultipleThreading>
			class SystemCoreGcPluginBase : public LanguagePlugin
			{
			private:
				collections::List<Ptr<Object>>		gcs;
				_Callback							callback;
			public:

				BEGIN_FOREIGN_FUNCTION(GcCreate, SystemCoreGcPluginBase)
					vint label=reader.NextArgument<vint>();
					plugin->callback=_GcImpl::GcInitCallback(interpretor, stack, label);
					_Gc* gc=new _Gc(&_GcImpl::GcCallback, &plugin->callback);
					plugin->gcs.Add(gc);
					reader.Result<_Gc*>()=gc;
				END_FOREIGN_FUNCTION

				inline static bool GcIsMultipleThreadingSupported(_Gc* gc)
				{
					return _MultipleThreading;
				}
			protected:
				bool RegisterForeignFunctions(vl::scripting::basicil::BasicILRuntimeSymbol* symbol)
				{
					typedef vl::entities::GcMetaSegment GcMetaSegment;
					typedef vl::entities::GcMeta GcMeta;
					typedef vl::entities::GcHandle GcHandle;

					return 
						REGISTER_FOREIGN_FUNCTION(GcCreate) &&
						REGISTER_LIGHT_FUNCTION(GcIsMultipleThreadingSupported, bool(_Gc*), GcIsMultipleThreadingSupported) &&

						REGISTER_LIGHT_FUNCTION(GcCreateHandle, GcHandle*(_Gc*, GcMeta*, vint), _GcImpl::GcCreateHandle) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleMeta, GcMeta*(_Gc*, GcHandle*), _GcImpl::GcGetHandleMeta) &&
						REGISTER_LIGHT_FUNCTION(GcIsValidHandle, bool(_Gc*, GcHandle*), _GcImpl::GcIsValidHandle) &&
						REGISTER_LIGHT_FUNCTION(GcIncreaseHandleRef, bool(_Gc*, GcHandle*), _GcImpl::GcIncreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION(GcDecreaseHandleRef, bool(_Gc*, GcHandle*), _GcImpl::GcDecreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION(GcIncreaseHandlePin, char*(_Gc*, GcHandle*), _GcImpl::GcIncreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION(GcDecreaseHandlePin, bool(_Gc*, GcHandle*), _GcImpl::GcDecreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION(GcDisposeHandle, bool(_Gc*, GcHandle*), _GcImpl::GcDisposeHandle) &&
						REGISTER_LIGHT_FUNCTION(GcIsHandleDisposed, bool(_Gc*, GcHandle*), _GcImpl::GcIsHandleDisposed) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleSize, vint(_Gc*, GcHandle*), _GcImpl::GcGetHandleSize) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleRepeat, vint(_Gc*, GcHandle*), _GcImpl::GcGetHandleRepeat) &&
						REGISTER_LIGHT_FUNCTION(GcReadHandle, bool(_Gc*, GcHandle*, vint, vint, char*), _GcImpl::GcReadHandle) &&
						REGISTER_LIGHT_FUNCTION(GcWriteHandle, bool(_Gc*, GcHandle*, vint, vint, char*),_GcImpl::GcWriteHandle) &&
						REGISTER_LIGHT_FUNCTION(GcCollect, bool(_Gc*), _GcImpl::GcCollect);
				}
			};

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