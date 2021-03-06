/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
				vint												start;
				vint												lineStart;
				vint												lineIndex;
				vint												codeIndex;

				ManagedPosition();
			};

			class ManagedLanguageElement : public Object, private NotCopyable
			{
			public:
				ManagedPosition										position;
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

			class ManagedReferencedType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				WString												name;
			};

			class ManagedMemberType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				Ptr<ManagedType>									operand; // null = global::
				WString												member;
			};

			class ManagedInstantiatedGenericType : public ManagedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedType)

				Ptr<ManagedType>									elementType;
				collections::List<Ptr<ManagedType>>					argumentTypes;
			};

#define MANAGED_TYPE_TARGETS(P, F)\
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

				Ptr<ManagedType>									elementType;
				int													dimensionCount;
			};

			class ManagedFunctionType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)

				Ptr<ManagedType>									returnType;
				collections::List<Ptr<ManagedType>>					parameterTypes;
			};

			class ManagedEventType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)
					
				collections::List<Ptr<ManagedType>>					parameterTypes;
			};

			// lambda parameter
			// lambda return type
			// using
			// foreach
			// variable statement
			class ManagedAutoReferType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)
			};

			class ManagedDynamicType : public ManagedExtendedType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedType)
			};

#define MANAGED_EXTENDED_TYPE_TARGETS(P, F)\
			F(P, ManagedArrayType)\
			F(P, ManagedFunctionType)\
			F(P, ManagedEventType)\
			F(P, ManagedAutoReferType)\
			F(P, ManagedDynamicType)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedType, MANAGED_EXTENDED_TYPE_TARGETS)

/***********************************************************************
Basic Expression Fragments
***********************************************************************/

			class ManagedArgument : public ManagedLanguageElement
			{
			public:
				enum ArgumentType
				{
					Normal,
					Ref,
					Out,
				};
				Ptr<ManagedExpression>								value;
				ArgumentType										argumentType;
				WString												defaultParameterName;
			};

			class ManagedPropertySetter : public ManagedLanguageElement
			{
			public:
				Ptr<ManagedExpression>								value;
				WString												propertyName;
			};

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

				bool												sign;
				union
				{
					signed __int64									signedInteger;
					unsigned __int64								unsignedInteger;
				}													value;
			};

			class ManagedFloatExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				double												value;
			};

			class ManagedBooleanExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				bool												value;
			};

			class ManagedCharExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				wchar_t												value;
			};

			class ManagedStringExpression : public ManagedPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				WString												value;
			};

			class ManagedReferenceExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				WString												name;
			};

			class ManagedMemberExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>								operand; // null = global::
				WString												member;
			};

			class ManagedInstantiatedExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>								operand;
				collections::List<Ptr<ManagedType>>					argumentTypes;
			};

			class ManagedInvokeExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedExpression>								function;
				collections::List<Ptr<ManagedArgument>>				arguments;
			};

			class ManagedNewObjectExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)

				Ptr<ManagedType>									objectType;
				collections::List<Ptr<ManagedArgument>>				arguments;
				collections::List<Ptr<ManagedPropertySetter>>		properties;
			};

			class ManagedFunctionResultExpression : public ManagedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExpression)
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

				Ptr<ManagedExpression>								leftOperand;
				Ptr<ManagedExpression>								rightOperand;
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
			F(P, ManagedInstantiatedExpression)\
			F(P, ManagedInvokeExpression)\
			F(P, ManagedNewObjectExpression)\
			F(P, ManagedFunctionResultExpression)\
			F(P, ManagedThisExpression)\
			F(P, ManagedBaseExpression)\
			F(P, ManagedAssignmentExpression)\
			F(P, ManagedExtendedExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExpression, MANAGED_EXPRESSION_TARGETS)

