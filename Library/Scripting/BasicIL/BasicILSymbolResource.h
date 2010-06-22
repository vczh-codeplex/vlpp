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

			vint										address;
			ResourceString								name;
		};

/***********************************************************************
�����ӷ���
***********************************************************************/

		struct BasicILLinkingRes
		{
			ResourceString								symbolName;
			ResourceString								assemblyName;
		};

/***********************************************************************
���
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