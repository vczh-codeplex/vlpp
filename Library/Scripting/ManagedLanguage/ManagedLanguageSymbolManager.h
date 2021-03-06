/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
				typedef collections::Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*>		AssociatedTypeMap;
			public:
				enum ManagedSymbolType
				{
					Global,
					Block,
					Variable,
					Lambda,
					LambdaParameter,

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
				AssociatedTypeMap			associatedTypes;
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

			struct ManagedAbstractItem
			{
				typedef collections::List<ManagedAbstractItem>			ListType;

				ManagedTypeSymbol*										type;
				ManagedSymbolItem*										symbol;

				ManagedAbstractItem();
				~ManagedAbstractItem();

				operator bool()const;

				bool													operator==(const ManagedAbstractItem& value);
				bool													operator!=(const ManagedAbstractItem& value);
			};

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

				ManagedAbstractItem::ListType							_overridedTargets;
			};

			// PropertySetterValue
			class ManagedSymbolPropertySetterValue : public ManagedSymbolItem
			{
			public:
				ManagedSymbolPropertySetterValue(ManagedSymbolManager* _manager);

				ManagedSymbolProperty*									associatedProperty;
			};

			// ConverterOperator {GenericParameter, Block}
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

				ManagedAbstractItem::ListType							_overridedTargets;
			};

			// Method {GenericParameter, MethodParameter, Block}
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

				ManagedAbstractItem::ListType							_overridedTargets;
			};

			// Constructor {MethodParameter, Block}
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

			// Namespace {Namespace, TypeRename, Class, Structure, Interface}
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
				collections::List<ManagedUsingNamespaceDeclaration*>	availableUsingNamespaceLanguageElements;
				WString													foreignAssemblyName;

				declatt::Accessor										accessor;
				declatt::Inheritation									inheritation;
				declatt::MemberType										memberType;

				collections::List<ManagedTypeSymbol*>					baseTypes;
				collections::List<WString>								orderedGenericParameterNames;
				collections::List<WString>								orderedDataMemberNames;

				ManagedTypeSymbol*										_baseType;
				collections::List<ManagedTypeSymbol*>					_baseInterfaces;			// Flatten interface inheritence structure
				ManagedAbstractItem::ListType							_abstractTargets;
				collections::List<ManagedSymbolNamespace*>				_availableUsingNamespaces;	// only for first level type
			};

/***********************************************************************
Others
***********************************************************************/

			// Global {Namespace, UsingNamespace, TypeRename, Class, Structure, Interface}
			class ManagedSymbolGlobal : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolGlobal(ManagedSymbolManager* _manager);
			};

			// Block {Block, Variable, Lambda}
			class ManagedSymbolBlock : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolBlock(ManagedSymbolManager* _manager);

				ManagedStatement*										languageElement;
			};

			// Variable
			class ManagedSymbolVariable : public ManagedSymbolItem
			{
			public:
				ManagedSymbolVariable(ManagedSymbolManager* _manager);

				ManagedVariableStatement*								variableLanguageElement;
				ManagedCatchClause*										catchLanguageElement;
				ManagedUsingStatement*									usingLanguageElement;
				ManagedForEachStatement*								forEachLanguageElement;

				ManagedTypeSymbol*										type;
				bool													constant;
			};

			// Lambda {Block, LambdaParameter}
			class ManagedSymbolLambda : public ManagedSymbolItem
			{
			public:
				static const wchar_t* const								SymbolName;
			public:
				ManagedSymbolLambda(ManagedSymbolManager* _manager);

				ManagedLambdaExpression*								languageElement;

				ManagedTypeSymbol*										returnType;
			};

			// LambdaParameter
			class ManagedSymbolLambdaParameter : public ManagedSymbolItem
			{
			public:
				ManagedSymbolLambdaParameter(ManagedSymbolManager* _manager);

				ManagedLambdaParameter*									languageElement;

				ManagedTypeSymbol*										type;
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
				ManagedTypeSymbol*			ReplaceGenericArguments(ManagedTypeSymbol* type, const collections::IReadonlyDictionary<ManagedTypeSymbol*, ManagedTypeSymbol*>& replacement);
				ManagedTypeSymbol*			ReplaceGenericArguments(ManagedTypeSymbol* type, ManagedTypeSymbol* contextType);
				ManagedTypeSymbol*			GetBaseType(ManagedTypeSymbol* declaredBaseType, ManagedTypeSymbol* instantiatedThisType);

				void						SetSymbol(ManagedLanguageElement* element, ManagedSymbolItem* symbolItem);
				ManagedSymbolItem*			GetSymbol(ManagedLanguageElement* element);
				ManagedTypeSymbol*			GetThisType(ManagedSymbolDeclaration* declaration);

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