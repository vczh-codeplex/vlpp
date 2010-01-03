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

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			class BasicILEnv : public Object
			{
			protected:
				int								stackBase;
				int								stackSize;
				int								stackTop;
				unsigned char*					stack;
			public:
				BasicILEnv(int _stackSize);
				~BasicILEnv();

				int								StackBase()const;
				int								StackSize()const;
				int								StackTop()const;
				void*							DereferenceStack(int stackPosition)const;
				void*							Reserve(int size);
				void							Reset();
				void							SetBase(int stackPosition);

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

			class BasicILInterpretor : public Object
			{
			protected:
				BasicILEnv*						env;
				BasicIL**						ils;
				int								ilCount;
				int								instruction;
				int								insKey;
				int								foreignFunctionIndex;
				void*							foreignFunctionResult;
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

				BasicILInterpretor(int _stackSize);
				~BasicILInterpretor();

				int								LoadIL(BasicIL* il);
				void							UnloadIL(BasicIL* il);
				void							Reset(int entryInstruction, int entryInsKey, int returnSize);
				int								GetForeignFunctionIndex();
				void*							GetForeignFunctionResult();
				BasicILEnv*						GetEnv();
				int								GetInstruction();
				RunningResult					Run();
			};
		}
	}
}

#endif