/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILEnv						：指令执行环境
  BasicILInterpretor				：指令虚拟机
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILDefinition.h"
#include "..\..\Exception.h"
#include "..\..\Entity\Linear.h"
#include "..\BasicIL\BasicILSymbolResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
运行时存储环境
***********************************************************************/

			class BasicILEnv : public Object
			{
			protected:
				vint											stackBase;
				vint											stackSize;
				vint											stackTop;
				unsigned char*									stack;
				vint											stackReserveTopSize;
			public:
				BasicILEnv(vint _stackSize);
				~BasicILEnv();

				vint											StackBase()const;
				vint											StackSize()const;
				vint											StackTop()const;
				void*											DereferenceStack(vint stackPosition)const;
				void*											Reserve(vint size);
				void											ReserveTop(vint size);
				vint											GetReserveTopSize();
				void											Reset();
				void											SetBase(vint stackPosition);
				void											SetTop(vint stackPosition);

				template<typename T>
				void Push(const T& value)
				{
					(*(T*)Reserve(sizeof(T)))=value;
				}

				template<typename T>
				T Pop()
				{
					T result=*(T*)DereferenceStack(stackTop);
					Reserve(-(vint)sizeof(T));
					return result;
				}
			};

/***********************************************************************
模板符号表
***********************************************************************/

			struct BasicILGenericArgument
			{
				vint											size;
				WString											name;
				collections::Array<Ptr<BasicILGenericArgument>>	subArguments;

				bool											operator==(const BasicILGenericArgument& value)const;
				bool											operator!=(const BasicILGenericArgument& value)const;
			};

			//----------------------------------------------------------------------------------------------------

			struct BasicILGenericFunctionEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericFunctionEntry>>	MapType;

				vint											instruction;
				vint											key;
				vint											count;
				vint											argumentCount;
				WString											uniqueEntryID;
			};

			struct BasicILGenericVariableEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericVariableEntry>>	MapType;

				vint											argumentCount;
				Linear<vint, vint>								size;
				WString											uniqueEntryID;
			};

			struct BasicILGenericArgumentEnvironment
			{
				collections::Array<Ptr<BasicILGenericArgument>>	arguments;
			};

			struct BasicILGenericTarget : public BasicILGenericArgumentEnvironment
			{
				typedef collections::List<Ptr<BasicILGenericTarget>> ListType;

				WString											symbolName;
				WString											assemblyName;
			};

			struct BasicILGenericInstanceEntry
			{
				struct Key
				{
					WString										symbolName;
					WString										assemblyName;
					WString										typeUniqueName;

					int Compare(const Key& k)const
					{
						if(symbolName<k.symbolName)return -1;
						if(symbolName>k.symbolName)return 1;
						if(assemblyName<k.assemblyName)return -1;
						if(assemblyName>k.assemblyName)return 1;
						if(typeUniqueName<k.typeUniqueName)return -1;
						if(typeUniqueName>k.typeUniqueName)return 1;
						return 0;
					}

					bool operator<(const Key& k)const
					{
						return Compare(k)<0;
					}

					bool operator>(const Key& k)const
					{
						return Compare(k)>0;
					}

					bool operator==(const Key& k)const
					{
						return Compare(k)==0;
					}
				};

				typedef collections::Dictionary<Key, Ptr<BasicILGenericInstanceEntry>>									MapType;
				typedef collections::Dictionary<WString, vint>															FunctionMap;

				vint											argumentCount;
				vint											instanceIndex;
				WString											assemblyName;
				FunctionMap										functions;
			};

/***********************************************************************
外接函数
***********************************************************************/

			class BasicILInterpretor;
			class BasicILStack;

			class IBasicILForeignFunction : public Interface
			{
			public:
				virtual void									Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)=0;
			};

			typedef void(*BasicILLightFunction)(void* result, void* arguments);

			struct BasicILLightFunctionInfo
			{
				BasicILLightFunction							function;
				vint											argumentSize;

				bool											operator==(const BasicILLightFunctionInfo& info)const;
				bool											operator!=(const BasicILLightFunctionInfo& info)const;
			};