/***********************************************************************
Extended Expressions
***********************************************************************/

			class ManagedLambdaParameter : public Object
			{
			public:
				Ptr<ManagedType>									type; // can be autoref
				WString												name;
			};

			class ManagedLambdaExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				collections::List<Ptr<ManagedLambdaParameter>>		parameters;
				Ptr<ManagedType>									returnType; // can be autoref
				Ptr<ManagedStatement>								body;
			};

			class ManagedChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								condition;
				Ptr<ManagedExpression>								trueExpression;
				Ptr<ManagedExpression>								falseExpression;
			};

			class ManagedNullChoiceExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								valueExpression;
				Ptr<ManagedExpression>								candidateExpression;
			};

			class ManagedTypeofExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedType>									type;
			};

			class ManagedDefaultExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedType>									type;
			};

			class ManagedUnaryExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								operand;
				WString												operatorName;
			};

			class ManagedBinaryExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								leftOperand;
				Ptr<ManagedExpression>								rightOperand;
				WString												operatorName;
			};

			class ManagedNewArrayExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedType>									objectType;
				collections::List<Ptr<ManagedExpression>>			sizes;
			};

			class ManagedIsTypeExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								operand;
				Ptr<ManagedType>									type;
			};

			class ManagedCastingExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								operand;
				Ptr<ManagedType>									type;
			};

			class ManagedIndexExpression : public ManagedExtendedExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedExpression)

				Ptr<ManagedExpression>								operand;
				collections::List<Ptr<ManagedExpression>>			indices;
				bool												forRead;
			};

#define MANAGED_EXTENDED_EXPRESSION_TARGETS(P, F)\
			F(P, ManagedLambdaExpression)\
			F(P, ManagedChoiceExpression)\
			F(P, ManagedNullChoiceExpression)\
			F(P, ManagedTypeofExpression)\
			F(P, ManagedDefaultExpression)\
			F(P, ManagedUnaryExpression)\
			F(P, ManagedBinaryExpression)\
			F(P, ManagedNewArrayExpression)\
			F(P, ManagedIsTypeExpression)\
			F(P, ManagedCastingExpression)\
			F(P, ManagedIndexExpression)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedExpression, MANAGED_EXTENDED_EXPRESSION_TARGETS)

/***********************************************************************
Basic Statement Fragments
***********************************************************************/

			class ManagedCatchClause : public ManagedLanguageElement
			{
			public:
				Ptr<ManagedType>									exceptionType;
				WString												exceptionName; // nullable
				Ptr<ManagedStatement>								exceptionHandler;
			};

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

				collections::List<Ptr<ManagedStatement>>			statements;
			};

			class ManagedExpressionStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>								expression;
			};

			class ManagedVariableStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				bool												constant;
				Ptr<ManagedType>									type; // can be autoref
				WString												name;
				Ptr<ManagedExpression>								initializer; // nullable
			};

			class ManagedIfStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>								condition;
				Ptr<ManagedStatement>								trueStatement;
				Ptr<ManagedStatement>								falseStatement; // nullable
			};

			class ManagedWhileStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>								beginCondition; // nullable
				Ptr<ManagedExpression>								endCondition; // nullable
				Ptr<ManagedStatement>								statement;
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

				Ptr<ManagedStatement>								tryStatement;
				collections::List<Ptr<ManagedCatchClause>>			catches;
				Ptr<ManagedStatement>								finallyStatement; // nullable
			};

			class ManagedThrowStatement : public ManagedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedStatement)

				Ptr<ManagedExpression>								expression; // nullable
			};

#define MANAGED_STATEMENT_TARGETS(P, F)\
			F(P, ManagedEmptyStatement)\
			F(P, ManagedCompositeStatement)\
			F(P, ManagedExpressionStatement)\
			F(P, ManagedVariableStatement)\
			F(P, ManagedIfStatement)\
			F(P, ManagedWhileStatement)\
			F(P, ManagedBreakStatement)\
			F(P, ManagedContinueStatement)\
			F(P, ManagedReturnStatement)\
			F(P, ManagedTryCatchStatement)\
			F(P, ManagedThrowStatement)\
			F(P, ManagedExtendedStatement)

			DEFINE_ALGORITHM_INTERFACE(ManagedStatement, MANAGED_STATEMENT_TARGETS)

