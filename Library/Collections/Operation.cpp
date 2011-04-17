#include "Operation.h"

namespace vl
{
	namespace collections
	{
		MaxProcessor Max()
		{
			return MaxProcessor();
		}
		
		MinProcessor Min()
		{
			return MinProcessor();
		}

		FirstProcessor First()
		{
			return FirstProcessor();
		}

		CountProcessor Count()
		{
			return CountProcessor();
		}

		IsEmptyProcessor IsEmpty()
		{
			return IsEmptyProcessor();
		}

		TakeProcessor Take(vint count)
		{
			return TakeProcessor(count);
		}
		
		SkipProcessor Skip(vint count)
		{
			return SkipProcessor(count);
		}

		RepeatProcessor Repeat(vint count)
		{
			return RepeatProcessor(count);
		}

		DistinctProcessor Distinct()
		{
			return DistinctProcessor();
		}

		ReverseProcessor Reverse()
		{
			return ReverseProcessor();
		}
	}
}