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
Helper Functions
***********************************************************************/
			
			extern ManagedTypeSymbol*			GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument, vint genericParameterCount=0);
			extern ManagedTypeSymbol*			GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument, ManagedSymbolItem* extraGenericParameterContainer=0);
			extern void							EnsureUsingNamespaceSymbolCompleted(ManagedSymbolUsingNamespace* symbol, const MAP& argument);
			extern void							EnsureSymbolBaseTypesCompleted(ManagedSymbolDeclaration* symbol, const MAP& argument);
			extern void							EnsureTypeVisibility(ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, const MAP& argument);

/***********************************************************************
Build Global Scope Pass 1 <before linking symbols from other assemblies>
  Build place holders
  Bind symbols and language elements
  Fill declatt data
  Fill ordered*Names
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

/***********************************************************************
Build Global Scope Pass 2 <after linking symbols from other assemblies>
  Fill place holders
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

/***********************************************************************
Build Global Scope Pass 3
  Check types in declarations
  Check declarations and function bodies
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)
		}
	}
}

#endif