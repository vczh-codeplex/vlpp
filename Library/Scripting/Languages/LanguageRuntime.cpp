#include "LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

/***********************************************************************
LanguageAssembly
***********************************************************************/

		LanguageAssembly::LanguageAssembly(Ptr<basicil::BasicIL> _il)
			:il(_il)
		{
		}

		Ptr<basicil::BasicIL> LanguageAssembly::GetIL()
		{
			return il;
		}

/***********************************************************************
LanguageHost
***********************************************************************/

		LanguageHost::LanguageHost(int stackSize)
		{
			interpretor=new BasicILInterpretor(stackSize);
		}
	}
}