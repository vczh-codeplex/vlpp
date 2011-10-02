/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEANALYZER

#include "ManagedLanguageSymbolManager.h"
#include "ManagedLanguageContextManager.h"
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
				typedef collections::List<ManagedUsingNamespaceDeclaration*>		UsingNamespaceList;
			public:
				ManagedSymbolManager*			symbolManager;
				ManagedContextManager*			contextManager;
				ManagedSymbolItem*				currentSymbol;
				ErrorList&						errors;
				UsingNamespaceList&				availableUsingNamespaceLanguageElements;
				ManagedTypeSymbol*				expectedType;

				ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ManagedContextManager* _contextManager, ErrorList& _errors, UsingNamespaceList& _availableUsingNamespaceLanguageElements);
				ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol, ManagedTypeSymbol* _expectedType=0);
			};
			typedef ManagedAnalyzerParameter MAP;

			extern void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument);

/***********************************************************************
Helper Functions
***********************************************************************/
			
			extern ManagedTypeSymbol*			GetSystemType(ManagedLanguageElement* element, const WString& extraNamespace, const WString& name, const MAP& argument, vint genericParameterCount=0);
			extern ManagedTypeSymbol*			GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument, vint genericParameterCount=0);
			extern ManagedTypeSymbol*			GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument, ManagedSymbolItem* extraGenericParameterContainer=0);
			extern void							EnsureUsingNamespaceSymbolCompleted(ManagedSymbolDeclaration* symbol, const MAP& argument);
			extern void							EnsureSymbolBaseTypesCompleted(ManagedSymbolDeclaration* symbol, const MAP& argument);
			extern void							EnsureTypeVisibilityInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, const MAP& argument);
			extern collections::List<WString>*	GetOrderedGenericParameterNames(ManagedSymbolItem* declSymbol);
			extern ManagedSymbolItem*			GetRealSymbol(ManagedSymbolItem* declSymbol);
			extern ManagedTypeSymbol*			GetRealType(ManagedTypeSymbol* typeRenameType, const MAP& argument);
			extern bool							IsInheritedFrom(ManagedSymbolItem* realSymbol, ManagedSymbolItem* parentSymbol);
			extern bool							IsInheritedFrom(ManagedTypeSymbol* type, ManagedTypeSymbol* parentType, const MAP& argument);
			extern void							EnsureTypeSatisfiesConstraintsInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, const MAP& argument);
			extern void							EnsureTypeVisibilityOutSideOfAssemblyInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* memberItem, const MAP& argument);
			extern void							CheckType(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, ManagedSymbolItem* memberItem, const MAP& argument);
			extern ManagedSymbolItem*			FindScopeItemInMethod(const MAP& argument);
			extern void							CheckTypeInMethod(ManagedLanguageElement* languageElement, ManagedTypeSymbol* typeSymbol, const MAP& argument);
			extern ManagedTypeSymbol*			GetTypeSymbolInMethod(Ptr<ManagedType> type, const MAP& argument);
			extern void							EnsureAbstractTargetsCompleted(ManagedSymbolDeclaration* symbol, const MAP& argument);

			extern void							InitializeContextManager(ManagedProgram* program, const MAP& argument);
			extern void							BuildLocalScope(ManagedStatement* node, const MAP& argument);

			extern bool							CanAssignedToNull(ManagedTypeSymbol* type, const MAP& argument);
			extern bool							CanImplicitlyConvertTo(ManagedTypeSymbol* from, ManagedTypeSymbol* to, const MAP& argument);
			extern bool							IsValue(ManagedSymbolItem* symbol);
			extern ManagedTypeSymbol*			GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument);
			extern bool							IsInStaticMethod(const MAP& argument);
			extern ManagedTypeSymbol*			GetTypeFromInsideScope(ManagedSymbolItem* symbol, const MAP& argument);
			extern ManagedTypeSymbol*			GetThisType(const MAP& argument);
			extern ManagedTypeSymbol*			GetBaseType(ManagedTypeSymbol* type, const MAP& argument);

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
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope3_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

/***********************************************************************
Build Global Scope Pass 4
  Check declarations
  Fill _xxx symbol information
  TODO:
    check overriding relations
	check in/out/inout (function parameter=in, function result=out, others=inout)
	check abstract class/interface method overridding
	check abstract method cannot have an implementation
	check attribute usage
	check field initializer/enum initializer
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope4_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

/***********************************************************************
Build Global Scope Pass 5
  Check statements and expressions
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Member, ManagedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedMember, ManagedExtendedMember, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_Declaration, ManagedDeclaration, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope5_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)
		}
	}
}

#endif