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
				ManagedTypeSymbol*			parentType;
				ManagedTypeSymbol*			genericDeclaration;
				TypeList					genericArguments;
				TypeList					associatedInstantiatedTypes;

				ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedSymbolItem* _typeSymbol, ManagedTypeSymbol* _parentType);
				ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedTypeSymbol* _genericDeclaration);
			public:
				~ManagedTypeSymbol();

				ManagedSymbolManager*		GetManager();
				ManagedSymbolItem*			GetSymbol();				// declaration symbol
				ManagedTypeSymbol*			GetParentType();			// parent type (exists => this type is a member type)
																		//   will not be a type rename
																		//   may contains some generic arguments
																		//   a full "generic declaration -> type" map can be retrived from the full parent type train
				ManagedTypeSymbol*			GetGenericDeclaration();	// generic declaration type (exists => this type is a instantiated type)
				const ITypeList&			GetGenericArguments();		// generic arguments (not empty => this type is a instantiated type)
																		//    generic argument can be another generic parameter
			};
			/*
			================================================
			generic<inout T>
			class A
			{
				generic<inout U>
				public using S = B<T, U>;
			}

			generic<inout T, inout U>
			class B
			{
				public using T = Dictionary<T, U>;
			}
			================================================
			A<int> ->
				TypeSymbol(A<int>)
				{
					GetSymbol() = A
					GetParentType() = 0
					GetGenericDeclaration() = TypeSymbol(A)
					{
						GetSymbol() = A
						GetParentType() = 0
						GetGenericDeclaration() = 0
						GetGenericArguments() = []
					}
					GetGenericArguments() = [int]
				}
			================================================
			A<int>.S<string> ->
				TypeSymbol(A<int>.S<string>)
				{
					GetSymbol() = A.S
					GetParentType() = 0
					GetGenericDeclaration() = TypeSymbol(A<int>.S)
					{
						GetSymbol() = A.S
						GetParentType() = TypeSymbol(A<int>)
						GetGenericDeclaration() = 0
						GetGenericArguments() = []
					}
					GetGenericArguments() = [string]
				}
			================================================
			A<int>.S<string>.T ->
				TypeSymbol(Dictionary<int, string>)
				{
					GetSymbol() = Dictionary
					GetParentType() = 0
					GetGenericDeclaration() = TypeSymbol(Dictionary)
					GetGenericArguments() = [int, string]
				}
			================================================
			*/

/***********************************************************************
Others
***********************************************************************/

			// GenericParameter
			class ManagedSymbolGenericParameter : public ManagedSymbolItem
			{
			public:
				ManagedSymbolGenericParameter(ManagedSymbolManager* _manager);

				ManagedGenericInfo::ArgumentConversion					conversion;
				bool													newConstraint;
				collections::List<ManagedTypeSymbol*>					typeConstraints;
			};

			// MethodParameter
			class ManagedSymbolMethodParameter : public ManagedSymbolItem
			{
			public:
				ManagedSymbolMethodParameter(ManagedSymbolManager* _manager);

				ManagedParameter*										languageElement;

				ManagedParameter::ParameterType							parameterType;
				bool													containsDefaultValue;
				ManagedTypeSymbol*										type;
			};

/***********************************************************************
Data Members
***********************************************************************/

			// Field
			class ManagedSymbolField : public ManagedSymbolItem
			{
			public:
				ManagedSymbolField(ManagedSymbolManager* _manager);

				ManagedField*											typeLanguageElement;
				ManagedEnumItem*										enumerationLanguageElement;

				declatt::Accessor										accessor;
				declatt::MemberType										memberType;
				declatt::DataType										dataType;

				ManagedTypeSymbol*										type;
			};

			// Property {PropertySetterValue}
			class ManagedSymbolProperty : public ManagedSymbolItem
			{
			public:
				ManagedSymbolProperty(ManagedSymbolManager* _manager);

				ManagedProperty*										languageElement;

				declatt::Accessor										accessor;
				declatt::Accessor										setterAccessor;
				declatt::MemberType										memberType;
				declatt::Inheritation									inheritation;

				ManagedTypeSymbol*										type;
				ManagedTypeSymbol*										implementedInterfaceType;
				bool													containsGetter;
				bool													containsSetter;
			};

			// PropertySetterValue
			class ManagedSymbolPropertySetterValue : public ManagedSymbolItem
			{
			public:
				ManagedSymbolPropertySetterValue(ManagedSymbolManager* _manager);

				ManagedSymbolProperty*									associatedProperty;
			};

			// ConverterOperator {GenericParameter}
			class ManagedSymbolConverterOperator : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const SymbolName;
			public:
				ManagedSymbolConverterOperator(ManagedSymbolManager* _manager);

				ManagedConverterOperator*								languageElement;

				declatt::Accessor										accessor;
				declatt::MemberType										memberType;
				declatt::Inheritation									inheritation;

				bool													implicit;
				ManagedTypeSymbol*										targetType;
				collections::List<WString>								orderedGenericParameterNames;
			};

			// Method {GenericParameter, MethodParameter}
			class ManagedSymbolMethod : public ManagedSymbolItem
			{
			public:
				ManagedSymbolMethod(ManagedSymbolManager* _manager);

				ManagedMethod*											languageElement;

				declatt::Accessor										accessor;
				declatt::MemberType										memberType;
				declatt::Inheritation									inheritation;

				ManagedTypeSymbol*										returnType;
				ManagedTypeSymbol*										implementedInterfaceType;
				collections::List<WString>								orderedGenericParameterNames;
				collections::List<WString>								orderedMethodParameterNames;
			};

			// Constructor {MethodParameter}
			class ManagedSymbolConstructor : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolConstructor(ManagedSymbolManager* _manager);

				ManagedConstructor*										languageElement;

				declatt::Accessor										accessor;

				bool													implicit;
				collections::List<WString>								orderedMethodParameterNames;
			};

