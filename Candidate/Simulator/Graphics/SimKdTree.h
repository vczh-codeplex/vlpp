#ifndef VCZHSIM_SIMKDTREE
#define VCZHSIM_SIMKDTREE

#include "SimMath.h"
#include "..\..\..\Library\Entity\GeneralObjectPoolEntity.h"

namespace simulator
{
	using namespace vl::entities;
/*********************************************************************************************************
KdTree
*********************************************************************************************************/

	template<typename T>
	struct KdTree
	{
		enum Dim
		{
			Leaf=0,
			X=1,
			Y=2,
			Z=3,
		};

		Dim					dim;
		float				plane;
		union
		{
			struct
			{
				KdTree<T>*	left;
				KdTree<T>*	right;
			};
			T*				value;
		};

		KdTree()
			:dim(Leaf)
			,plane(0)
			,left(0)
			,right(0)
		{
		}
	};

	template<typename T>
	struct KdTreeManager
	{
		GeneralObjectPool	allocator;

		KdTreeManager(int count)
			:allocator(1048576, sizeof(count)*sizeof(KdTree<T>)/1048576+1)
		{
		}

		KdTree<T>* Alloc()
		{
			return (KdTree<T>*)allocator.Alloc(sizeof(KdTree<T>));
		}
	};
}

#endif