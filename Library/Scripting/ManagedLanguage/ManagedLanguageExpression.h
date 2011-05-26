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

			class ManagedVoidType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedGenericArgumentType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										name;
			};

			class ManagedReferencedType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										name;
			};

			class ManagedMemberType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							operand;
				WString										member;
			};

			class ManagedInstantiatedGenericType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							elementType;
				collections::List<Ptr<ManagedType>>			argumentTypes;
			};

#define MANAGED_TYPE_TARGETS(F)\
			F(ManagedVoidType)\
			F(ManagedGenericArgumentType)\
			F(ManagedReferencedType)\
			F(ManagedMemberType)\
			F(ManagedInstantiatedGenericType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedType, MANAGED_TYPE_TARGETS)

/***********************************************************************
Extended Types
***********************************************************************/

			class ManagedArrayType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							elementType;
				int											dimensionCount;
			};

			class ManagedFunctionType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							returnType;
				collections::List<Ptr<ManagedType>>			parameterTypes;
			};

			class ManagedEventType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedFunctionType>					functionType;
			};

			class ManagedAutoReferType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_EXTENDED_TYPE_TARGETS(F)\
			F(ManagedArrayType)\
			F(ManagedFunctionType)\
			F(ManagedEventType)\
			F(ManagedAutoReferType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedType, MANAGED_EXTENDED_TYPE_TARGETS)

/***********************************************************************
Basic Expressions
***********************************************************************/

			class ManagedPrimitiveExpression : public ManagedExpression
			{
			};

			class ManagedNullExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedIntegerExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				bool										sign;
				union
				{
					signed __int64							signedInteger;
					unsigned __int64						unsignedInteger;
				}											value;
			};

			class ManagedFloatExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				bool										doublePrecision;
				double										value;
			};

			class ManagedBooleanExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				bool										value;
			};

			class ManagedCharExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				wchar_t										value;
			};

			class ManagedStringExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										value;
			};

			class ManagedReferenceExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										name;
			};

			class ManagedMemberExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						operand;
				WString										member;
			};

			class ManagedInstanciatedExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						operand;
				collections::List<Ptr<ManagedType>>			argumentTypes;
			};

			class ManagedInvokeExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						function;
				collections::List<Ptr<ManagedExpression>>	arguments;
			};

			class ManagedNewExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							objectType;
				collections::List<Ptr<ManagedExpression>>	arguments;
			};

			class ManagedFunctionResultExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedCastingExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						operand;
				Ptr<ManagedType>							type;
			};

			class ManagedThisExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedBaseExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedAssignmentExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

#define MANAGED_EXPRESSION_TARGETS(F)\
			F(ManagedNullExpression)\
			F(ManagedIntegerExpression)\
			F(ManagedFloatExpression)\
			F(ManagedBooleanExpression)\
			F(ManagedCharExpression)\
			F(ManagedStringExpression)\
			F(ManagedReferenceExpression)\
			F(ManagedMemberExpression)\
			F(ManagedInstanciatedExpression)\
			F(ManagedInvokeExpression)\
			F(ManagedNewExpression)\
			F(ManagedFunctionResultExpression)\
			F(ManagedCastingExpression)\
			F(ManagedThisExpression)\
			F(ManagedBaseExpression)\
			F(ManagedAssignmentExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExpression, MANAGED_EXPRESSION_TARGETS)

/***********************************************************************
Extended Expressions
***********************************************************************/

			class ManagedLambdaExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				collections::List<Ptr<ManagedType>>			parameterTypes;
				collections::List<WString>					parameterNames;
				Ptr<ManagedType>							returnType;
				Ptr<ManagedStatement>						body;
			};

			class ManagedChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						condition;
				Ptr<ManagedExpression>						trueExpression;
				Ptr<ManagedExpression>						falseExpression;
			};

			class ManagedNullChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						valueExpression;
				Ptr<ManagedExpression>						candidateExpression;
			};

			class ManagedTypeofExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							type;
			};

#define MANAGED_EXTENDED_EXPRESSION_TARGETS(F)\
			F(ManagedLambdaExpression)\
			F(ManagedChoiceExpression)\
			F(ManagedNullChoiceExpression)\
			F(ManagedTypeofExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedExpression, MANAGED_EXTENDED_EXPRESSION_TARGETS)

/***********************************************************************
Basic Statements
***********************************************************************/

			class ManagedEmptyStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedCompositeStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				collections::List<Ptr<ManagedStatement>>	statements;
			};

			class ManagedExpressionStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						expression;
			};

			class ManagedVariableStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				bool										constant;
				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedExpression>						initializer;
			};

			class ManagedIfStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						condition;
				Ptr<ManagedStatement>						trueStatement;
				Ptr<ManagedStatement>						falseStatement;
			};

			class ManagedWhileStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						beginCondition;
				Ptr<ManagedExpression>						endCondition;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedForStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedStatement>						initializer;
				Ptr<ManagedStatement>						sideEffect;
				Ptr<ManagedExpression>						condition;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedBreakStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedContinueStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedReturnStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION
			};

			class ManagedTryCatchStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedStatement>						tryStatement;
				collections::List<Ptr<ManagedType>>			catchExceptionTypes;
				collections::List<WString>					catchExceptionNames;
				collections::List<Ptr<ManagedStatement>>	catchExceptionHandlers;
				Ptr<ManagedStatement>						finallyStatement;
			};

			class ManagedThrowStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						expression;
			};

#define MANAGED_STATEMENT_TARGETS(F)\
			F(ManagedEmptyStatement)\
			F(ManagedCompositeStatement)\
			F(ManagedExpressionStatement)\
			F(ManagedVariableStatement)\
			F(ManagedIfStatement)\
			F(ManagedWhileStatement)\
			F(ManagedForStatement)\
			F(ManagedBreakStatement)\
			F(ManagedContinueStatement)\
			F(ManagedReturnStatement)\
			F(ManagedTryCatchStatement)\
			F(ManagedThrowStatement)\

			DEFINE_ALGORITHM_INTERFACE(ManagedStatement, MANAGED_STATEMENT_TARGETS)

/***********************************************************************
Extended Statements
***********************************************************************/

			class ManagedUsingStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedExpression>						initialize;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedLockStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						lock;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedSelectStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<ManagedExpression>						expression;
				collections::List<Ptr<ManagedExpression>>	caseConditions;
				collections::List<Ptr<ManagedStatement>>	caseStatements;
				Ptr<ManagedStatement>						defaultStatements;
			};

#define MANAGED_EXTENDED_STATEMENT_TARGETS(F)\
			F(ManagedUsingStatement)\
			F(ManagedLockStatement)\
			F(ManagedSelectStatement)\

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