/***********************************************************************
Declarations
***********************************************************************/

			// Namespace {Namespace, UsingNamespace, TypeRename, Class, Structure, Interface}
			class ManagedSymbolNamespace : public ManagedSymbolItem
			{
			public:
				ManagedSymbolNamespace(ManagedSymbolManager* _manager);

				collections::List<ManagedNamespaceDeclaration*>			languageElements;
			};

			// UsingNamespace
			class ManagedSymbolUsingNamespace : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolUsingNamespace(ManagedSymbolManager* _manager);

				ManagedUsingNamespaceDeclaration*						languageElement;

				ManagedSymbolNamespace*									associatedNamespace;
			};

			// TypeRename {GenericParameter}
			class ManagedSymbolTypeRename : public ManagedSymbolItem
			{
			public:
				ManagedSymbolTypeRename(ManagedSymbolManager* _manager);

				ManagedTypeRenameDeclaration*							languageElement;

				declatt::Accessor										accessor;

				ManagedTypeSymbol*										type;
				collections::List<WString>								orderedGenericParameterNames;
			};

			// Class, Structure, Interface {GenericParameter, TypeRename, Class, Structure, Interface, <Members>}
			class ManagedSymbolDeclaration : public ManagedSymbolItem
			{
			public:
				ManagedSymbolDeclaration(ManagedSymbolManager* _manager, ManagedSymbolType _symbolType);

				ManagedTypeDeclaration*									typeLanguageElement;
				ManagedEnumerationDeclaration*							enumerationLanguageElement;

				declatt::Accessor										accessor;
				declatt::Inheritation									inheritation;

				collections::List<ManagedTypeSymbol*>					baseTypes;
				collections::List<WString>								orderedGenericParameterNames;
				collections::List<WString>								orderedDataMemberNames;
			};

/***********************************************************************
Global
***********************************************************************/

			// Global {Namespace, UsingNamespace, TypeRename, Class, Structure, Interface}
			class ManagedSymbolGlobal : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolGlobal(ManagedSymbolManager* _manager);
			};

/***********************************************************************
ManagedSymbolManager
***********************************************************************/

			class ManagedSymbolManager : public Object
			{
				typedef collections::List<Ptr<ManagedSymbolItem>>								ItemList;
				typedef collections::List<Ptr<ManagedSymbolItemGroup>>							GroupList;
				typedef collections::List<Ptr<ManagedTypeSymbol>>								TypeList;
				typedef collections::Dictionary<ManagedLanguageElement*, ManagedSymbolItem*>	ElementSymbolMap;
			protected:
				ItemList					allocatedItems;
				GroupList					allocatedGroups;
				TypeList					allocatedTypes;
				ElementSymbolMap			elementSymbolMap;
				ManagedSymbolGlobal*		global;
			public:
				ManagedSymbolManager();
				~ManagedSymbolManager();

				void						Register(ManagedSymbolItem* item);
				void						Register(ManagedSymbolItemGroup* group);
				void						Register(ManagedTypeSymbol* typeSymbol);
				ManagedSymbolItem*			Global();
				
				ManagedTypeSymbol*			GetType(ManagedSymbolItem* item, ManagedTypeSymbol* parentType=0);
				ManagedTypeSymbol*			GetInstiantiatedType(ManagedTypeSymbol* genericDeclaration, const collections::IReadonlyList<ManagedTypeSymbol*>& genericArguments);

				void						SetSymbol(ManagedLanguageElement* element, ManagedSymbolItem* symbolItem);
				ManagedSymbolItem*			GetSymbol(ManagedLanguageElement* element);

				template<typename T>
				T* GetTypedSymbol(ManagedLanguageElement* element)
				{
					return dynamic_cast<T*>(GetSymbol(element));
				}
			};
		}
	}
}

#endif