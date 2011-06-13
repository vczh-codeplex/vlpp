#ifndef VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXPARSER_COMMON
#define VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXPARSER_COMMON

#include "ManagedXParser.h"
#include "ManagedXErrorMessage.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"
#include "..\..\..\Collections\Operation.h"

/***********************************************************************
+	op_pos
-	op_neg
!	op_not
~	op_bitnot
++	op_preinc/op_postinc
--	op_predec/op_postdec
====================================
*	op_mul
/	op_div
%	op_mod

+	op_add
-	op_sub

<<	op_shl
>>	op_shr

<	op_lt
<=	op_le
>	op_gt
>=	op_ge
as	ManagedCastingExpression
is	ManagedIsTypeExpression

==	op_eq
!=	op_ne

&	op_bitand

^	op_xor

|	op_bitor

&&	op_and

||	op_or

??	ManagedNullChoiceExpression

?:	ManagedChoiceExpression

+=	op_add_eq
-=	op_sub_eq
*=	op_mul_eq
/=	op_div_eq
%=	op_mod_eq
&&=	op_and_eq
&=	op_bitand_eq
||=	op_or_eq
|=	op_bitor_eq
^=	op_xor_eq
<<=	op_shl_eq
>>=	op_shr_eq
=	ManagedAssignmentExpression
***********************************************************************/

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace regex;
			using namespace combinator;

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			template<typename T>
			Ptr<T> CreateNode(const RegexToken& position)
			{
				Ptr<T> node=new T;
				node->position.start=position.start;
				node->position.lineStart=position.lineStart;
				node->position.lineIndex=position.lineIndex;
				node->position.codeIndex=position.codeIndex;
				return node;
			}

			template<typename T, typename U>
			Ptr<T> CreateNode(Ptr<U> copy)
			{
				Ptr<T> node=new T;
				node->position.start=copy->position.start;
				node->position.lineStart=copy->position.lineStart;
				node->position.lineIndex=copy->position.lineIndex;
				node->position.codeIndex=copy->position.codeIndex;
				return node;
			}

			extern Node<TokenInput<RegexToken>, RegexToken>		CreateToken(List<WString>& tokens, const WString& token);
			extern wchar_t										Escape(wchar_t input);
			extern wchar_t										EscapeWalk(const wchar_t*& input);
			extern WString										EscapeString(const WString& input);
			extern WString										ConvertID(const WString& input);

