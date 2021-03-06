/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILStack						�����ö�ջ
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
���ö�ջ
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
				static const vint								StackDataOffset=0;
				static const vint								ExceptionHandlerOffset=sizeof(void*);
				static const vint								ExceptionObjectOffset=sizeof(void*)+sizeof(BasicILExceptionHandler*);

				BasicILEnv*										env;
				BasicILInterpretor*								interpretor;
				vint											instruction;
				vint											insKey;
				void*											userData;
				volatile bool									needToPause;

			public:
				BasicILStack(BasicILInterpretor* _interpretor);
				~BasicILStack();

				BasicILEnv*										GetEnv();
				BasicILInterpretor*								GetInterpretor();
				vint											GetInstruction();
				vint											GetInsKey();
				void											Reset(vint entryInstruction, vint entryInsKey, vint returnSize);
				void											ResetBuffer(vint entryInstruction, vint entryInsKey, void* returnPointer);
				void											Pause();
				ILException::RunningResult						Run();

				BasicILExceptionHandler*						GetExceptionHandler();
				void											SetExceptionHandler(BasicILExceptionHandler* handler);
				void											InvokeLightFunction(vint index);
				void*											GetUserData();
				void											SetUserData(void* data);
			};
		}
	}
}

#endif