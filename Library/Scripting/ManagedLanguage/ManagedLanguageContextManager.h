/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECONTEXTMANAGER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECONTEXTMANAGER

#include "ManagedLanguageSymbolManager.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedContextManager : public Object
			{
			public:
				struct PredefinedTypes
				{
					ManagedTypeSymbol*						sint8;
					ManagedTypeSymbol*						sint16;
					ManagedTypeSymbol*						sint32;
					ManagedTypeSymbol*						sint64;
					ManagedTypeSymbol*						uint8;
					ManagedTypeSymbol*						uint16;
					ManagedTypeSymbol*						uint32;
					ManagedTypeSymbol*						uint64;
					ManagedTypeSymbol*						singleType;
					ManagedTypeSymbol*						doubleType;
					ManagedTypeSymbol*						charType;
					ManagedTypeSymbol*						boolType;
					ManagedTypeSymbol*						stringType;
					ManagedTypeSymbol*						typeType;
					ManagedTypeSymbol*						attributeType;
				};

				PredefinedTypes								predefinedTypes;
			public:

				ManagedContextManager();
				~ManagedContextManager();
			};
		}
	}
}

#endif