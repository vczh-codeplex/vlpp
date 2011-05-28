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
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				ALGORITHM_TARGET_ROOT(ManagedExtendedType)
			};

			class ManagedExtendedExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				ALGORITHM_TARGET_ROOT(ManagedExtendedExpression)
			};

			class ManagedExtendedStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				ALGORITHM_TARGET_ROOT(ManagedExtendedStatement)
			};

			class ManagedExtendedDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedDeclaration)

				ALGORITHM_TARGET_ROOT(ManagedExtendedDeclaration)
			};

/***********************************************************************
Basic Types
***********************************************************************/

			class ManagedGenericArgumentType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				WString										name;
			};

			class ManagedReferencedType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				WString										name;
			};

			class ManagedMemberType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				Ptr<ManagedType>							operand;
				WString										member;
			};

			class ManagedInstantiatedGenericType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				Ptr<ManagedType>							elementType;
				collections::List<Ptr<ManagedType>>			argumentTypes;
			};

#define MANAGED_TYPE_TARGETS(P, F)\
			F(P, ManagedGenericArgumentType)\
			F(P, ManagedReferencedType)\
			F(P, ManagedMemberType)\
			F(P, ManagedInstantiatedGenericType)\
			F(P, ManagedExtendedType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedType, MANAGED_TYPE_TARGETS)

/***********************************************************************
Extended Types
***********************************************************************/

			class ManagedArrayType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)

				Ptr<ManagedType>							elementType;
				int											dimensionCount;
			};

			class ManagedFunctionType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)

				Ptr<ManagedType>							returnType;
				collections::List<Ptr<ManagedType>>			parameterTypes;
			};

			class ManagedEventType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)

				Ptr<ManagedType>							functionType;
			};

			class ManagedAutoReferType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)
			};

#define MANAGED_EXTENDED_TYPE_TARGETS(P, F)\
			F(P, ManagedArrayType)\
			F(P, ManagedFunctionType)\
			F(P, ManagedEventType)\
			F(P, ManagedAutoReferType)\

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
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
			};

			class ManagedIntegerExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

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
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				bool										doublePrecision;
				double										value;
			};

			class ManagedBooleanExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				bool										value;
			};

			class ManagedCharExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				wchar_t										value;
			};

			class ManagedStringExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				WString										value;
			};

			class ManagedReferenceExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				WString										name;
			};

			class ManagedMemberExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>						operand;
				WString										member;
			};

			class ManagedInstanciatedExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>						operand;
				collections::List<Ptr<ManagedType>>			argumentTypes;
			};

			class ManagedInvokeExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>						function;
				collections::List<Ptr<ManagedExpression>>	arguments;
				collections::List<WString>					defaultParameterNames;
				collections::List<Ptr<ManagedExpression>>	defaultParameterValues;
			};

			class ManagedNewExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedType>							objectType;
				collections::List<Ptr<ManagedExpression>>	arguments;
				collections::List<WString>					defaultParameterNames;
				collections::List<Ptr<ManagedExpression>>	defaultParameterValues;
				collections::List<WString>					propertyNames;
				collections::List<Ptr<ManagedExpression>>	propertyValues;
			};

			class ManagedFunctionResultExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
			};

			class ManagedCastingExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>						operand;
				Ptr<ManagedType>							type;
			};

			class ManagedThisExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
			};

			class ManagedBaseExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
			};

			class ManagedAssignmentExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
			};

#define MANAGED_EXPRESSION_TARGETS(P, F)\
			F(P, ManagedNullExpression)\
			F(P, ManagedIntegerExpression)\
			F(P, ManagedFloatExpression)\
			F(P, ManagedBooleanExpression)\
			F(P, ManagedCharExpression)\
			F(P, ManagedStringExpression)\
			F(P, ManagedReferenceExpression)\
			F(P, ManagedMemberExpression)\
			F(P, ManagedInstanciatedExpression)\
			F(P, ManagedInvokeExpression)\
			F(P, ManagedNewExpression)\
			F(P, ManagedFunctionResultExpression)\
			F(P, ManagedCastingExpression)\
			F(P, ManagedThisExpression)\
			F(P, ManagedBaseExpression)\
			F(P, ManagedAssignmentExpression)\
			F(P, ManagedExtendedExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExpression, MANAGED_EXPRESSION_TARGETS)

