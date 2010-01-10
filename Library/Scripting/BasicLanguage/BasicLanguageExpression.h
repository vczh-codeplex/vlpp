/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXPRESSION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXPRESSION

#include "..\..\Pointer.h"
#include "..\..\String.h"
#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "..\Common\AlgorithmDeclaration.h"
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			enum BasicPrimitiveTypeEnum
			{
				s8,s16,s32,s64,
				u8,u16,u32,u64,
				f32,f64,
				bool_type,
				void_type,
				char_type=s8,
				wchar_type=u16,
#ifdef _WIN64
				int_type=s64,
#else
				int_type=s32,
#endif
			};

			union BasicPrimitiveValueEnum
			{
				signed __int8				s8;
				signed __int16				s16;
				signed __int32				s32;
				signed __int64				s64;
				unsigned __int8				u8;
				unsigned __int16			u16;
				unsigned __int32			u32;
				unsigned __int64			u64;
				float						f32;
				double						f64;

				bool						bool_value;
				char						char_value;
				wchar_t						wchar_value;
#ifdef _WIN64
				signed __int64				int_value;
				unsigned __int64			pointer_value;
#else
				signed __int32				int_value;
				unsigned __int32			pointer_value;
#endif
			};

			class BasicExpressionAlgorithm;
			class BasicStatementAlgorithm;
			class BasicTypeAlgorithm;
			class BasicDeclarationAlgorithm;

			class BasicExpression : public Object, private NotCopyable
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicExpressionAlgorithm)
			};

			class BasicStatement : public Object, private NotCopyable
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicStatementAlgorithm)
			};

			class BasicType : public Object, private NotCopyable
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicTypeAlgorithm)
			};

			class BasicDeclaration : public Object, private NotCopyable
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicDeclarationAlgorithm)

				WString										name;
			};

			class BasicProgram : public Object, private NotCopyable
			{
			public:
				collections::List<Ptr<BasicDeclaration>>	declarations;
			};

/***********************************************************************
Primitive Expression
***********************************************************************/

			class BasicPrimitiveExpression : public BasicExpression
			{
			};

			class BasicNumericExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				BasicPrimitiveTypeEnum						type;
				BasicPrimitiveValueEnum						argument;
			};

			class BasicMbcsStringExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				AString										value;
			};

			class BasicUnicodeStringExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										value;
			};

/***********************************************************************
Operator Expression
***********************************************************************/

			class BasicUnaryExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				enum OperatorType
				{
					PrefixIncrease,
					PostfixIncrease,
					PrefixDecrease,
					PostfixDecrease,
					GetAddress,
					DereferencePointer,
					Negative,
					Not,
					BitNot,
				}											type;
				Ptr<BasicExpression>						operand;
			};

			class BasicBinaryExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				enum OperatorType
				{
					Add,Sub,Mul,Div,Mod,Shl,Shr,
					And,Or,Xor,BitAnd,BitOr,
					Lt,Le,Gt,Ge,Eq,Ne,
					AddAssign,
					SubAssign,
					MulAssign,
					DivAssign,
					ModAssign,
					ShlAssign,
					ShrAssign,
					AndAssign,
					OrAssign,
					XorAssign,
					Assign,
				}											type;
				Ptr<BasicExpression>						leftOperand;
				Ptr<BasicExpression>						rightOperand;
			};

			class BasicSubscribeExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						operand;
				Ptr<BasicExpression>						subscribe;
			};

/***********************************************************************
Other Expression
***********************************************************************/

			class BasicMemberExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						operand;
				WString										member;
				bool										pointerMember;
			};

			class BasicInvokeExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						function;
				collections::List<Ptr<BasicExpression>>		arguments;
			};

			class BasicFunctionResultExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

			};

			class BasicCastingExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						operand;
				Ptr<BasicType>								type;
			};

			class BasicReferenceExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										name;
			};

