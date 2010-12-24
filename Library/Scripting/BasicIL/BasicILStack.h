/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILEnv						：指令执行环境
  BasicILInterpretor				：指令虚拟机
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
调用堆栈
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