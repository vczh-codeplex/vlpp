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
				vint										stackBase;
				vint										stackSize;
				vint										stackTop;
				unsigned char*								stack;
			public:
				BasicILEnv(vint _stackSize);
				~BasicILEnv();

				vint										StackBase()const;
				vint										StackSize()const;
				vint										StackTop()const;
				void*										DereferenceStack(vint stackPosition)const;
				void*										Reserve(vint size);
				void										Reset();
				void										SetBase(vint stackPosition);

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

			struct BasicILGenericName
			{
				vint										argumentIndex;
				WString										name;

				BasicILGenericName();
				BasicILGenericName(vint _argumentIndex);
				BasicILGenericName(const WString& _name);

				bool										operator==(const BasicILGenericName& value)const;
				bool										operator!=(const BasicILGenericName& value)const;
			};

			struct BasicILGenericArgument
			{
				vint										size;
				WString										name;

				bool										operator==(const BasicILGenericArgument& value)const;
				bool										operator!=(const BasicILGenericArgument& value)const;
			};

			//----------------------------------------------------------------------------------------------------

			struct BasicILGenericFunctionEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericFunctionEntry>> MapType;

				vint										instruction;
				vint										key;
				vint										count;
				vint										argumentCount;
				collections::Array<BasicILGenericName>		nameTemplate;
			};

			struct BasicILGenericVariableEntry
			{
				typedef collections::Dictionary<collections::Pair<WString, WString>, Ptr<BasicILGenericVariableEntry>> MapType;

				vint										instruction;
				vint										argumentCount;
				Linear<vint, vint>							size;
				collections::Array<BasicILGenericName>		nameTemplate;
			};

			struct BasicILGenericTarget
			{
				typedef collections::List<Ptr<BasicILGenericTarget>> ListType;

				WString										symbolName;
				WString										assemblyName;
				collections::Array<BasicILGenericArgument>	arguments;
			};

/***********************************************************************
虚拟机
***********************************************************************/

			struct BasicILLabel
			{
				vint										instruction;
				vint										key;

				bool										operator==(const BasicILLabel& label)const;
				bool										operator!=(const BasicILLabel& label)const;
			};

			class BasicILInterpretor : public Object
			{
				friend class BasicILStack;

				typedef collections::Dictionary<collections::Pair<WString, WString>, vint>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
				typedef collections::Dictionary<WString, vint>								_InstanciatedGenericFunctionMap;

				static const vint							GenericFunctionSitingAssemblyKey;

			public:
				struct VariablePackage
				{
					collections::Array<char>				buffer;
					vint									remainBytes;

					VariablePackage(vint size);
					char*									Allocate(vint size);
				};

				struct VariableManager
				{
					collections::List<Ptr<VariablePackage>>	packages;
					collections::Dictionary<WString, char*>	variables;

					char*									Allocate(const WString& name, vint size);
				};
			protected:
				vint										stackSize;
				collections::List<BasicIL*>					ils;
				_BasicILMap									ilMap;
				collections::List<BasicILLabel>				labels;
				_SymbolMap									symbolMap;

				BasicILGenericFunctionEntry::MapType		genericFunctionEntries;
				BasicILGenericVariableEntry::MapType		genericVariableEntries;
				BasicILGenericTarget::ListType				genericTargets;

				_InstanciatedGenericFunctionMap				instanciatedGenericFunctions;
				VariableManager								instanciatedGenericVariables;
				Ptr<BasicIL>								genericFunctionSitingIL;
				
				void										LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols);
				void										LinkILSymbol(BasicIL* il, vint index, _SymbolList& linkingSymbols);
				vint										RegisterTarget(BasicILGenericTarget* target, BasicIL* il, vint targetIndex);
				vint										InstanciateGenericFunction(BasicILGenericTarget* target);
				char*										InstanciateGenericVariable(BasicILGenericTarget* target);
			public:
				BasicILInterpretor(vint _stackSize);
				~BasicILInterpretor();

				vint										LoadIL(BasicIL* il);
				void										UnloadIL(BasicIL* il);
				collections::IList<BasicILLabel>&			GetLabels();
			};

/***********************************************************************
调用堆栈
***********************************************************************/

			class BasicILStack : public Object
			{
			protected:
				BasicILEnv*									env;
				BasicILInterpretor*							interpretor;
				vint										instruction;
				vint										insKey;
				vint										foreignFunctionIndex;
				void*										foreignFunctionResult;

			public:
				enum RunningResult
				{
					Finished,
					ForeignFunctionCall,
					StackOverflow,
					DividByZero,
					AccessViolation,
					InstructionIndexOutOfRange,
					UnknownInstruction,
					BadInstructionArgument,
				};

				BasicILStack(BasicILInterpretor* _interpretor);
				~BasicILStack();

				BasicILEnv*									GetEnv();
				void										Reset(vint entryInstruction, vint entryInsKey, vint returnSize);
				void										ResetBuffer(vint entryInstruction, vint entryInsKey, void* returnPointer);
				vint										GetInstruction();
				vint										GetForeignFunctionIndex();
				void*										GetForeignFunctionResult();
				RunningResult								Run();
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