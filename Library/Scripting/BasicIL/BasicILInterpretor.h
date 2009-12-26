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
				friend class BasicILInterpretor;
			protected:
				int						instruction;
				int						stackBase;
				int						stackSize;
				int						stackTop;
				unsigned char*			stack;
			public:
				BasicILEnv(int _stackSize);
				~BasicILEnv();

				int						Instruction()const;
				int						StackBase()const;
				int						StackSize()const;
				int						StackTop()const;
				void*					DereferenceStack(int stackPosition)const;
				void*					Reserve(int size);
			};

			class BasicILInterpretor : public Object
			{
			public:
			};
		}
	}
}

#endif