/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  BasicILExportRes				：对外可见符号链表
  BasicILLinkingRes				：待链接符号链表
  BasicILEntryRes				：入口
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE
#define VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE

#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
对外符号
***********************************************************************/

		struct BasicILExportRes
		{
			enum TypeEnum
			{
				Data,
				Instruction,
			};

			int									address;
			ResourceString						name;
			ResourceHandle<BasicILExportRes>	next;
		};

/***********************************************************************
待链接符号
***********************************************************************/

		struct BasicILLinkingRes
		{
			ResourceString						symbolName;
			ResourceString						assemblyName;
			ResourceHandle<BasicILLinkingRes>	next;
		};

/***********************************************************************
入口
***********************************************************************/

		struct BasicILEntryRes
		{
			ResourceString						assemblyName;
			ResourceHandle<BasicILExportRes>	exports;
			ResourceHandle<BasicILLinkingRes>	linkings;
		};
	}
}

#endif