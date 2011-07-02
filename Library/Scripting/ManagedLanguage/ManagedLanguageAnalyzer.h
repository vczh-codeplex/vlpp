/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER

#include "ManagedLanguageSymbolManager.h"
#include "ManagedLanguageException.h"

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
				typedef collections::List<Ptr<ManagedLanguageCodeException>>		ErrorList;
			public:
				ManagedSymbolManager*			symbolManager;
				ManagedSymbolItem*				currentSymbol;
				ErrorList&						errors;

				ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ErrorList& _errors);
				ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol);
			};
			typedef ManagedAnalyzerParameter MAP;

			extern void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument);

/***********************************************************************
Build Global Scope Pass 1 (Build place holders, bind symbol and language element)
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