/***********************************************************************
虚拟机
***********************************************************************/

			struct BasicILLabel
			{
				vint											instruction;
				vint											key;

				bool											operator==(const BasicILLabel& label)const;
				bool											operator!=(const BasicILLabel& label)const;
			};

			struct BasicILExceptionHandler
			{
				vint											instruction;
				vint											key;
				vint											stackBase;
				vint											stackTop;
				BasicILExceptionHandler*						previous;
			};

			class BasicILInterpretor : public Object
			{
				friend class BasicILStack;

				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
				typedef collections::Dictionary<WString, vint>								_InstanciatedGenericFunctionMap;
				typedef collections::List<Ptr<IBasicILForeignFunction>>						_ForeignFunctionList;
				typedef collections::List<BasicILLightFunctionInfo>							_LightFunctionList;

				static const vint								GenericFunctionSitingAssemblyKey=0;
				static const vint								ForeignFunctionSitingAssemblyKey=-2;
				static const vint								LightFunctionSitingAssemblyKey=-3;

			public:
				struct VariablePackage
				{
					collections::Array<char>					buffer;
					vint										remainBytes;

					VariablePackage(vint size);
					char*										Allocate(vint size);
				};

				struct VariableManager
				{
					collections::List<Ptr<VariablePackage>>	packages;
					collections::Dictionary<WString, char*>	variables;

					char*										Allocate(const WString& name, vint size);
				};
			protected:
				vint											stackSize;
				collections::List<BasicIL*>						ils;
				_BasicILMap										ilMap;
				collections::List<BasicILLabel>					labels;
				_SymbolMap										symbolMap;

				_SymbolMap										foreignFunctionLabelMap;
				_ForeignFunctionList							foreignFunctionList;
				_LightFunctionList								lightFunctionList;

				BasicILGenericFunctionEntry::MapType			genericFunctionEntries;
				BasicILGenericVariableEntry::MapType			genericVariableEntries;
				BasicILGenericTarget::ListType					genericTargets;
				_SymbolList										genericConcepts;
				BasicILGenericInstanceEntry::MapType			genericInstances;

				_InstanciatedGenericFunctionMap					instanciatedGenericFunctions;
				VariableManager									instanciatedGenericVariables;
				Ptr<BasicIL>									genericFunctionSitingIL;
				
				void											LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
				void											LinkILSymbol(BasicIL* il, vint index, _SymbolList& linkingSymbols, _SymbolList& foreignFunctions);
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, ResourceHandle<BasicILGenericTargetRes> targetRecordHandle);
				vint											RegisterTarget(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex);
				vint											RegisterInstanceFunction(BasicILGenericArgumentEnvironment* environment, BasicIL* il, vint targetIndex, bool& isGenericFunction);
				void											RewriteInstanceFunctionInstruction(BasicILGenericArgumentEnvironment* environment, BasicIns& ins, BasicIL* originIL, BasicIns::OpCode genericOp, BasicIns::OpCode normalOp);
				vint											InstanciateGenericFunction(BasicILGenericTarget* target);
				char*											InstanciateGenericVariable(BasicILGenericTarget* target);
			public:
				BasicILInterpretor(vint _stackSize);
				~BasicILInterpretor();

				vint											LoadIL(BasicIL* il);
				void											UnloadIL(BasicIL* il);
				bool											RegisterForeignFunction(const WString& category, const WString& name, Ptr<IBasicILForeignFunction> function);
				bool											RegisterLightFunction(const WString& category, const WString& name, BasicILLightFunction function, vint argumentSize);
				collections::IList<BasicILLabel>&				GetLabels();
				void											LogInternalState(stream::TextWriter& writer);
			};

/***********************************************************************
调用堆栈
***********************************************************************/

			class BasicILStack : public Object
			{
			protected:
				BasicILEnv*										env;
				BasicILInterpretor*								interpretor;
				vint											instruction;
				vint											insKey;
				void*											userData;

			public:
				enum RunningResult
				{
					Finished,
					StackOverflow,
					DividByZero,
					AccessViolation,
					InstructionIndexOutOfRange,
					UnknownInstruction,
					BadInstructionArgument,
					UnhandledException,
				};

				BasicILStack(BasicILInterpretor* _interpretor);
				~BasicILStack();

				BasicILEnv*										GetEnv();
				void											Reset(vint entryInstruction, vint entryInsKey, vint returnSize);
				void											ResetBuffer(vint entryInstruction, vint entryInsKey, void* returnPointer);
				vint											GetInstruction();
				vint											GetInsKey();
				RunningResult									Run();

				BasicILExceptionHandler*						GetExceptionHandler();
				void											SetExceptionHandler(BasicILExceptionHandler* handler);
				void											InvokeForeignFunction(vint index);
				void											InvokeLightFunction(vint index);
				void*											GetUserData();
				void											SetUserData(void* data);
			};

/***********************************************************************
异常
***********************************************************************/

			class ILException : public Exception
			{
			private:
				static WString				GetExceptionMessage(BasicILStack::RunningResult result);
			public:
				BasicILStack::RunningResult result;

				ILException(BasicILStack::RunningResult _result)
					:Exception(GetExceptionMessage(_result))
					,result(_result)
				{
				}
			};

			class ILLinkerException : public Exception
			{
			public:
				enum ErrorType
				{
					DuplicatedAssemblyName,
					AssemblyNotExists,
					DuplicatedSymbolName,
					SymbolNotExists,
					SymbolNotALabel,
					DuplicatedInstance,
					ForeignFunctionNotExists,
				};
			private:
				static WString				GetExceptionMessage(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName);
			public:
				ErrorType					errorType;
				WString						assemblyName;
				WString						symbolName;

				ILLinkerException(ErrorType _errorType, const WString& _assemblyName, const WString& _symbolName)
					:Exception(GetExceptionMessage(_errorType, _assemblyName, _symbolName))
					,errorType(_errorType)
					,assemblyName(_assemblyName)
					,symbolName(_symbolName)
				{
				}
			};
		}
	}
}

#endif