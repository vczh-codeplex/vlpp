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

			class ManagedSymbolItem : public Object
			{
				friend class ManagedSymbolItemGroup;
				friend class ManagedSymbolManager;

				typedef collections::Dictionary<WString, ManagedSymbolItemGroup*>			GroupMap;
				typedef collections::IReadonlyDictionary<WString, ManagedSymbolItemGroup*>	IGroupMap;
			public:
				enum ManagedSymbolType
				{
					Global,
					Namespace,
					UsingNamespace,
					TypeRename,
					Class,
					Structure,
					Interface,
					Field,
					Property,
					PropertySetterValue,
					Method,
					Constructor,
					ConverterOperator,
					GenericParameter,
					MethodParameter,
				};
			protected:
				ManagedSymbolManager*		manager;
				ManagedSymbolItemGroup*		parent;
				ManagedTypeSymbol*			associatedType;
				WString						name;
				ManagedSymbolType			symbolType;
				GroupMap					itemGroups;
			public:
				ManagedSymbolItem(ManagedSymbolManager* _manager, ManagedSymbolType _symbolType);
				~ManagedSymbolItem();

				ManagedSymbolManager*		GetManager();
				ManagedSymbolItemGroup*		GetParentGroup();
				ManagedSymbolItem*			GetParentItem();
				const WString&				GetName();
				void						SetName(const WString& value);
				ManagedSymbolType			GetSymbolType();
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

				ManagedSymbolItem*			GetParentItem();
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
				ManagedSymbolItem*			typeSymbol; // GenericParameter, Class, Structure, Interface, TypeRename(temporary)
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

			// GenericParameter
			class ManagedSymbolGenericParameter : public ManagedSymbolItem
			{
			public:
				ManagedSymbolGenericParameter(ManagedSymbolManager* _manager);

				ManagedGenericInfo::ArgumentConversion		conversion;
				bool										newConstraint;
				collections::Array<ManagedTypeSymbol*>		typeConstraints;
			};

			// MethodParameter
			class ManagedSymbolMethodParameter : public ManagedSymbolItem
			{
			public:
				ManagedSymbolMethodParameter(ManagedSymbolManager* _manager);

				ManagedParameter::ParameterType				parameterType;
				bool										containsDefaultValue;
				ManagedTypeSymbol*							type;
			};

/***********************************************************************
Data Members
***********************************************************************/

			// Field
			class ManagedSymbolField : public ManagedSymbolItem
			{
			public:
				ManagedSymbolField(ManagedSymbolManager* _manager);

				ManagedField*								languageElement;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				declatt::DataType							dataType;

				ManagedTypeSymbol*							type;
			};

			// Property {PropertySetterValue}
			class ManagedSymbolProperty : public ManagedSymbolItem
			{
			public:
				ManagedSymbolProperty(ManagedSymbolManager* _manager);

				ManagedProperty*							languageElement;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				declatt::Inheritation						inheritation;

				ManagedTypeSymbol*							type;
				ManagedTypeSymbol*							implementedInterfaceType;
				bool										containsGetter;
				bool										containsSetter;
			};

			// PropertySetterValue
			class ManagedSymbolPropertySetterValue : public ManagedSymbolItem
			{
			public:
				ManagedSymbolPropertySetterValue(ManagedSymbolManager* _manager);

				ManagedSymbolProperty*						associatedProperty;
			};

			// ConverterOperator {GenericParameter}
			class ManagedSymbolConverterOperator : public ManagedSymbolItem
			{
			public:
				ManagedSymbolConverterOperator(ManagedSymbolManager* _manager);

				ManagedConverterOperator*					languageElement;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				declatt::Inheritation						inheritation;

				bool										implicit;
				ManagedTypeSymbol*							targetType;
				collections::Array<WString>					orderedGenericParameterNames;
			};

			// Method {GenericParameter, MethodParameter}
			class ManagedSymbolMethod : public ManagedSymbolItem
			{
			public:
				ManagedSymbolMethod(ManagedSymbolManager* _manager);

				ManagedMethod*								languageElement;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				declatt::Inheritation						inheritation;

				ManagedTypeSymbol*							returnType;
				ManagedTypeSymbol*							implementedInterfaceType;
				collections::Array<WString>					orderedGenericParameterNames;
				collections::Array<WString>					orderedMethodParameterNames;
			};

			// Constructor {MethodParameter}
			class ManagedSymbolConstructor : public ManagedSymbolItem
			{
			public:
				ManagedSymbolConstructor(ManagedSymbolManager* _manager);

				ManagedConstructor*							languageElement;

				declatt::Accessor							accessor;

				bool										implicit;
				collections::Array<WString>					orderedMethodParameterNames;
			};

/***********************************************************************
Declarations
***********************************************************************/

			// Namespace {Namespace, UsingNamespace, TypeRename, Class, Structure, Interface}
			class ManagedSymbolNamespace : public ManagedSymbolItem
			{
			public:
				ManagedSymbolNamespace(ManagedSymbolManager* _manager);

				ManagedNamespaceDeclaration*				languageElement;
			};

			// UsingNamespace
			class ManagedSymbolUsingNamespace : public ManagedSymbolItem
			{
			public:
				ManagedSymbolUsingNamespace(ManagedSymbolManager* _manager);

				ManagedUsingNamespaceDeclaration*			languageElement;

				ManagedSymbolNamespace*						associatedNamespace;
			};

			// TypeRename {GenericParameter}
			class ManagedSymbolTypeRename : public ManagedSymbolItem
			{
			public:
				ManagedSymbolTypeRename(ManagedSymbolManager* _manager);

				ManagedTypeRenameDeclaration*				languageElement;

				ManagedTypeSymbol*							type;
				collections::Array<WString>					orderedGenericParameterNames;
			};

			// Class, Structure, Interface {GenericParameter, TypeRename, Class, Structure, Interface}
			class ManagedSymbolDeclaration : public ManagedSymbolItem
			{
			public:
				ManagedSymbolDeclaration(ManagedSymbolManager* _manager, ManagedSymbolType _symbolType);

				ManagedTypeDeclaration*						languageElement;

				declatt::Accessor							accessor;
				declatt::Inheritation						inheritation;

				collections::Array<ManagedTypeSymbol*>		baseTypes;
				collections::Array<WString>					orderedGenericParameterNames;
				collections::Array<WString>					orderedDataMemberNames;
			};

/***********************************************************************
Global
***********************************************************************/

			// Global {Namespace, UsingNamespace, TypeRename, Class, Structure, Interface}
			class ManagedSymbolGlobal : public ManagedSymbolItem
			{
			public:
				ManagedSymbolGlobal(ManagedSymbolManager* _manager);
			};

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
				ManagedSymbolGlobal*		global;
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