/***********************************************************************
Extended Expressions
***********************************************************************/

			class ManagedLambdaExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				collections::List<Ptr<ManagedType>>			parameterTypes;
				collections::List<WString>					parameterNames;
				Ptr<ManagedType>							returnType;
				Ptr<ManagedStatement>						body;
			};

			class ManagedChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>						condition;
				Ptr<ManagedExpression>						trueExpression;
				Ptr<ManagedExpression>						falseExpression;
			};

			class ManagedNullChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>						valueExpression;
				Ptr<ManagedExpression>						candidateExpression;
			};

			class ManagedTypeofExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedType>							type;
			};

#define MANAGED_EXTENDED_EXPRESSION_TARGETS(P, F)\
			F(P, ManagedLambdaExpression)\
			F(P, ManagedChoiceExpression)\
			F(P, ManagedNullChoiceExpression)\
			F(P, ManagedTypeofExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedExpression, MANAGED_EXTENDED_EXPRESSION_TARGETS)

/***********************************************************************
Basic Statements
***********************************************************************/

			class ManagedEmptyStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)
			};

			class ManagedCompositeStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				collections::List<Ptr<ManagedStatement>>	statements;
			};

			class ManagedExpressionStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>						expression;
			};

			class ManagedVariableStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				bool										constant;
				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedExpression>						initializer;
			};

			class ManagedIfStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>						condition;
				Ptr<ManagedStatement>						trueStatement;
				Ptr<ManagedStatement>						falseStatement;
			};

			class ManagedWhileStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>						beginCondition;
				Ptr<ManagedExpression>						endCondition;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedForStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedStatement>						initializer;
				Ptr<ManagedStatement>						sideEffect;
				Ptr<ManagedExpression>						condition;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedBreakStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)
			};

			class ManagedContinueStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)
			};

			class ManagedReturnStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)
			};

			class ManagedTryCatchStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedStatement>						tryStatement;
				collections::List<Ptr<ManagedType>>			catchExceptionTypes;
				collections::List<WString>					catchExceptionNames;
				collections::List<Ptr<ManagedStatement>>	catchExceptionHandlers;
				Ptr<ManagedStatement>						finallyStatement;
			};

			class ManagedThrowStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>						expression;
			};

#define MANAGED_STATEMENT_TARGETS(P, F)\
			F(P, ManagedEmptyStatement)\
			F(P, ManagedCompositeStatement)\
			F(P, ManagedExpressionStatement)\
			F(P, ManagedVariableStatement)\
			F(P, ManagedIfStatement)\
			F(P, ManagedWhileStatement)\
			F(P, ManagedForStatement)\
			F(P, ManagedBreakStatement)\
			F(P, ManagedContinueStatement)\
			F(P, ManagedReturnStatement)\
			F(P, ManagedTryCatchStatement)\
			F(P, ManagedThrowStatement)\
			F(P, ManagedExtendedStatement)

			DEFINE_ALGORITHM_INTERFACE(ManagedStatement, MANAGED_STATEMENT_TARGETS)

/***********************************************************************
Extended Statements
***********************************************************************/

			class ManagedUsingStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedExpression>						initialize;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedLockStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedExpression>						lock;
				Ptr<ManagedStatement>						statement;
			};

			class ManagedSelectStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedExpression>						expression;
				collections::List<Ptr<ManagedExpression>>	caseConditions;
				collections::List<Ptr<ManagedStatement>>	caseStatements;
				Ptr<ManagedStatement>						defaultStatements;
			};

#define MANAGED_EXTENDED_STATEMENT_TARGETS(P, F)\
			F(P, ManagedUsingStatement)\
			F(P, ManagedLockStatement)\
			F(P, ManagedSelectStatement)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedStatement, MANAGED_EXTENDED_STATEMENT_TARGETS)