/***********************************************************************
Óï·¨·ÖÎöÆ÷
***********************************************************************/

			typedef Node<TokenInput<RegexToken>, RegexToken>									TokenType;
			typedef Node<TokenInput<RegexToken>, declatt::Accessor>								AccessorNode;
			typedef Node<TokenInput<RegexToken>, declatt::Inheritation>							InheritationNode;
			typedef Node<TokenInput<RegexToken>, declatt::MemberType>							MemberTypeNode;
			typedef Node<TokenInput<RegexToken>, declatt::DataType>								DataTypeNode;
			typedef Node<TokenInput<RegexToken>, bool>											ImplicitExplicitNode;
			typedef Node<TokenInput<RegexToken>, bool>											InternalExternalNode;
			typedef Node<TokenInput<RegexToken>, ManagedGenericInfo::ArgumentConversion>		GenericArgconv;
			typedef Node<TokenInput<RegexToken>, ManagedParameter::ParameterType>				FunctionArgconv;
			typedef Node<TokenInput<RegexToken>, ManagedArgument::ArgumentType>					InvokeArgconv;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedType>>								TypeNode;
			
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedArgument>>							ArgumentNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedLambdaParameter>>					LambdaParameterNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedPropertySetter>>					PropertySetterNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedExpression>>						IncompleteExpressionNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedExpression>>						ExpressionNode;
			
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedVariableStatement>>					ForInitializerNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedCatchClause>>						CatchClauseNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedCaseClause>>						CaseClauseNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedStatement>>							IncompleteStatementNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedStatement>>							StatementNode;

			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo::Argument>>				GenericArgumentNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo>>						GenericInfoNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedNewObjectExpression>>				AttributeItemNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedAttributeInfo>>						AttributeInfoNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedEnumItem>>							EnumItemNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedParameter>>							ParameterNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedMember>>							MemberNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedDeclaration>>						DeclarationNode;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedXUnit>>								UnitRule;

			class ManagedXParserImpl : public ManagedXParser
			{
			protected:
#define ERROR_HANDLER(NAME, TYPE)																						\
				static ParsingResult<TYPE> NAME(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& info)	\
				{																											\
					info.errors.Clear();																					\
					info.errors.Add(new CombinatorError<TokenInput<RegexToken>>(ManagedXErrorMessage::NAME(), input));		\
					return ParsingResult<TYPE>();																			\
				}
			
				ERROR_HANDLER(NeedId,							RegexToken)
				ERROR_HANDLER(NeedWhile,						RegexToken)
				ERROR_HANDLER(NeedAs,							RegexToken)
				ERROR_HANDLER(NeedInOut,						RegexToken)
				ERROR_HANDLER(NeedBase,							RegexToken)
				
				ERROR_HANDLER(NeedDot,							RegexToken)
				ERROR_HANDLER(NeedColon,						RegexToken)
				ERROR_HANDLER(NeedSemicolon,					RegexToken)
				ERROR_HANDLER(NeedLambda,						RegexToken)
				ERROR_HANDLER(NeedEq,							RegexToken)
				ERROR_HANDLER(NeedLt,							RegexToken)
				ERROR_HANDLER(NeedGt,							RegexToken)
				ERROR_HANDLER(NeedOpenDeclBrace,				RegexToken)
				ERROR_HANDLER(NeedCloseDeclBrace,				RegexToken)
				ERROR_HANDLER(NeedOpenArrayBrace,				RegexToken)
				ERROR_HANDLER(NeedCloseArrayBrace,				RegexToken)
				ERROR_HANDLER(NeedOpenExpBrace,					RegexToken)
				ERROR_HANDLER(NeedCloseExpBrace,				RegexToken)
			
				ERROR_HANDLER(NeedExpression,					RegexToken)
				ERROR_HANDLER(NeedStatement,					RegexToken)
#undef ERROR_HANDLER
			protected:
				Ptr<RegexLexer>						lexer;

				/*--------KEYWORDS--------*/

				TokenType							TYPEKEYWORD, VAR, DYNAMIC, FUNCTION, EVENT;
				TokenType							SWITCH, THIS, BASE, NEW, VALUE, AS, IS, RESULT, TYPEOF;
				TokenType							EXIT, BREAK, CONTINUE, IF, ELSE, WHILE, DO, WHEN, FOR, TRY, CATCH, FINALLY, THROW, LOCK, CASE, DEFAULT, RETURN;
				TokenType							GLOBAL, NAMESPACE, USING, GENERIC, ENUM, CLASS, STRUCT, INTERFACE, GET, SET, IMPLICIT, EXPLICIT, CONSTRUCTOR, DESTRUCTOR, EXTERNAL;
				
				TokenType							IN, OUT, PARAMS, REF;
				TokenType							PUBLIC, PROTECTED, PRIVATE, INTERNAL;
				TokenType							SEALED, ABSTRACT, VIRTUAL, OVERRIDE;
				TokenType							STATIC, CONST, READONLY;

				/*--------OBJECTS--------*/

				TokenType							VCHAR, VSTRING, VINTEGER, VFLOAT, VNULL, VBOOLEAN, ID;

				/*--------SYMBOLS--------*/

				TokenType							DOT, COLON, SEMICOLON, COMMA, LAMBDA;
				TokenType							OPEQ;
				TokenType							LE, GE, EE, NE, EQ, LT, GT, QQ, QT;
				TokenType							ADD_SUB, MUL_DIV_MOD, NOT_BITNOT, INC_DEC;
				TokenType							AND, BITAND, OR, BITOR, XOR;
				TokenType							OPEN_DECL_BRACE, CLOSE_DECL_BRACE;
				TokenType							OPEN_ARRAY_BRACE, CLOSE_ARRAY_BRACE;
				TokenType							OPEN_EXP_BRACE, CLOSE_EXP_BRACE;

				/*--------RULES--------*/

				AccessorNode						accessor, setterAccessor;
				InheritationNode					inheritation;
				MemberTypeNode						memberType;
				DataTypeNode						dataType;
				ImplicitExplicitNode				implicitExplicit;
				InternalExternalNode				internalExternal;
				GenericArgconv						genericArgconv;
				FunctionArgconv						functionArgconv;
				InvokeArgconv						invokeArgconv;

				TypeNode							type, primitiveType, genericTypeConstraint;

				ArgumentNode						argument;
				LambdaParameterNode					lambdaUntypedParameter, lambdaParameter;
				PropertySetterNode					propertySetter;
				IncompleteExpressionNode			constant, primitiveExpression, exp0;
				ExpressionNode						expression;

				ForInitializerNode					forInitializer;
				CatchClauseNode						catchClause;
				CaseClauseNode						caseClause;
				IncompleteStatementNode				compositeStatement, lambdaBody;
				StatementNode						statement;

				GenericArgumentNode					genericArgument;
				GenericInfoNode						genericInfo;
				AttributeItemNode					attributeItem1, attributeItem2, attributeItem3;
				AttributeInfoNode					attributeInfo;
				EnumItemNode						enumItem;
				ParameterNode						parameter;
				MemberNode							member;
				DeclarationNode						declaration;

				UnitRule							unit;

			public:
				ManagedXParserImpl();

				static bool							Blank(vint token);
				Ptr<ManagedXUnit>					Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors);

				void								LoadLexer();
				void								LoadAttributes();
				void								LoadTypes();
				void								LoadStatements();
				void								LoadExpressions();
				void								LoadDeclarations();
			};
		}
	}
}

#endif