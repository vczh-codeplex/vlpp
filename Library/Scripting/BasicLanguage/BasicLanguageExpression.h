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
				char_type,
				wchar_type,
#ifdef VCZH_64
				int_type=s64,
				uint_type=u64,
#else
				int_type=s32,
				uint_type=u32,
#endif
			};

			union BasicPrimitiveValueEnum
			{
				signed __int8								s8;
				signed __int16								s16;
				signed __int32								s32;
				signed __int64								s64;
				unsigned __int8								u8;
				unsigned __int16							u16;
				unsigned __int32							u32;
				unsigned __int64							u64;
				float										f32;
				double										f64;

				bool										bool_value;
				char										char_value;
				wchar_t										wchar_value;
#ifdef VCZH_64
				signed __int64								int_value;
				unsigned __int64							pointer_value;
#else
				signed __int32								int_value;
				unsigned __int32							pointer_value;
#endif
			};

			struct BasicPosition
			{
				vint										start;
				vint										lineStart;
				vint										lineIndex;
				vint										codeIndex;

				BasicPosition();
			};

			struct BasicLinking
			{
				WString										assemblyName;
				WString										symbolName;

				bool										HasLink()const;
				bool										operator==(const BasicLinking& linking)const;
				bool										operator!=(const BasicLinking& linking)const;
			};

			struct BasicGeneric
			{
				struct Constraint
				{
					WString									argumentName;
					WString									conceptName;
				};

				collections::List<WString>					arguments;
				collections::List<Ptr<Constraint>>			constraints;

				bool										HasGeneric()const;
			};

			struct BasicAttribute
			{
				WString										attributeName;
			};

			class BasicLanguageElement : public Object, private NotCopyable
			{
			public:
				BasicPosition								position;
			};

			class BasicExpression : public BasicLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicExpression)
			};

			class BasicStatement : public BasicLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicStatement)
			};

			class BasicType : public BasicLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicType)
			};

			class BasicDeclaration : public BasicLanguageElement
			{
			public:
				ALGORITHM_TARGET_ROOT(BasicDeclaration)

				BasicGeneric								genericDeclaration;
				BasicLinking								linking;
				collections::List<Ptr<BasicAttribute>>		attributes;
				WString										name;
			};

			class BasicProgram : public BasicLanguageElement
			{
			public:
				collections::List<Ptr<BasicDeclaration>>	declarations;
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			extern Ptr<BasicAttribute> BasicLanguage_FindFirstAttribute(const collections::ICollection<Ptr<BasicAttribute>>& attributes, const WString& name);
			extern bool BasicLanguage_FindAllAttributes(const collections::ICollection<Ptr<BasicAttribute>>& attributes, const WString& name, collections::ICollection<Ptr<BasicAttribute>>& result);

/***********************************************************************
Primitive Expression
***********************************************************************/

			class BasicPrimitiveExpression : public BasicExpression
			{
			};

			class BasicNullExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)
			};

			class BasicExceptionAddressExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)
			};

			class BasicStackDataAddressExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)
			};

			class BasicNumericExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				bool										implicitIntegerType;
				BasicPrimitiveTypeEnum						type;
				BasicPrimitiveValueEnum						argument;
			};

			class BasicMbcsStringExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				AString										value;
			};

			class BasicUnicodeStringExpression : public BasicPrimitiveExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				WString										value;
			};

/***********************************************************************
Operator Expression
***********************************************************************/

			class BasicUnaryExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

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
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

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
					BitAndAssign,
					BitOrAssign,
					Assign,
				}											type;
				Ptr<BasicExpression>						leftOperand;
				Ptr<BasicExpression>						rightOperand;
				bool										shortcut;

				BasicBinaryExpression();
			};

			class BasicSubscribeExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicExpression>						operand;
				Ptr<BasicExpression>						subscribe;
			};

/***********************************************************************
Other Expression
***********************************************************************/

			class BasicMemberExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicExpression>						operand;
				WString										member;
				bool										pointerMember;
			};

			class BasicInvokeExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicExpression>						function;
				collections::List<Ptr<BasicExpression>>		arguments;
			};

			class BasicFunctionResultExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

			};

			class BasicSizeofExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicType>								type;
			};

			class BasicOffsetofExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicType>								type;
				WString										member;
			};

			class BasicCastingExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicExpression>						operand;
				Ptr<BasicType>								type;
			};

			class BasicReferenceExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				WString										name;
			};

			class BasicInstantiatedExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				Ptr<BasicReferenceExpression>				reference;
				collections::List<Ptr<BasicType>>			argumentTypes;
			};

			class BasicInstanceFunctionExpression : public BasicExpression
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicExpression)

				WString										conceptName;
				WString										functionName;
				Ptr<BasicType>								type;
			};

/***********************************************************************
Statement
***********************************************************************/

			class BasicEmptyStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

			};

			class BasicCompositeStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				collections::List<Ptr<BasicStatement>>		statements;
			};

			class BasicExpressionStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicExpression>						expression;
			};

			class BasicVariableStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicType>								type;
				WString										name;
				Ptr<BasicExpression>						initializer;
			};

			class BasicIfStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							trueStatement;
				Ptr<BasicStatement>							falseStatement;
			};

			class BasicWhileStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicExpression>						beginCondition;
				Ptr<BasicExpression>						endCondition;
				Ptr<BasicStatement>							statement;
			};

			class BasicForStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicStatement>							initializer;
				Ptr<BasicStatement>							sideEffect;
				Ptr<BasicExpression>						condition;
				Ptr<BasicStatement>							statement;
			};

			class BasicBreakStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)
			};

			class BasicContinueStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)
			};

			class BasicReturnStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)
			};

			class BasicTryCatchStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicStatement>							tryStatement;
				Ptr<BasicStatement>							catchStatement;
			};

			class BasicThrowStatement : public BasicStatement
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicStatement)

				Ptr<BasicExpression>						expression;
			};

