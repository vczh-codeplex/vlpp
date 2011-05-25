/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXPRESSION
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXPRESSION

#include "..\..\Pointer.h"
#include "..\..\String.h"
#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "..\Common\AlgorithmDeclaration.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			struct ManagedPosition
			{
				vint										start;
				vint										lineStart;
				vint										lineIndex;
				vint										codeIndex;

				ManagedPosition();
			};

			struct ManagedGenericInfo
			{
			};

			struct ManagedAttributeInfo
			{
			};

			class ManagedLanguageElement : public Object, private NotCopyable
			{
			public:
				ManagedPosition								position;
			};

/***********************************************************************
Basic Constructions
***********************************************************************/

			class ManagedType : public ManagedLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedType)
			};

			class ManagedExpression : public ManagedLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedExpression)
			};

			class ManagedStatement : public ManagedLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedStatement)
			};

			class ManagedDeclaration : public ManagedLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedDeclaration)
					
				ManagedAttributeInfo						attributeInfo;
				ManagedGenericInfo							genericInfo;
			};

			class ManagedProgram : public ManagedLanguageElement
			{
			public:
				collections::List<Ptr<ManagedDeclaration>>	declarations;
			};

/***********************************************************************
Extended Constructions
***********************************************************************/

			class ManagedExtendedType : public ManagedType
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedExtendedType)
			};

			class ManagedExtendedExpression : public ManagedExpression
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedExtendedExpression)
			};

			class ManagedExtendedStatement : public ManagedStatement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedExtendedStatement)
			};

			class ManagedExtendedDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedExtendedDeclaration)
			};

/***********************************************************************
Basic Types
***********************************************************************/

			class ManagedGenericArgumentType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedReferencedType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedInstantiatedGenericType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_TYPE_TARGETS(F)\
			F(ManagedGenericArgumentType)\
			F(ManagedReferencedType)\
			F(ManagedInstantiatedGenericType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedType, MANAGED_TYPE_TARGETS)

/***********************************************************************
Extended Types
***********************************************************************/

			class ManagedArrayType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedFunctionType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedEventType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_EXTENDED_TYPE_TARGETS(F)\
			F(ManagedArrayType)\
			F(ManagedFunctionType)\
			F(ManagedEventType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedType, MANAGED_EXTENDED_TYPE_TARGETS)

/***********************************************************************
Basic Expressions
***********************************************************************/

#define MANAGED_EXPRESSION_TARGETS(F)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExpression, MANAGED_EXPRESSION_TARGETS)

/***********************************************************************
Extended Expressions
***********************************************************************/

#define MANAGED_EXTENDED_EXPRESSION_TARGETS(F)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedExpression, MANAGED_EXTENDED_EXPRESSION_TARGETS)

/***********************************************************************
Basic Statements
***********************************************************************/

#define MANAGED_STATEMENT_TARGETS(F)\

			DEFINE_ALGORITHM_INTERFACE(ManagedStatement, MANAGED_STATEMENT_TARGETS)

/***********************************************************************
Extended Statements
***********************************************************************/

#define MANAGED_EXTENDED_STATEMENT_TARGETS(F)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedStatement, MANAGED_EXTENDED_STATEMENT_TARGETS)

/***********************************************************************
Basic Declarations
***********************************************************************/

			class ManagedClassDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedStructureDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedInterfaceDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedNamespaceDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedTypeRenameDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_DECLARATION_TARGETS(F)\
			F(ManagedClassDeclaration)\
			F(ManagedStructureDeclaration)\
			F(ManagedInterfaceDeclaration)\
			F(ManagedNamespaceDeclaration)\
			F(ManagedTypeRenameDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(ManagedDeclaration, MANAGED_DECLARATION_TARGETS)

/***********************************************************************
Extended Declarations
***********************************************************************/

			class ManagedEnumerationDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedDelegateDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedEventDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_EXTENDED_DECLARATION_TARGETS(F)\
			F(ManagedEnumerationDeclaration)\
			F(ManagedDelegateDeclaration)\
			F(ManagedEventDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedDeclaration, MANAGED_EXTENDED_DECLARATION_TARGETS)
		}
	}
}

#endif