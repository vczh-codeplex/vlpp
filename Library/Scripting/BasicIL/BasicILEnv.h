/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILEnv						：运行时堆栈
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILENV
#define VCZH_SCRIPTING_BASICIL_BASICILENV

#include "..\..\Basic.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
运行时堆栈
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
		}
	}
}

#endif