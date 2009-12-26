#include "BasicILInterpretor.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
BasicILEnv
***********************************************************************/

			BasicILEnv::BasicILEnv(int _stackSize)
				:stackBase(_stackSize)
				,stackTop(_stackSize)
				,stackSize(_stackSize)
			{
				stack=new unsigned char[_stackSize];
			}

			BasicILEnv::~BasicILEnv()
			{
				delete[] stack;
			}

			int BasicILEnv::StackBase()const
			{
				return stackBase;
			}

			int BasicILEnv::StackSize()const
			{
				return stackSize;
			}

			int BasicILEnv::StackTop()const
			{
				return stackTop;
			}

			void* BasicILEnv::DereferenceStack(int stackPosition)const
			{
				if(stackPosition<0 || stackPosition>=stackSize)
				{
					return 0;
				}
				else
				{
					return &stack[stackPosition];
				}
			}

			void* BasicILEnv::Reserve(int size)
			{
				stackTop-=size;
				if(stackTop<0 || stackTop>=stackSize)
				{
					return 0;
				}
				else
				{
					return &stack[stackTop];
				}
			}

			void BasicILEnv::Reset()
			{
				stackBase=stackSize;
				stackTop=stackSize;
			}

			void BasicILEnv::SetBase(int stackPosition)
			{
				stackBase=stackPosition;
			}

/***********************************************************************
BasicILInterpretor
***********************************************************************/

			BasicILInterpretor::BasicILInterpretor(int _stackSize, BasicIL* _instructions)
			{
				env=new BasicILEnv(_stackSize);
				instructions=_instructions;
				instruction=-1;
				foreignFunctionIndex=-1;
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
				delete env;
			}

			void BasicILInterpretor::Reset(int entryInstruction, int returnSize)
			{
				// reserve returnSize
				// push returnPointer
				// push returnInstruction
				// push returnStackBase
				void* returnPointer=env->Reserve(returnSize);
				env->Push<void*>(returnPointer);
				env->Push<int>(-1);
				env->Push<int>(0);
				instruction=entryInstruction;
			}

			int BasicILInterpretor::GetForeignFunctionIndex()
			{
				return foreignFunctionIndex;
			}

			BasicILInterpretor::RunningResult BasicILInterpretor::Run()
			{
				return InstructionIndexOutOfRange;
			}
		}
	}
}