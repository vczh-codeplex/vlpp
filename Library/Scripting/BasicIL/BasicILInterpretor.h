/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:
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
				int						stackBase;
				int						stackSize;
				int						stackTop;
				unsigned char*			stack;
			public:
				BasicILEnv(int _stackSize);
				~BasicILEnv();

				int						StackBase()const;
				int						StackSize()const;
				int						StackTop()const;
				void*					DereferenceStack(int stackPosition)const;
				void*					Reserve(int size);
				void					Reset();
				void					SetBase(int stackPosition);

				template<typename T>
				void Push(const T& value)
				{
					(*(T*)Reserve(sizeof(T)))=value;
				}

				template<typename T>
				T Pop()
				{
					T result=*(T*)DereferenceStack(stackTop);
					Reserve(-sizeof(T));
					return result;
				}
			};

			class BasicILInterpretor : public Object
			{
			protected:
				BasicILEnv*				env;
			public:
				BasicILInterpretor(int stackSize);
				~BasicILInterpretor();
			};
		}
	}
}

#endif