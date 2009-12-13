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

		TakeProcessor Take(int count)
		{
			return TakeProcessor(count);
		}
		
		SkipProcessor Skip(int count)
		{
			return SkipProcessor(count);
		}

		RepeatProcessor Repeat(int count)
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