/***********************************************************************
Statement
***********************************************************************/

			class BasicEmptyStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

			};

			class BasicCompositeStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				collections::List<Ptr<BasicStatement>>		statements;
			};

			class BasicExpressionStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						expression;
			};

			class BasicVariableStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								type;
				WString										name;
				Ptr<BasicExpression>						initializer;
			};

			class BasicIfStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							trueStatement;
				Ptr<BasicStatement>							falseStatement;
			};

			class BasicWhileStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							statement;
				bool										checkConditionAfterLooping;
			};

			class BasicForStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicStatement>							initializer;
				Ptr<BasicStatement>							sideEffect;
				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							statement;
			};

			class BasicBreakStatement : public BasicStatement
			{
				ALGORITHM_ACCEPT_DECLARATION

			};

			class BasicContinueStatement : public BasicStatement
			{
				ALGORITHM_ACCEPT_DECLARATION

			};

			class BasicReturnStatement : public BasicStatement
			{
				ALGORITHM_ACCEPT_DECLARATION

			};

/***********************************************************************
Type
***********************************************************************/

			class BasicPrimitiveType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				BasicPrimitiveTypeEnum						type;
			};

			class BasicPointerType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								elementType;
			};

			class BasicArrayType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								elementType;
				int											size;
			};

			class BasicReferenceType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				WString										name;
			};

			class BasicFunctionType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								returnType;
				collections::List<Ptr<BasicType>>			parameterTypes;
			};

/***********************************************************************
Declaration
***********************************************************************/

			class BasicFunctionDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicFunctionType>						signatureType;
				collections::List<WString>					parameterNames;
				Ptr<BasicStatement>							statement;
				WString										externalKey;
			};

			class BasicStructureDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				collections::List<Ptr<BasicType>>			memberTypes;
				collections::List<WString>					memberNames;
			};

			class BasicVariableDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								type;
				Ptr<BasicExpression>						initializer;
			};

			class BasicTypeRenameDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION

				Ptr<BasicType>								type;
			};

/***********************************************************************
Algorithms
***********************************************************************/

#define BASIC_EXPRESSION_TARGETS(F)\
			F(BasicNumericExpression)\
			F(BasicMbcsStringExpression)\
			F(BasicUnicodeStringExpression)\
			F(BasicUnaryExpression)\
			F(BasicBinaryExpression)\
			F(BasicSubscribeExpression)\
			F(BasicMemberExpression)\
			F(BasicInvokeExpression)\
			F(BasicFunctionResultExpression)\
			F(BasicCastingExpression)\
			F(BasicReferenceExpression)\

			DEFINE_ALGORITHM_INTERFACE(BasicExpressionAlgorithm, BASIC_EXPRESSION_TARGETS)

#define BASIC_STATEMENT_TARGETS(F)\
			F(BasicEmptyStatement)\
			F(BasicCompositeStatement)\
			F(BasicExpressionStatement)\
			F(BasicVariableStatement)\
			F(BasicIfStatement)\
			F(BasicWhileStatement)\
			F(BasicForStatement)\
			F(BasicBreakStatement)\
			F(BasicContinueStatement)\
			F(BasicReturnStatement)\

			DEFINE_ALGORITHM_INTERFACE(BasicStatementAlgorithm, BASIC_STATEMENT_TARGETS)

#define BASIC_TYPE_TARGETS(F)\
			F(BasicPrimitiveType)\
			F(BasicPointerType)\
			F(BasicArrayType)\
			F(BasicReferenceType)\
			F(BasicFunctionType)\

			DEFINE_ALGORITHM_INTERFACE(BasicTypeAlgorithm, BASIC_TYPE_TARGETS)

#define BASIC_DECLARATION_TARGETS(F)\
			F(BasicFunctionDeclaration)\
			F(BasicStructureDeclaration)\
			F(BasicVariableDeclaration)\
			F(BasicTypeRenameDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(BasicDeclarationAlgorithm, BASIC_DECLARATION_TARGETS)

		}
	}
}

#endif