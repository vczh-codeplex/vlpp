#include "ManagedLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedAnalyzerParameter
***********************************************************************/

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager)
				:symbolManager(_symbolManager)
				,currentSymbol(_symbolManager->Global())
			{
			}

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol)
				:symbolManager(parameter.symbolManager)
				,currentSymbol(_currentSymbol)
			{
			}
		}
	}
}