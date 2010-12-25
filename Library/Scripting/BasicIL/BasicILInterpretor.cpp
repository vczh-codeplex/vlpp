#include "BasicILInterpretor.h"
#include "BasicILSymbolResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basicil
		{
			using namespace collections;

/***********************************************************************
BasicILInterpretor
***********************************************************************/

			BasicILInterpretor::BasicILInterpretor(vint _stackSize)
				:stackSize(_stackSize)
				,expander(&symbols)
			{
			}

			BasicILInterpretor::~BasicILInterpretor()
			{
			}

			vint BasicILInterpretor::GetStackSize()
			{
				return stackSize;
			}

			vint BasicILInterpretor::LoadIL(BasicIL* il)
			{
				List<Pair<WString, WString>> linkingSymbols, foreignFunctions;
				vint index=symbols.LoadILSymbol(il, linkingSymbols, foreignFunctions);
				symbols.LinkILSymbol(il, linkingSymbols, foreignFunctions);
				return index;
			}

			BasicILRuntimeSymbol* BasicILInterpretor::Symbols()
			{
				return &symbols;
			}

			BasicILCodeExpander* BasicILInterpretor::Expander()
			{
				return &expander;
			}
		}
	}
}