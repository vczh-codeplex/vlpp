/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILEnv						������ʱ��ջ
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
����ʱ��ջ
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