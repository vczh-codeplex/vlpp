/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILInterpretor				：虚拟机运行时环境
  BasicILLinker						：链接器
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR
#define VCZH_SCRIPTING_BASICIL_BASICILINTERPRETOR

#include "BasicILEnv.h"
#include "BasicILException.h"
#include "BasicILStack.h"
#include "BasicILRuntimeSymbol.h"
#include "BasicILCodeExpander.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{

/***********************************************************************
虚拟机
***********************************************************************/

			class BasicILInterpretor : public Object
			{
			protected:
				vint											stackSize;
				BasicILRuntimeSymbol							symbols;
				BasicILCodeExpander								expander;
			public:
				BasicILInterpretor(vint _stackSize);
				~BasicILInterpretor();

				vint											GetStackSize();
				vint											LoadIL(BasicIL* il);
				BasicILRuntimeSymbol*							Symbols();
				BasicILCodeExpander*							Expander();
				void											LogInternalState(stream::TextWriter& writer);
			};

/***********************************************************************
链接器
***********************************************************************/

			class BasicILLinker : public Object
			{
			protected:
				BasicILRuntimeSymbol							symbols;
				BasicILCodeExpander								expander;

				void											ExpandIns(BasicIL* il, vint index);
				void											ExpandLoadedIL(BasicIL* il);
				void											ExpandSitingIL(BasicIL* il);
				void											ExpandAll();
			public:
				BasicILLinker();
				~BasicILLinker();

				void											LoadIL(BasicIL* il);
				Ptr<BasicIL>									Link();
				BasicILRuntimeSymbol*							Symbols();
				void											LogInternalState(stream::TextWriter& writer);
			};
		}
	}
}

#endif