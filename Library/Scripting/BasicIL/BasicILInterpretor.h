/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILEnv						��ָ��ִ�л���
  BasicILInterpretor				��ָ�������
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILDefinition.h"
#include "..\..\Exception.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
����ʱ����
***********************************************************************/

			class BasicILEnv : public Object
			{
			protected:
				int											stackBase;
				int											stackSize;
				int											stackTop;
				unsigned char*								stack;
			public:
				BasicILEnv(int _stackSize);
				~BasicILEnv();

				int											StackBase()const;
				int											StackSize()const;
				int											StackTop()const;
				void*										DereferenceStack(int stackPosition)const;
				void*										Reserve(int size);
				void										Reset();
				void										SetBase(int stackPosition);

				template<typename T>
				void Push(const T& value)
				{
					(*(T*)Reserve(sizeof(T)))=value;
				}

				template<typename T>
				T Pop()
				{
					T result=*(T*)DereferenceStack(stackTop);
					Reserve(-(int)sizeof(T));
					return result;
				}
			};

			struct BasicILLabel
			{
				int											instruction;
				int											key;

				bool										operator==(const BasicILLabel& label)const;
				bool										operator!=(const BasicILLabel& label)const;
			};

			class BasicILInterpretor : public Object
			{
				friend class BasicILStack;

				typedef collections::Dictionary<collections::Pair<WString, WString>, int>	_SymbolMap;
				typedef collections::List<collections::Pair<WString, WString>>				_SymbolList;
				typedef collections::Dictionary<WString, BasicIL*>							_BasicILMap;
			protected:
				int											stackSize;
				collections::List<BasicIL*>					ils;
				_BasicILMap									ilMap;
				collections::List<BasicILLabel>				labels;
				_SymbolMap									symbolMap;

				void										LoadILSymbol(BasicIL* il, _SymbolList& linkingSymbols);
				void										LinkILSymbol(BasicIL* il, int index, _SymbolList& linkingSymbols);
			public:
				BasicILInterpretor(int _stackSize);
				~BasicILInterpretor();

				int											LoadIL(BasicIL* il);
				void										UnloadIL(BasicIL* il);
				collections::IList<BasicILLabel>&			GetLabels();
			};

/***********************************************************************
����ʱ��ջ
***********************************************************************/

			class BasicILStack : public Object
			{
			protected:
				BasicILEnv*									env;
				BasicILInterpretor*							interpretor;
				int											instruction;
				int											insKey;
				int											foreignFunctionIndex;
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
				void										Reset(int entryInstruction, int entryInsKey, int returnSize);
				void										ResetBuffer(int entryInstruction, int entryInsKey, void* returnPointer);
				int											GetInstruction();
				int											GetForeignFunctionIndex();
				void*										GetForeignFunctionResult();
				RunningResult								Run();
			};

/***********************************************************************
�쳣
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