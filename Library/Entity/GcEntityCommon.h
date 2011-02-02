/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Garbage Collector

Classes:
***********************************************************************/

#ifndef VCZH_ENTITY_GCENTITYCOMMON
#define VCZH_ENTITY_GCENTITYCOMMON

#include "..\Basic.h"

namespace vl
{
	namespace entities
	{
		struct GcMetaSegment
		{
			vint					size;
			vint					subHandleCount;
			vint*					subHandles;
		};

		struct GcMeta
		{
			GcMetaSegment			mainSegment;
			GcMetaSegment			repeatSegment;
		};

		struct GcHandle
		{
		};
	}
}

#endif