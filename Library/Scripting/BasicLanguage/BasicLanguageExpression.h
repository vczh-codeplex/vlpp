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

			class BasicDeclaration : public Object, private NotCopyable
			{
			public:
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
				BasicPrimitiveTypeEnum						type;
				BasicPrimitiveValueEnum						argument;
			};

			class BasicMbcsStringExpression : public BasicPrimitiveExpression
			{
			public:
				AString										value;
			};

			class BasicUnicodeStringExpression : public BasicPrimitiveExpression
			{
			public:
				WString										value;
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
					PostfixDecrease,
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
				Ptr<BasicExpression>						operand;
				Ptr<BasicExpression>						subscribe;
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

			class BasicCastingExpression : public BasicExpression
			{
			public:
				Ptr<BasicExpression>						operand;
				Ptr<BasicType>								type;
			};

			class BasicReferenceExpression : public BasicExpression
			{
			public:
				WString										name;
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

			class BasicPrimitiveType : public BasicType
			{
			public:
				BasicPrimitiveTypeEnum						type;
			};

			class BasicPointerType : public BasicType
			{
			public:
				Ptr<BasicType>								elementType;
			};

			class BasicArrayType : public BasicType
			{
			public:
				Ptr<BasicType>								elementType;
				int											size;
			};

			class BasicReferenceType : public BasicType
			{
			public:
				WString										name;
			};

			class BasicFunctionType : public BasicType
			{
			public:
				Ptr<BasicType>								returnType;
				collections::List<Ptr<BasicType>>			parameterTypes;
			};

/***********************************************************************
Declaration
***********************************************************************/

			class BasicFunctionDeclaration : public BasicDeclaration
			{
			public:
				Ptr<BasicFunctionType>						signatureType;
				collections::List<WString>					parameterNames;
				Ptr<BasicStatement>							functionBody;
				WString										externalKey;
			};

			class BasicStructureDeclaration : public BasicDeclaration
			{
			public:
				collections::List<Ptr<BasicType>>			memberTypes;
				collections::List<Ptr<BasicType>>			memberNames;
			};

			class BasicVariableDeclaration : public BasicDeclaration
			{
			public:
				Ptr<BasicType>								type;
				Ptr<BasicExpression>						initializer;
			};

			class BasicTypeRenameDeclaration : public BasicDeclaration
			{
			public:
				Ptr<BasicType>								type;
			};
		}
	}
}

#endif