/***********************************************************************
Type
***********************************************************************/

			class BasicPrimitiveType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				BasicPrimitiveTypeEnum						type;
			};

			class BasicPointerType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicType>								elementType;
			};

			class BasicArrayType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicType>								elementType;
				Ptr<BasicExpression>						count;
			};

			class BasicReferenceType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				WString										name;
			};

			class BasicFunctionType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicType>								returnType;
				collections::List<Ptr<BasicType>>			parameterTypes;
			};

			class BasicInstantiatedGenericType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicType>								elementType;
				collections::List<Ptr<BasicType>>			argumentTypes;
			};

			class BasicTypeofExpressionType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicExpression>						expression;
			};

			class BasicTypeofMemberType : public BasicType
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicType)

				Ptr<BasicType>								type;
				WString										member;
			};

/***********************************************************************
Declaration
***********************************************************************/

			class BasicFunctionDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				Ptr<BasicFunctionType>						signatureType;
				collections::List<WString>					parameterNames;
				Ptr<BasicStatement>							statement;
				bool										foreignFunction;
			};

			class BasicStructureDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				collections::List<Ptr<BasicType>>			memberTypes;
				collections::List<WString>					memberNames;
				bool										defined;
			};

			class BasicVariableDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				Ptr<BasicType>								type;
				Ptr<BasicExpression>						initializer;
				bool										constant;
			};

			class BasicTypeRenameDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				Ptr<BasicType>								type;
			};

			class BasicConceptBaseDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				struct FunctionConcept
				{
					WString									name;
					Ptr<BasicFunctionType>					signatureType;
				};

				WString										conceptType;
				collections::List<Ptr<FunctionConcept>>		functions;
			};

			class BasicConceptInstanceDeclaration : public BasicDeclaration
			{
			public:
				ALGORITHM_ACCEPT_DECLARATION(BasicDeclaration)

				struct FunctionInstance
				{
					WString									name;
					Ptr<BasicReferenceExpression>			normalFunction;
					Ptr<BasicInstantiatedExpression>		genericFunction;
				};

				bool										defined;
				Ptr<BasicType>								instanceType;
				collections::List<Ptr<FunctionInstance>>	functions;
			};

/***********************************************************************
Algorithms
***********************************************************************/

#define BASIC_EXPRESSION_TARGETS(P, F)\
			F(P, BasicNullExpression)\
			F(P, BasicExceptionAddressExpression)\
			F(P, BasicStackDataAddressExpression)\
			F(P, BasicNumericExpression)\
			F(P, BasicMbcsStringExpression)\
			F(P, BasicUnicodeStringExpression)\
			F(P, BasicUnaryExpression)\
			F(P, BasicBinaryExpression)\
			F(P, BasicSubscribeExpression)\
			F(P, BasicMemberExpression)\
			F(P, BasicInvokeExpression)\
			F(P, BasicFunctionResultExpression)\
			F(P, BasicSizeofExpression)\
			F(P, BasicOffsetofExpression)\
			F(P, BasicCastingExpression)\
			F(P, BasicReferenceExpression)\
			F(P, BasicInstantiatedExpression)\
			F(P, BasicInstanceFunctionExpression)\

			DEFINE_ALGORITHM_INTERFACE(BasicExpression, BASIC_EXPRESSION_TARGETS)

#define BASIC_STATEMENT_TARGETS(P, F)\
			F(P, BasicEmptyStatement)\
			F(P, BasicCompositeStatement)\
			F(P, BasicExpressionStatement)\
			F(P, BasicVariableStatement)\
			F(P, BasicIfStatement)\
			F(P, BasicWhileStatement)\
			F(P, BasicForStatement)\
			F(P, BasicBreakStatement)\
			F(P, BasicContinueStatement)\
			F(P, BasicReturnStatement)\
			F(P, BasicTryCatchStatement)\
			F(P, BasicThrowStatement)\

			DEFINE_ALGORITHM_INTERFACE(BasicStatement, BASIC_STATEMENT_TARGETS)

#define BASIC_TYPE_TARGETS(P, F)\
			F(P, BasicPrimitiveType)\
			F(P, BasicPointerType)\
			F(P, BasicArrayType)\
			F(P, BasicReferenceType)\
			F(P, BasicFunctionType)\
			F(P, BasicInstantiatedGenericType)\
			F(P, BasicTypeofExpressionType)\
			F(P, BasicTypeofMemberType)\

			DEFINE_ALGORITHM_INTERFACE(BasicType, BASIC_TYPE_TARGETS)

#define BASIC_DECLARATION_TARGETS(P, F)\
			F(P, BasicFunctionDeclaration)\
			F(P, BasicStructureDeclaration)\
			F(P, BasicVariableDeclaration)\
			F(P, BasicTypeRenameDeclaration)\
			F(P, BasicConceptBaseDeclaration)\
			F(P, BasicConceptInstanceDeclaration)\

			DEFINE_ALGORITHM_INTERFACE(BasicDeclaration, BASIC_DECLARATION_TARGETS)

		}
	}
}

#endif