#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedAnalyzerParameter
***********************************************************************/

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ErrorList& _errors)
				:symbolManager(_symbolManager)
				,currentSymbol(_symbolManager->Global())
				,errors(_errors)
			{
			}

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol)
				:symbolManager(parameter.symbolManager)
				,currentSymbol(_currentSymbol)
				,errors(parameter.errors)
			{
			}

/***********************************************************************
ManagedLanguage_AnalyzeProgram
***********************************************************************/

			void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument)
			{
				// build symbol item place holder
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope1_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
				
				// fill symbol item
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope2_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
			}
		}
	}
}