/***********************************************************************
Basic Declaration Fragments
***********************************************************************/

			struct ManagedAttributeInfo
			{
			public:
				collections::List<Ptr<ManagedNewExpression>>	attributes;
			};

			struct ManagedGenericInfo
			{
			public:
				struct Argument
				{
					WString									name;
					collections::List<Ptr<ManagedType>>		typeConstraints;
					bool									newConstraint;
				};

				collections::List<Ptr<Argument>>			arguments;
			};

			namespace declatt
			{
				enum Accessor
				{
					Public,
					Protected,
					Private,
					ProtectedInternal,
					Internal,
				};

				enum Inheritation
				{
					Normal,
					Sealed,
					Abstract,
					Virtual,
					Override,
				};

				enum MemberType
				{
					Instance,
					Static,
				};

				enum DataType
				{
					Variable,
					Constant,
					Readonly,
				};
			};

			class ManagedMember : public ManagedLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(ManagedMember)
			};

			class ManagedField : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedExpression>						initializer;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				declatt::DataType							dataType;
				ManagedAttributeInfo						attributeInfo;
			};

			class ManagedProperty : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				Ptr<ManagedType>							type;
				WString										name;
				Ptr<ManagedStatement>						getter;
				Ptr<ManagedStatement>						setter;

				declatt::Accessor							getterAccessor;
				declatt::Accessor							setterAccessor;
				declatt::Inheritation						inheritation;
				declatt::MemberType							memberType;
				ManagedAttributeInfo						attributeInfo;
			};

			class ManagedParameter : public ManagedLanguageElement
			{
			public:
				enum ParameterType
				{
					Normal,
					Default,
					Params
				};

				Ptr<ManagedType>							type;
				WString										name;
				ParameterType								parameterType;
				Ptr<ManagedExpression>						defaultValue;

				ManagedAttributeInfo						attributeInfo;
			};

			class ManagedMethod : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				Ptr<ManagedType>							implementedInterfaceType;
				WString										name;
				Ptr<ManagedType>							returnType;
				collections::List<Ptr<ManagedParameter>>	parameters;
				Ptr<ManagedStatement>						body;

				declatt::Accessor							accessor;
				declatt::Inheritation						inheritation;
				declatt::MemberType							memberType;
				ManagedAttributeInfo						attributeInfo;
				ManagedGenericInfo							genericInfo;
			};

			class ManagedConstructor : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				collections::List<Ptr<ManagedParameter>>	parameters;
				collections::List<Ptr<ManagedExpression>>	baseArguments;
				collections::List<WString>					baseDefaultParameterNames;
				collections::List<Ptr<ManagedExpression>>	baseDefaultParameterValues;
				Ptr<ManagedStatement>						body;

				declatt::Accessor							accessor;
				declatt::MemberType							memberType;
				ManagedAttributeInfo						attributeInfo;
			};

#define MANAGED_MEMBER_TARGETS(P, F)\
			F(P, ManagedField)\
			F(P, ManagedProperty)\
			F(P, ManagedMethod)\
			F(P, ManagedConstructor)\

			DEFINE_ALGORITHM_INTERFACE(ManagedMember, MANAGED_MEMBER_TARGETS)

/***********************************************************************
Basic Declarations
***********************************************************************/

			class ManagedTypeDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedDeclaration)

				enum DeclarationType
				{
					Class,
					Structure,
					Interface,
				};

				collections::List<Ptr<ManagedMember>>		members;

				declatt::Accessor							accessor;
				declatt::Inheritation						inheritation;
				ManagedGenericInfo							genericInfo;
				ManagedAttributeInfo						attributeInfo;
			};

			class ManagedNamespaceDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedDeclaration)

				collections::List<Ptr<ManagedDeclaration>>	declarations;
			};

#define MANAGED_DECLARATION_TARGETS(P, F)\
			F(P, ManagedTypeDeclaration)\
			F(P, ManagedNamespaceDeclaration)\
			F(P, ManagedExtendedDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(ManagedDeclaration, MANAGED_DECLARATION_TARGETS)

/***********************************************************************
Extended Declarations
***********************************************************************/

			class ManagedEnumerationDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)

				bool										composable;
				collections::List<WString>					enumerationNames;
				collections::List<vint>						enumerationValues;

				declatt::Accessor							accessor;
				ManagedAttributeInfo						attributeInfo;
			};

			class ManagedTypeRenameDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)

				declatt::Accessor							accessor;
				ManagedGenericInfo							genericInfo;
			};

			class ManagedUsingNamespaceDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)

				collections::List<WString>					namespaceFragments;
			};

#define MANAGED_EXTENDED_DECLARATION_TARGETS(P, F)\
			F(P, ManagedEnumerationDeclaration)\
			F(P, ManagedTypeRenameDeclaration)\
			F(P, ManagedUsingNamespaceDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedDeclaration, MANAGED_EXTENDED_DECLARATION_TARGETS)
		}
	}
}

#endif