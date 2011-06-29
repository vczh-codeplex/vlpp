/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER

#include "ManagedLanguageSymbolManager.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedAnalyzerParameter
***********************************************************************/

			class ManagedAnalyzerParameter : public Object
			{
			public:
				ManagedSymbolManager*			symbolManager;
				ManagedSymbolItem*				currentSymbol;

				ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager);
				ManagedAnalyzerParameter(ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol);
			};
			typedef ManagedAnalyzerParameter MAP;

/***********************************************************************
Build Global Scope Pass 1 (Build place holders)
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

/***********************************************************************
Build Global Scope Pass 2 (Fill place holders)
***********************************************************************/

/***********************************************************************
Build Global Scope Pass 3 (Check declarations and function bodies)
***********************************************************************/
		}
	}
}

#endif