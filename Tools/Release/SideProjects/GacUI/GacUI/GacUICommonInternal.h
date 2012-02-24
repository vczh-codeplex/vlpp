/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI

Interfaces:
***********************************************************************/

#ifndef GACUI_COMMON_INTERNAL
#define GACUI_COMMON_INTERNAL

#include "GacUICommon.h"
#include "..\..\..\..\..\Candidate\GUI\GUI\GacVlppReferences.h"

namespace gacui_cpp_1_0
{
	using namespace vl;

	class GacPointerHolderData
	{
	public:
		Ptr<Object>				internalPointerHolder;
	};

	template<typename T>
	class _RptrBuilder
	{
	public:
		static rptr<T> CreateRptr(void* __internal_object_reference_input)
		{
			return rptr<T>(__internal_object_reference_input);
		}
	};

	template<typename T>
	class _SptrBuilder
	{
	public:
		template<typename U>
		static sptr<T> CreateSptr(void* __internal_object_reference_input, const Ptr<U>& pointerHolder)
		{
			Ptr<Object> internalPointerHolder=new ObjectBox<Ptr<U>>(pointerHolder);
			GacPointerHolderData* __internal_pointer_holder_input=new GacPointerHolderData;
			__internal_pointer_holder_input->internalPointerHolder=internalPointerHolder;
			return sptr<T>(__internal_object_reference_input, __internal_pointer_holder_input);
		}
	};
}

#endif