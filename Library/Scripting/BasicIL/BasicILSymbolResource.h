/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILExportRes				������ɼ���������
  BasicILLinkingRes				�������ӷ�������
  BasicILEntryRes				�����
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE
#define VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE

#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
�������
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
�����ӷ���
***********************************************************************/

		struct BasicILLinkingRes
		{
			ResourceString						symbolName;
			ResourceString						assemblyName;
			ResourceHandle<BasicILLinkingRes>	next;
		};

/***********************************************************************
���
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