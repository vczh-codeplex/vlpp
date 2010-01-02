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
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicExpression : public Object, private NotCopyable
			{
			public:
			};

			class BasicStatement : public Object, private NotCopyable
			{
			public:
			};

			class BasicType : public Object, private NotCopyable
			{
			public:
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
				basicil::BasicIns::ValueType				type;
				basicil::BasicIns::Argument					argument;
			};

			class BasicMbcsCharExpression : public BasicPrimitiveExpression
			{
			public:
				char										value;
			};

			class BasicUnicodeCharExpression : public BasicPrimitiveExpression
			{
			public:
				wchar_t										value;
			};

			class BasicMbcsStringExpression : public BasicPrimitiveExpression
			{
			public:
				AString										value;
			};

			class BasicUnicodeStringExpression : public BasicPrimitiveExpression
			{
			public:
				AString										value;
			};

/***********************************************************************
Operator Expression
***********************************************************************/

			class BasicUnaryExpression : public BasicExpression
			{
			public:
				enum OperatorType
				{
					PrefixIncrease,
					PostfixIncrease,
					PrefixDecrease,
					PostfixDecrase,
					GetAddress,
					DereferencePointer,
					Negative,
					Not,
				}											type;
				Ptr<BasicExpression>						operand;
			};

			class BasicBinaryExpression : public BasicExpression
			{
			public:
				enum OperatorType
				{
					Add,Sub,Mul,Div,Mod,Shl,Shr,
					And,Or,Xor,
					Lt,Le,Gt,Ge,Eq,Ne,
				}											type;
				Ptr<BasicExpression>						leftOperand;
				Ptr<BasicExpression>						rightOperand;
			};

			class BasicSubscribeExpression : public BasicExpression
			{
			public:
				Ptr<BasicExpression>						leftOperand;
				Ptr<BasicExpression>						rightOperand;
			};

/***********************************************************************
Other Expression
***********************************************************************/

			class BasicMemberExpression : public BasicExpression
			{
			public:
				Ptr<BasicExpression>						operand;
				WString										member;
			};

			class BasicInvokeExpression : public BasicExpression
			{
			public:
				Ptr<BasicExpression>						function;
				collections::List<Ptr<BasicExpression>>		arguments;
			};

			class BasicFunctionResultExpression : public BasicExpression
			{
			public:
			};

/***********************************************************************
Statement
***********************************************************************/

			class BasicEmptyStatement : public BasicStatement
			{
			public:
			};

			class BasicCompositeStatement : public BasicStatement
			{
			public:
				collections::List<Ptr<BasicStatement>>		statements;
			};

			class BasicExpressionStatement : public BasicStatement
			{
			public:
				Ptr<BasicExpression>						expression;
			};

			class BasicVariableStatement : public BasicStatement
			{
			public:
				Ptr<BasicType>								type;
				WString										name;
				Ptr<BasicExpression>						initializer;
			};

			class BasicIfStatement : public BasicStatement
			{
			public:
				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							trueStatement;
				Ptr<BasicStatement>							falseStatement;
			};

			class BasicWhileStatement : public BasicStatement
			{
			public:
				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							loopingBody;
				bool										checkConditionAfterLooping;
			};

			class BasicForStatement : public BasicStatement
			{
			public:
				collections::List<Ptr<BasicStatement>>		initializer;
				collections::List<Ptr<BasicStatement>>		sideEffect;
				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							loopingBody;
			};

			class BasicBreakStatement : public BasicStatement
			{
			};

			class BasicContinueStatement : public BasicStatement
			{
			};

			class BasicReturnStatement : public BasicStatement
			{
			};

/***********************************************************************
Type
***********************************************************************/

/***********************************************************************
Declaration
***********************************************************************/
		}
	}
}

#endif