/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicIL

Classes:
  BasicILStack						£ºµ÷ÓÃ¶ÑÕ»
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILSTACK
#define VCZH_SCRIPTING_BASICIL_BASICILSTACK

#include "BasicILEnv.h"
#include "BasicILException.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			class BasicILInterpretor;

/***********************************************************************
µ÷ÓÃ¶ÑÕ»
***********************************************************************/

			struct BasicILExceptionHandler
			{
				vint											instruction;
				vint											key;
				vint											stackBase;
				vint											stackTop;
				BasicILExceptionHandler*						previous;
			};

			class BasicILStack : public Object
			{
			protected:
				static const vint								StackDataSize=sizeof(void*);

				BasicILEnv*										env;
				BasicILInterpretor*								interpretor;
				vint											instruction;
				vint											insKey;
				void*											userData;

			public:
				BasicILStack(BasicILInterpretor* _interpretor);
				~BasicILStack();

				BasicILEnv*										GetEnv();
				void											Reset(vint entryInstruction, vint entryInsKey, vint returnSize);
				void											ResetBuffer(vint entryInstruction, vint entryInsKey, void* returnPointer);
				vint											GetInstruction();
				vint											GetInsKey();
				ILException::RunningResult						Run();

				BasicILExceptionHandler*						GetExceptionHandler();
				void											SetExceptionHandler(BasicILExceptionHandler* handler);
				void											InvokeForeignFunction(vint index);
				void											InvokeLightFunction(vint index);
				void*											GetUserData();
				void											SetUserData(void* data);
			};
		}
	}
}

#endif