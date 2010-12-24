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

			BasicILEnv::BasicILEnv(vint _stackSize)
				:stackBase(_stackSize)
				,stackTop(_stackSize)
				,stackSize(_stackSize)
				,stackReserveTopSize(0)
			{
				stack=new unsigned char[_stackSize];
			}

			BasicILEnv::~BasicILEnv()
			{
				delete[] stack;
			}

			vint BasicILEnv::StackBase()const
			{
				return stackBase;
			}

			vint BasicILEnv::StackSize()const
			{
				return stackSize;
			}

			vint BasicILEnv::StackTop()const
			{
				return stackTop;
			}

			void* BasicILEnv::DereferenceStack(vint stackPosition)const
			{
				if(stackPosition<0 || stackPosition>=stackSize)
				{
					throw ILException(BasicILStack::StackOverflow);
				}
				else
				{
					return &stack[stackPosition];
				}
			}

			void* BasicILEnv::Reserve(vint size)
			{
				vint newStackTop=stackTop-size;
				if(newStackTop<stackReserveTopSize || newStackTop>stackSize)
				{
					throw ILException(BasicILStack::StackOverflow);
				}
				else
				{
					stackTop=newStackTop;
					return &stack[stackTop];
				}
			}

			void BasicILEnv::ReserveTop(vint size)
			{
				if(size<0 || size>stackTop)
				{
					throw ILException(BasicILStack::StackOverflow);
				}
				stackReserveTopSize=size;
			}

			vint BasicILEnv::GetReserveTopSize()
			{
				return stackReserveTopSize;
			}

			void BasicILEnv::Reset()
			{
				stackBase=stackSize;
				stackTop=stackSize;
				stackReserveTopSize=0;
			}

			void BasicILEnv::SetBase(vint stackPosition)
			{
				stackBase=stackPosition;
			}

			void BasicILEnv::SetTop(vint stackPosition)
			{
				stackTop=stackPosition;
			}
		}
	}
}