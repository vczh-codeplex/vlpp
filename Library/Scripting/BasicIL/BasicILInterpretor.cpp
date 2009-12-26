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
				:instruction(0)
				,stackBase(_stackSize)
				,stackTop(_stackSize)
				,stackSize(_stackSize)
			{
				stack=new unsigned char[_stackSize];
			}

			BasicILEnv::~BasicILEnv()
			{
				delete[] stack;
			}

			int BasicILEnv::Instruction()const
			{
				return instruction;
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
				return &stack[stackPosition];
			}

			void* BasicILEnv::Reserve(int size)
			{
				stackTop-=size;
				return &stack[stackTop];
			}
		}
	}
}