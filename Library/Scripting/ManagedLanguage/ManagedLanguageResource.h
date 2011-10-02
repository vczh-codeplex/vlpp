/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGERESOURCE
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGERESOURCE

#include "..\BasicIL\BasicILResourceStream.h"
#include "ManagedLanguageExpression.h"
#include "ManagedLanguageSymbolManager.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			struct ManagedSymbolItemRes;
			struct ManagedSymbolItemGroupRes;
			struct ManagedTypeSymbolRes;
			struct ManagedStringRes;
			struct ManagedAbstractItemRes;

/***********************************************************************
Basic Constructions
***********************************************************************/

#define COMMON_TYPE_DEFINITION \
			protected: \
				typedef ResourceHandle<ManagedSymbolItemRes>			ItemResource; \
				typedef ResourceHandle<ManagedTypeSymbolRes>			TypeResource; \
				typedef ResourceHandle<ManagedAbstractItemRes>			TargetResource; \
				typedef ResourceArrayHandle<ManagedSymbolItemRes>		ItemArrayResource; \
				typedef ResourceArrayHandle<ManagedTypeSymbolRes>		TypeArrayResource; \
				typedef ResourceArrayHandle<ManagedStringRes>			StringArrayResource; \
				typedef ResourceArrayHandle<ManagedSymbolItemRes>		TargetArrayResource; \
			public:

			struct ManagedSymbolItemRes
			{
			COMMON_TYPE_DEFINITION

				enum SymbolType
				{
					Global,
					Namespace,
					TypeRename,
					TypeDeclaration,
					ExternalDeclaration,

					Field,
					Property,
					PropertySetterValue,
					Method,
					Constructor,
					ConverterOperator,

					GenericParameter,
					MethodParameter,
				};

				SymbolType								symbolType;
				ResourceString							name;
				ItemArrayResource						subItems;
			};

			struct ManagedTypeSymbolRes
			{
			COMMON_TYPE_DEFINITION

				ItemResource							typeSymbol;
				TypeResource							parentType;
				TypeResource							genericDeclaration;
				TypeArrayResource						genericArguments;
			};

			struct ManagedStringRes
			{
				ResourceString							stringValue;
			};

			struct ManagedEntryRes
			{
			COMMON_TYPE_DEFINITION

				ItemResource							globalNamespace;
			};

#undef COMMON_TYPE_DEFINITION

/***********************************************************************
Parameters
***********************************************************************/

			struct ManagedSymbolGenericParameterRes : public ManagedSymbolItemRes
			{
				ManagedGenericInfo::ArgumentConversion	conversion;
				bool									newConstraint;
				TypeArrayResource						typeConstraints;
			};

			struct ManagedSymbolMethodParameterRes : public ManagedSymbolItemRes
			{
				ManagedParameter::ParameterType			parameterType;
				bool									containsDefaultValue;
				TypeResource							type;
			};

/***********************************************************************
Members
***********************************************************************/

			struct ManagedSymbolFieldRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				declatt::MemberType						memberType;
				declatt::DataType						dataType;
				TypeResource							type;
			};

			struct ManagedSymbolPropertyRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				declatt::Accessor						setterAccessor;
				declatt::MemberType						memberType;
				declatt::Inheritation					inheritation;
				TypeResource							type;
				TypeResource							implementedInterfaceType;
				bool									containsGetter;
				bool									containsSetter;
			};

			struct ManagedSymbolPropertySetterValueRes : public ManagedSymbolItemRes
			{
				typedef ResourceHandle<ManagedSymbolPropertyRes>	PropertyResource;

				PropertyResource						associatedProperty;
			};

			struct ManagedSymbolConverterOperatorRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				declatt::MemberType						memberType;
				declatt::Inheritation					inheritation;
				bool									implicit;
				TypeResource							targetType;
				StringArrayResource						orderedGenericParameterNames;
			};

			struct ManagedSymbolMethodRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				declatt::MemberType						memberType;
				declatt::Inheritation					inheritation;
				TypeResource							returnType;
				TypeResource							implementedInterfaceType;
				StringArrayResource						orderedGenericParameterNames;
				StringArrayResource						orderedMethodParameterNames;
			};

			struct ManagedSymbolConstructorRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				bool									implicit;
				StringArrayResource						orderedMethodParameterNames;
			};

/***********************************************************************
Declarations
***********************************************************************/

			struct ManagedSymbolGlobalRes : public ManagedSymbolItemRes
			{
			};

			struct ManagedSymbolNamespaceRes : public ManagedSymbolItemRes
			{
			};

			struct ManagedSymbolTypeRenameRes : public ManagedSymbolItemRes
			{
				declatt::Accessor						accessor;
				TypeResource							type;
				StringArrayResource						orderedGenericParameterNames;
			};

			struct ManagedSymbolDeclarationRes : public ManagedSymbolItemRes
			{
				enum DeclarationType
				{
					Class,
					Structure,
					Interface,
				};

				DeclarationType							declarationType;
				declatt::Accessor						accessor;
				declatt::Inheritation					inheritation;
				declatt::MemberType						memberType;
				TypeArrayResource						baseTypes;
				StringArrayResource						orderedGenericParameterNames;
				StringArrayResource						orderedDataMemberNames;
				TypeResource							_basicType;
				TypeArrayResource						_baseInterfaces;
			};

			struct ManagedSymbolExternalDeclarationRes : public ManagedSymbolItemRes
			{
				ResourceString							foreignAssemblyName;
			};
		}
	}
}

#endif