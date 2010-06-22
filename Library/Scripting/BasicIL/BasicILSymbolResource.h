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

			vint										address;
			ResourceString								name;
		};

/***********************************************************************
待链接符号
***********************************************************************/

		struct BasicILLinkingRes
		{
			ResourceString								symbolName;
			ResourceString								assemblyName;
		};

/***********************************************************************
入口
***********************************************************************/

		struct BasicILEntryRes
		{
			ResourceString								assemblyName;
			ResourceArrayHandle<BasicILExportRes>		exports;
			ResourceArrayHandle<BasicILLinkingRes>		linkings;
		};
	}
}

#endif