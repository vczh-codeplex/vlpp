/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGESYMBOLMANAGER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGESYMBOLMANAGER

#include "ManagedLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedSymbolItem;
			class ManagedSymbolItemGroup;
			class ManagedTypeSymbol;
			class ManagedSymbolManager;

/***********************************************************************
Basic Constructions
***********************************************************************/

			enum ManagedSymbolType
			{
				Namespace,
				Class,
				Structure,
				Interface,
				Field,
				Property,
				Method,
				Constructor,
				ConverterOperator,
				GenericParameter,
				MethodParameter,
			};

			class ManagedSymbolItem : public Object
			{
				friend class ManagedSymbolItemGroup;
				friend class ManagedSymbolManager;

				typedef collections::Dictionary<WString, ManagedSymbolItemGroup*>			GroupMap;
				typedef collections::IReadonlyDictionary<WString, ManagedSymbolItemGroup*>	IGroupMap;
			protected:
				ManagedSymbolManager*		manager;
				ManagedSymbolItemGroup*		parent;
				ManagedTypeSymbol*			associatedType;
				WString						name;
				GroupMap					itemGroups;
			public:
				ManagedSymbolItem(ManagedSymbolManager* _manager);
				~ManagedSymbolItem();

				ManagedSymbolManager*		GetManager();
				ManagedSymbolItemGroup*		GetParent();
				const WString&				GetName();
				void						SetName(const WString& value);
				const IGroupMap&			ItemGroups();
				ManagedSymbolItemGroup*		ItemGroup(const WString& name);
				void						Add(ManagedSymbolItem* item);
			};

			class ManagedSymbolItemGroup : public Object
			{
				typedef collections::List<ManagedSymbolItem*>								ItemList;
				typedef collections::IReadonlyList<ManagedSymbolItem*>						IItemList;
			protected:
				ManagedSymbolItem*			parent;
				ItemList					items;
			public:
				ManagedSymbolItemGroup(ManagedSymbolItem* _parent);
				~ManagedSymbolItemGroup();

				ManagedSymbolItem*			GetParent();
				const IItemList&			Items();
				void						Add(ManagedSymbolItem* item);
			};

/***********************************************************************
Types
***********************************************************************/

			class ManagedTypeSymbol : public Object
			{
				friend class ManagedSymbolManager;

				typedef collections::List<ManagedTypeSymbol*>								TypeList;
				typedef collections::IReadonlyList<ManagedTypeSymbol*>						ITypeList;
			protected:
				ManagedSymbolManager*		manager;
				ManagedSymbolItem*			typeSymbol;
				TypeList					genericArguments;
				TypeList					associatedInstantiatedTypes;

				ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedSymbolItem* _typeSymbol);
			public:
				~ManagedTypeSymbol();

				ManagedSymbolManager*		GetManager();
				ManagedSymbolItem*			GetSymbol();
				ManagedTypeSymbol*			GetGenericDeclaration();
				const ITypeList&			GetGenericArguments();
			};

/***********************************************************************
Others
***********************************************************************/

/***********************************************************************
Members
***********************************************************************/

/***********************************************************************
Declarations
***********************************************************************/

/***********************************************************************
ManagedSymbolManager
***********************************************************************/

			class ManagedSymbolManager : public Object
			{
				typedef collections::List<Ptr<ManagedSymbolItem>>							ItemList;
				typedef collections::List<Ptr<ManagedSymbolItemGroup>>						GroupList;
				typedef collections::List<Ptr<ManagedTypeSymbol>>							TypeList;
			protected:
				ItemList					allocatedItems;
				GroupList					allocatedGroups;
				TypeList					allocatedTypes;
				ManagedSymbolItem*			global;
			public:
				ManagedSymbolManager();
				~ManagedSymbolManager();

				void						Register(ManagedSymbolItem* item);
				void						Register(ManagedSymbolItemGroup* group);
				void						Register(ManagedTypeSymbol* typeSymbol);
				ManagedSymbolItem*			Global();
				
				ManagedTypeSymbol*			GetType(ManagedSymbolItem* item);
				ManagedTypeSymbol*			GetType(ManagedSymbolItem* item, const collections::IReadonlyList<ManagedTypeSymbol*>& genericArguments);
			};
		}
	}
}

#endif