/***********************************************************************
Extended Statement Fragments
***********************************************************************/

			class ManagedCaseClause : public ManagedLanguageElement
			{
			public:
				collections::List<Ptr<ManagedExpression>>			conditions;
				Ptr<ManagedStatement>								statement;
			};

/***********************************************************************
Extended Statements
***********************************************************************/

			class ManagedUsingStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedType>									type; // can be autoref
				WString												name;
				Ptr<ManagedExpression>								initialize;
				Ptr<ManagedStatement>								statement;
			};

			class ManagedLockStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedExpression>								lock;
				Ptr<ManagedStatement>								statement;
			};

			class ManagedSelectStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedExpression>								expression;
				collections::List<Ptr<ManagedCaseClause>>			cases;
				Ptr<ManagedStatement>								defaultStatement; // nullable
			};

			class ManagedForStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				collections::List<Ptr<ManagedVariableStatement>>	initializers;
				collections::List<Ptr<ManagedExpression>>			sideEffects;
				Ptr<ManagedExpression>								condition; // nullable
				Ptr<ManagedStatement>								statement;
			};

			class ManagedForEachStatement : public ManagedExtendedStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedStatement)

				Ptr<ManagedType>									type; // can be autoref
				WString												name;
				Ptr<ManagedExpression>								container;
				Ptr<ManagedStatement>								statement;
			};

#define MANAGED_EXTENDED_STATEMENT_TARGETS(P, F)\
			F(P, ManagedUsingStatement)\
			F(P, ManagedLockStatement)\
			F(P, ManagedSelectStatement)\
			F(P, ManagedForStatement)\
			F(P, ManagedForEachStatement)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedStatement, MANAGED_EXTENDED_STATEMENT_TARGETS)

/***********************************************************************
Basic Declaration Fragments
***********************************************************************/

			struct ManagedAttributeInfo
			{
			public:
				collections::List<Ptr<ManagedNewObjectExpression>>	attributes;
			};

			struct ManagedGenericInfo
			{
			public:
				enum ArgumentConversion
				{
					InOut,
					In,
					Out,
				};

				struct Argument
				{
					WString											name;
					collections::List<Ptr<ManagedType>>				typeConstraints;
					bool											newConstraint;
					ArgumentConversion								conversion;
				};

				collections::List<Ptr<Argument>>					arguments;

				bool												HasGeneric();
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

			class ManagedExtendedMember : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				ALGORITHM_TARGET_ROOT(ManagedExtendedMember)
			};

			class ManagedParameter : public ManagedLanguageElement
			{
			public:
				enum ParameterType
				{
					Normal,
					Params,
					Ref,
					Out,
					This,
				};

				ManagedAttributeInfo								attributeInfo;
				Ptr<ManagedType>									type;
				WString												name;
				ParameterType										parameterType;
				Ptr<ManagedExpression>								defaultValue;
			};

/***********************************************************************
Basic Members
***********************************************************************/

			class ManagedField : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				ManagedAttributeInfo								attributeInfo;
				declatt::Accessor									accessor;
				declatt::MemberType									memberType;
				declatt::DataType									dataType;

				Ptr<ManagedType>									type;
				WString												name;
				Ptr<ManagedExpression>								initializer; // nullable

			};

			class ManagedMethod : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)
					
				ManagedAttributeInfo								attributeInfo;
				ManagedGenericInfo									genericInfo;
				declatt::Accessor									accessor;
				declatt::MemberType									memberType;
				declatt::Inheritation								inheritation;
				bool												externalMethod;

				Ptr<ManagedType>									implementedInterfaceType; // nullable
				WString												name;
				collections::List<Ptr<ManagedParameter>>			parameters;
				Ptr<ManagedType>									returnType;
				Ptr<ManagedStatement>								body; // ;able

			};

			class ManagedConstructor : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)
					
				ManagedAttributeInfo								attributeInfo;
				declatt::Accessor									accessor;

				bool												implicit;
				collections::List<Ptr<ManagedParameter>>			parameters;
				collections::List<Ptr<ManagedArgument>>				baseArguments;
				Ptr<ManagedStatement>								body;
			};

			class ManagedTypeMember : public ManagedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedMember)

				Ptr<ManagedDeclaration>								declaration;
			};

#define MANAGED_MEMBER_TARGETS(P, F)\
			F(P, ManagedField)\
			F(P, ManagedMethod)\
			F(P, ManagedConstructor)\
			F(P, ManagedTypeMember)\
			F(P, ManagedExtendedMember)\

			DEFINE_ALGORITHM_INTERFACE(ManagedMember, MANAGED_MEMBER_TARGETS)

/***********************************************************************
Extended Members
***********************************************************************/

			class ManagedProperty : public ManagedExtendedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedMember)
					
				ManagedAttributeInfo								attributeInfo;
				declatt::Accessor									accessor;
				declatt::MemberType									memberType;
				declatt::Inheritation								inheritation;

				Ptr<ManagedType>									type;
				Ptr<ManagedType>									implementedInterfaceType; // nullable
				WString												name;

				Ptr<ManagedStatement>								getter; // ;able
				Ptr<ManagedStatement>								setter; // ;able
				WString												setterParameter; // nullable if setter == empty
				declatt::Accessor									setterAccessor;
			};

			class ManagedConverterOperator : public ManagedExtendedMember
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedMember)
					
				ManagedAttributeInfo								attributeInfo;
				ManagedGenericInfo									genericInfo;
				declatt::Accessor									accessor;
				declatt::MemberType									memberType;
				declatt::Inheritation								inheritation;
					
				bool												implicit;
				Ptr<ManagedType>									targetType;
				Ptr<ManagedStatement>								body;

			};

#define MANAGED_EXTENDED_MEMBER_TARGETS(P, F)\
			F(P, ManagedProperty)\
			F(P, ManagedConverterOperator)\

			DEFINE_ALGORITHM_INTERFACE(ManagedExtendedMember, MANAGED_EXTENDED_MEMBER_TARGETS)

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
				
				DeclarationType										declarationType;
				WString												name;
				collections::List<Ptr<ManagedType>>					baseTypes;
				collections::List<Ptr<ManagedMember>>				members;

				declatt::Accessor									accessor;
				declatt::Inheritation								inheritation;
				declatt::MemberType									memberType;
				ManagedGenericInfo									genericInfo;
				ManagedAttributeInfo								attributeInfo;
			};

			class ManagedNamespaceDeclaration : public ManagedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedDeclaration)
					
				collections::List<WString>							namespaceFragments;
				collections::List<Ptr<ManagedDeclaration>>			declarations;
			};

#define MANAGED_DECLARATION_TARGETS(P, F)\
			F(P, ManagedTypeDeclaration)\
			F(P, ManagedNamespaceDeclaration)\
			F(P, ManagedExtendedDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(ManagedDeclaration, MANAGED_DECLARATION_TARGETS)

/***********************************************************************
Extended Declaration Fragments
***********************************************************************/

			class ManagedEnumItem : public ManagedLanguageElement
			{
			public:
				ManagedAttributeInfo								attributeInfo;
				WString												name;
				Ptr<ManagedExpression>								value;
			};

/***********************************************************************
Extended Declarations
***********************************************************************/

			class ManagedEnumerationDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)
					
				WString												name;
				bool												composable;
				collections::List<Ptr<ManagedEnumItem>>				items;

				declatt::Accessor									accessor;
				ManagedAttributeInfo								attributeInfo;
			};

			class ManagedTypeRenameDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)

				WString												name;
				Ptr<ManagedType>									type;

				declatt::Accessor									accessor;
				ManagedGenericInfo									genericInfo;
			};

			class ManagedUsingNamespaceDeclaration : public ManagedExtendedDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(ManagedExtendedDeclaration)

				collections::List<WString>							namespaceFragments;
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