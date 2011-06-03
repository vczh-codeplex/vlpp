#include "ManagedXParser.h"
#include "ManagedXErrorMessage.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"
#include "..\..\..\Collections\Operation.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace regex;
			using namespace combinator;

/***********************************************************************
辅助函数
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

			Node<TokenInput<RegexToken>, RegexToken> CreateToken(List<WString>& tokens, const WString& token)
			{
				Node<TokenInput<RegexToken>, RegexToken> node=tk(tokens.Count());
				tokens.Add(token);
				return node;
			}

			wchar_t Escape(wchar_t input)
			{
				switch(input)
				{
				case L'r':
					return L'\r';
				case L'n':
					return L'\n';
				case L't':
					return L'\t';
				default:
					return input;
				}
			}

			wchar_t EscapeWalk(const wchar_t*& input)
			{
				if(*input==L'\\')
				{
					input++;
					return Escape(*input++);
				}
				else
				{
					return *input++;
				}
			}

			WString EscapeString(const WString& input)
			{
				wchar_t* buffer=new wchar_t[input.Length()+1];
				const wchar_t* reading=input.Buffer();
				wchar_t* writing=buffer;
				while(*reading)
				{
					*writing++=EscapeWalk(reading);
				}
				*writing=0;
				WString result=buffer;
				delete[] buffer;
				return result;
			}

			WString ConvertID(const WString& input)
			{
				if(input[0]==L'@')
				{
					if(input[1]==L'\"')
					{
						return EscapeString(input.Sub(2, input.Length()-3));
					}
					else
					{
						return input.Right(input.Length()-1);
					}
				}
				else
				{
					return input;
				}
			}

/***********************************************************************
Constants
***********************************************************************/

			Ptr<ManagedExpression> ToNull(const RegexToken& input)
			{
				Ptr<ManagedNullExpression> exp=CreateNode<ManagedNullExpression>(input);
				return exp;
			}

			Ptr<ManagedExpression> ToBoolean(const RegexToken& input)
			{
				Ptr<ManagedBooleanExpression> exp=CreateNode<ManagedBooleanExpression>(input);
				WString value(input.reading, input.length);
				exp->value=value==L"true";
				return exp;
			}

			Ptr<ManagedExpression> ToInteger(const RegexToken& input)
			{
				Ptr<ManagedIntegerExpression> exp=CreateNode<ManagedIntegerExpression>(input);
				WString value(input.reading, input.length);
				exp->sign=false;
				exp->value.unsignedInteger=wtou64(value);
				return exp;
			}

			Ptr<ManagedExpression> ToFloat(const RegexToken& input)
			{
				Ptr<ManagedFloatExpression> exp=CreateNode<ManagedFloatExpression>(input);
				WString value(input.reading, input.length);
				exp->value=wtof(value);
				return exp;
			}

			Ptr<ManagedExpression> ToChar(const RegexToken& input)
			{
				Ptr<ManagedCharExpression> exp=CreateNode<ManagedCharExpression>(input);
				WString value(input.reading+1, input.length-2);
				exp->value=EscapeString(value)[0];
				return exp;
			}

			Ptr<ManagedExpression> ToString(const RegexToken& input)
			{
				Ptr<ManagedStringExpression> exp=CreateNode<ManagedStringExpression>(input);
				WString value(input.reading+1, input.length-2);
				exp->value=EscapeString(value);
				return exp;
			}

/***********************************************************************
Basic Types
***********************************************************************/

			Ptr<ManagedType> ToKeywordType(const RegexToken& input)
			{
				WString name;
				WString keyword(input.reading, input.length);
				if(keyword==L"sbyte")			name = L"SInt8";
				else if(keyword==L"byte")		name = L"UInt8";
				else if(keyword==L"short")		name = L"SInt16";
				else if(keyword==L"word")		name = L"UInt16";
				else if(keyword==L"int")		name = L"SInt32";
				else if(keyword==L"uint")		name = L"UInt32";
				else if(keyword==L"long")		name = L"SInt64";
				else if(keyword==L"ulong")		name = L"UInt64";
				else if(keyword==L"char")		name = L"Char";
				else if(keyword==L"string")		name = L"String";
				else if(keyword==L"float")		name = L"Single";
				else if(keyword==L"double")		name = L"Double";
				else if(keyword==L"bool")		name = L"Bool";
				else if(keyword==L"object")		name = L"Object";
				else if(keyword==L"void")		name = L"Void";
#ifdef VCZH_64
				else if(keyword==L"intptr")		name = L"SInt64";
				else if(keyword==L"uintptr")	name = L"UInt64";
#else
				else if(keyword==L"intptr")		name = L"SInt32";
				else if(keyword==L"uintptr")	name = L"UInt32";
#endif

				Ptr<ManagedMemberType> system=CreateNode<ManagedMemberType>(input);
				system->member=L"System";

				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->operand=system;
				type->member=name;
				return type;
			}

			Ptr<ManagedType> ToReferenceType(const RegexToken& input)
			{
				Ptr<ManagedReferencedType> type=CreateNode<ManagedReferencedType>(input);
				type->name=ConvertID(WString(input.reading, input.length));
				return type;
			}

			Ptr<ManagedType> ToGlobalType(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=WString(input.reading, input.length);
				return type;
			}

			Ptr<ManagedType> ToLrecType(const Ptr<ManagedType>& elementType, const Ptr<ManagedType>& decoratorType)
			{
				if(Ptr<ManagedMemberType> memberType=decoratorType.Cast<ManagedMemberType>())
				{
					memberType->operand=elementType;
				}
				else if(Ptr<ManagedInstantiatedGenericType> genericType=decoratorType.Cast<ManagedInstantiatedGenericType>())
				{
					genericType->elementType=elementType;
				}
				else if(Ptr<ManagedArrayType> arrayType=decoratorType.Cast<ManagedArrayType>())
				{
					arrayType->elementType=elementType;
				}
				else
				{
					CHECK_ERROR(false, L"ToLrecType(Ptr<ManagedType>, Ptr<ManagedType>)#未知类型。");
				}
				return decoratorType;
			}

			Ptr<ManagedType> ToMemberTypeLrec(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=ConvertID(WString(input.reading, input.length));
				return type;
			}

			Ptr<ManagedType> ToInstantiatedGenericType(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedInstantiatedGenericType> type=CreateNode<ManagedInstantiatedGenericType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->argumentTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

/***********************************************************************
Extended Types
***********************************************************************/

			Ptr<ManagedType> ToArrayType(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedArrayType> type=CreateNode<ManagedArrayType>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				int count=1;
				while(current)
				{
					count++;
					current=current->Next();
				}
				type->dimensionCount=count;
				return type;
			}

			Ptr<ManagedType> ToFunctionType(const ParsingPair<ParsingPair<RegexToken, Ptr<ManagedType>>, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedFunctionType> type=CreateNode<ManagedFunctionType>(input.First().First());
				type->returnType=input.First().Second();
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToEventType(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedEventType> type=CreateNode<ManagedEventType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToAutoReferType(const RegexToken& input)
			{
				return CreateNode<ManagedAutoReferType>(input);
			}

			Ptr<ManagedType> ToDynamicType(const RegexToken& input)
			{
				return CreateNode<ManagedDynamicType>(input);
			}

/***********************************************************************
Basic Expressions
***********************************************************************/

			Ptr<ManagedExpression> ToReferenceExpression(const RegexToken& input)
			{
				Ptr<ManagedReferenceExpression> exp=CreateNode<ManagedReferenceExpression>(input);
				exp->name=ConvertID(WString(input.reading, input.length));
				return exp;
			}

/***********************************************************************
Extended Expressions
***********************************************************************/

/***********************************************************************
Basic Statements
***********************************************************************/

/***********************************************************************
Extended Statements
***********************************************************************/

/***********************************************************************
Basic Declaration Fragments
***********************************************************************/

			Ptr<ManagedGenericInfo::Argument> ToGenericArgument(const ParsingPair<
						ParsingPair<ManagedGenericInfo::ArgumentConversion, RegexToken>,
						ParsingList<ParsingList<Ptr<ManagedType>>>
					>& input)
			{
				Ptr<ManagedGenericInfo::Argument> argument=new ManagedGenericInfo::Argument;
				argument->newConstraint=false;
				argument->conversion=input.First().First();
				argument->name=WString(input.First().Second().reading, input.First().Second().length);
				if(input.Second().Head())
				{
					Ptr<ParsingList<Ptr<ManagedType>>::Node> type=input.Second().Head()->Value().Head();
					while(type)
					{
						if(type->Value())
						{
							argument->typeConstraints.Add(type->Value());
						}
						else
						{
							argument->newConstraint=true;
						}
						type=type->Next();
					}
				}
				return argument;
			}

			Ptr<ManagedGenericInfo> ToGenericInfo(const ParsingList<Ptr<ManagedGenericInfo::Argument>>& input)
			{
				Ptr<ManagedGenericInfo> info=new ManagedGenericInfo;
				Ptr<ParsingList<Ptr<ManagedGenericInfo::Argument>>::Node> current=input.Head();
				while(current)
				{
					info->arguments.Add(current->Value());
					current=current->Next();
				}
				return info;
			}

/***********************************************************************
Basic Declaration Members
***********************************************************************/

/***********************************************************************
Extended Declaration Members
***********************************************************************/

/***********************************************************************
Basic Declarations
***********************************************************************/

			Ptr<ManagedDeclaration> ToNamespaceDecl(const ParsingPair<ParsingPair<RegexToken, ParsingList<RegexToken>>, ParsingList<Ptr<ManagedDeclaration>>>& input)
			{
				Ptr<ManagedNamespaceDeclaration> decl=CreateNode<ManagedNamespaceDeclaration>(input.First().First());
				{
					Ptr<ParsingList<RegexToken>::Node> current=input.First().Second().Head();
					while(current)
					{
						decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
						current=current->Next();
					}
				}
				{
					Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Second().Head();
					while(current)
					{
						decl->declarations.Add(current->Value());
						current=current->Next();
					}
				}
				return decl;
			}

/***********************************************************************
Extended Declarations
***********************************************************************/

			Ptr<ManagedEnumItem> ToEnumItem(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedEnumItem> item=CreateNode<ManagedEnumItem>(input.First());
				item->name=ConvertID(WString(input.First().reading, input.First().length));
				if(input.Second().Head())
				{
					item->value=input.Second().Head()->Value();
				}
				return item;
			}

			Ptr<ManagedDeclaration> ToEnum(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				declatt::Accessor,
				RegexToken>,
				ParsingList<RegexToken>>,
				RegexToken>,
				ParsingList<Ptr<ManagedEnumItem>>>& input)
			{
				Ptr<ManagedEnumerationDeclaration> decl=CreateNode<ManagedEnumerationDeclaration>(input.First().First().First().Second());
				decl->accessor=input.First().First().First().First();
				decl->composable=input.First().First().Second().Head();
				decl->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));

				Ptr<ParsingList<Ptr<ManagedEnumItem>>::Node> current=input.Second().Head();
				while(current)
				{
					decl->items.Add(current->Value());
					current=current->Next();
				}
				return decl;
			}

			Ptr<ManagedDeclaration> ToTypeRenameDecl(const ParsingPair<ParsingPair<ParsingPair<
				ParsingList<Ptr<ManagedGenericInfo>>, 
				declatt::Accessor>, 
				RegexToken>, 
				Ptr<ManagedType>>& input)
			{
				Ptr<ManagedTypeRenameDeclaration> decl=CreateNode<ManagedTypeRenameDeclaration>(input.First().Second());
				if(input.First().First().First().Head())
				{
					Ptr<ManagedGenericInfo> genericInfo=input.First().First().First().Head()->Value();
					CopyFrom(decl->genericInfo.arguments.Wrap(), genericInfo->arguments.Wrap());
				}
				decl->accessor=input.First().First().Second();
				decl->name=WString(input.First().Second().reading, input.First().Second().length);
				decl->type=input.Second();
				return decl;
			}

			Ptr<ManagedDeclaration> ToUsingNamespaceDecl(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedUsingNamespaceDeclaration> decl=CreateNode<ManagedUsingNamespaceDeclaration>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				while(current)
				{
					decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
					current=current->Next();
				}
				return decl;
			}

/***********************************************************************
Unit
***********************************************************************/

			Ptr<ManagedXUnit> ToUnit(const ParsingList<Ptr<ManagedDeclaration>>& input)
			{
				Ptr<ManagedProgram> program=new ManagedProgram;
				Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Head();
				while(current)
				{
					program->declarations.Add(current->Value());
					current=current->Next();
				}
				
				Ptr<ManagedXUnit> unit=new ManagedXUnit;
				unit->program=program;
				unit->codeIndex=-1;
				return unit;
			}

/***********************************************************************
Error Handlers
***********************************************************************/

#define ERROR_HANDLER(NAME, TYPE)																						\
			ParsingResult<TYPE> NAME(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& info)	\
			{																											\
				info.errors.Clear();																					\
				info.errors.Add(new CombinatorError<TokenInput<RegexToken>>(ManagedXErrorMessage::NAME(), input));		\
				return ParsingResult<TYPE>();																			\
			}
			
			ERROR_HANDLER(NeedId,							RegexToken)
				
			ERROR_HANDLER(NeedColon,						RegexToken)
			ERROR_HANDLER(NeedSemicolon,					RegexToken)
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

/***********************************************************************
语法分析器
***********************************************************************/

			typedef Node<TokenInput<RegexToken>, RegexToken>									TokenType;
			typedef Node<TokenInput<RegexToken>, declatt::Accessor>								AccessorNode;
			typedef Node<TokenInput<RegexToken>, declatt::Inheritation>							InheritationNode;
			typedef Node<TokenInput<RegexToken>, declatt::MemberType>							MemberTypeNode;
			typedef Node<TokenInput<RegexToken>, declatt::DataType>								DataTypeNode;
			typedef Node<TokenInput<RegexToken>, ManagedGenericInfo::ArgumentConversion>		GenericArgconv;
			typedef Node<TokenInput<RegexToken>, ManagedParameter::ParameterType>				FunctionArgconv;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedType>>								TypeNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedExpression>>						ExpressionNode;

			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo::Argument>>				GenericArgumentNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo>>						GenericInfoNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedEnumItem>>							EnumItemNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedDeclaration>>						DeclarationNode;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedXUnit>>								UnitRule;

			class ManagedXParserImpl : public ManagedXParser
			{
			protected:
				Ptr<RegexLexer>						lexer;

				/*--------KEYWORDS--------*/

				TokenType							SBYTE, BYTE, SHORT, WORD, INT, UINT, LONG, ULONG, CHAR, STRING, FLOAT, DOUBLE, BOOL, OBJECT, VOID, INTPTR, UINTPTR, VAR, DYNAMIC, FUNCTION, EVENT;

				TokenType							SWITCH;

				TokenType							IN, OUT, PARAMS, REF, NEW;
				
				TokenType							GLOBAL, NAMESPACE, USING, GENERIC, ENUM;

				TokenType							PUBLIC, PROTECTED, PRIVATE, INTERNAL;
				TokenType							SEALED, ABSTRACT, VIRTUAL, OVERRIDE;
				TokenType							STATIC, CONST, READONLY;

				/*--------OBJECTS--------*/

				TokenType							VCHAR, VSTRING, VINTEGER, VFLOAT, VNULL, VBOOLEAN, ID;

				/*--------SYMBOLS--------*/

				TokenType							DOT, COLON, SEMICOLON, COMMA;
				TokenType							EQ, LT, GT;
				TokenType							OPEN_DECL_BRACE, CLOSE_DECL_BRACE;
				TokenType							OPEN_ARRAY_BRACE, CLOSE_ARRAY_BRACE;
				TokenType							OPEN_EXP_BRACE, CLOSE_EXP_BRACE;

				/*--------RULES--------*/

				AccessorNode						accessor;
				InheritationNode					inheritation;
				MemberTypeNode						memberType;
				DataTypeNode						dataType;
				GenericArgconv						genericArgconv;
				FunctionArgconv						functionArgconv;

				TypeNode							type, primitiveType, genericTypeConstraint;

				ExpressionNode						constant, primitiveExpression, expression;

				GenericArgumentNode					genericArgument;
				GenericInfoNode						genericInfo;
				EnumItemNode						enumItem;
				DeclarationNode						declaration;

				UnitRule							unit;

			public:
				ManagedXParserImpl()
				{
					List<WString> tokens;
					tokens.Add(L"/s+");
					tokens.Add(L"////[^\r\n]*(\r\n|\n)?");
					tokens.Add(L"///*([^*//]+|/*+[^//])*/*+//");

					/*--------KEYWORDS--------*/

					SBYTE					= CreateToken(tokens, L"sbyte");
					BYTE					= CreateToken(tokens, L"byte");
					SHORT					= CreateToken(tokens, L"short");
					WORD					= CreateToken(tokens, L"word");
					INT						= CreateToken(tokens, L"int");
					UINT					= CreateToken(tokens, L"uint");
					LONG					= CreateToken(tokens, L"long");
					ULONG					= CreateToken(tokens, L"ulong");
					CHAR					= CreateToken(tokens, L"char");
					STRING					= CreateToken(tokens, L"string");
					FLOAT					= CreateToken(tokens, L"float");
					DOUBLE					= CreateToken(tokens, L"double");
					BOOL					= CreateToken(tokens, L"bool");
					OBJECT					= CreateToken(tokens, L"object");
					VOID					= CreateToken(tokens, L"void");
					INTPTR					= CreateToken(tokens, L"intptr");
					UINTPTR					= CreateToken(tokens, L"uintptr");
					VAR						= CreateToken(tokens, L"var");
					DYNAMIC					= CreateToken(tokens, L"dynamic");
					FUNCTION				= CreateToken(tokens, L"function");
					EVENT					= CreateToken(tokens, L"event");
					
					SWITCH					= CreateToken(tokens, L"switch");

					IN						= CreateToken(tokens, L"in");
					OUT						= CreateToken(tokens, L"out");
					PARAMS					= CreateToken(tokens, L"params");
					REF						= CreateToken(tokens, L"ref");
					NEW						= CreateToken(tokens, L"new");
										
					GLOBAL					= CreateToken(tokens, L"global");
					NAMESPACE				= CreateToken(tokens, L"namespace");
					USING					= CreateToken(tokens, L"using");
					GENERIC					= CreateToken(tokens, L"generic");
					ENUM					= CreateToken(tokens, L"enum");

					PUBLIC					= CreateToken(tokens, L"public");
					PROTECTED				= CreateToken(tokens, L"protected");
					PRIVATE					= CreateToken(tokens, L"private");
					INTERNAL				= CreateToken(tokens, L"internal");
					SEALED					= CreateToken(tokens, L"sealed");
					ABSTRACT				= CreateToken(tokens, L"abstract");
					VIRTUAL					= CreateToken(tokens, L"virtual");
					OVERRIDE				= CreateToken(tokens, L"override");
					STATIC					= CreateToken(tokens, L"static");
					CONST					= CreateToken(tokens, L"const");
					READONLY				= CreateToken(tokens, L"readonly");

					/*--------OBJECTS--------*/
					VCHAR					= CreateToken(tokens, L"\'([^\\\\\']|\\\\\\.)\'");
					VSTRING					= CreateToken(tokens, L"\"([^\\\\\"]|\\\\\\.)*\"");
					VFLOAT					= CreateToken(tokens, L"/d+./d+");
					VINTEGER				= CreateToken(tokens, L"/d+");
					VNULL					= CreateToken(tokens, L"null");
					VBOOLEAN				= CreateToken(tokens, L"true|false");
					ID						= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");

					/*--------SYMBOLS--------*/

					DOT						= CreateToken(tokens, L".");
					COLON					= CreateToken(tokens, L":");
					SEMICOLON				= CreateToken(tokens, L";");
					COMMA					= CreateToken(tokens, L",");
					EQ						= CreateToken(tokens, L"=");
					LT						= CreateToken(tokens, L"<");
					GT						= CreateToken(tokens, L">");
					OPEN_DECL_BRACE			= CreateToken(tokens, L"/{");
					CLOSE_DECL_BRACE		= CreateToken(tokens, L"/}");
					OPEN_ARRAY_BRACE		= CreateToken(tokens, L"/[");
					CLOSE_ARRAY_BRACE		= CreateToken(tokens, L"/]");
					OPEN_EXP_BRACE			= CreateToken(tokens, L"/(");
					CLOSE_EXP_BRACE			= CreateToken(tokens, L"/)");

					/*--------LEXICAL ANALYZER--------*/

					lexer=new RegexLexer(tokens.Wrap());

					/*--------ATTRIBUTES--------*/

					accessor				= def(	let(PUBLIC, declatt::Public)
												|	let(PROTECTED+INTERNAL, declatt::ProtectedInternal)
												|	let(PROTECTED, declatt::Protected)
												|	let(PRIVATE, declatt::Private)
												|	let(INTERNAL, declatt::Internal)
												,	declatt::Private);

					inheritation			= def(	let(SEALED, declatt::Sealed)
												|	let(ABSTRACT, declatt::Abstract)
												|	let(VIRTUAL, declatt::Virtual)
												|	let(OVERRIDE, declatt::Override)
												,	declatt::Normal);

					memberType				= def(let(STATIC, declatt::Static), declatt::Instance);

					dataType				= def(	let(CONST, declatt::Constant)
												|	let(READONLY, declatt::Readonly)
												,	declatt::Variable);

					genericArgconv			= def(	let(IN, ManagedGenericInfo::In)
												|	let(OUT, ManagedGenericInfo::Out)
												,	ManagedGenericInfo::None);

					functionArgconv			= def(	let(PARAMS, ManagedParameter::Params)
												|	let(REF, ManagedParameter::Ref)
												|	let(OUT, ManagedParameter::Out)
												,	ManagedParameter::Normal);

					/*--------TYPES--------*/

					primitiveType			= (SBYTE|BYTE|SHORT|WORD|INT|UINT|LONG|ULONG|CHAR|STRING|FLOAT|DOUBLE|BOOL|OBJECT|VOID|INTPTR|UINTPTR)[ToKeywordType]
											| ID(NeedId)[ToReferenceType]
											| VAR[ToAutoReferType]
											| DYNAMIC[ToDynamicType]
											| (FUNCTION + type + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToFunctionType]
											| (EVENT + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToEventType]
											| ((GLOBAL + COLON(NeedColon) + COLON(NeedColon)) >> ID(NeedId))[ToGlobalType]
											;
					type					= lrec(primitiveType + *( (DOT >> ID)[ToMemberTypeLrec]
																	| ((LT + plist(type + *(COMMA >> type))) << GT(NeedGt))[ToInstantiatedGenericType]
																	| ((OPEN_ARRAY_BRACE + *COMMA) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))[ToArrayType]
																	), ToLrecType);

					/*--------EXPRESSIONS--------*/

					constant				= VNULL[ToNull] | VBOOLEAN[ToBoolean] | VINTEGER[ToInteger] | VFLOAT[ToFloat] | VCHAR[ToChar] | VSTRING[ToString];
					
					primitiveExpression		= constant
											| ID(NeedExpression)[ToReferenceExpression]
											;

					expression				= primitiveExpression;

					/*--------DECLARATION FRAGMENTS--------*/

					genericTypeConstraint	= type | let(NEW + OPEN_EXP_BRACE(NeedOpenExpBrace) + CLOSE_EXP_BRACE(NeedCloseExpBrace), Ptr<ManagedType>());

					genericArgument			= ((genericArgconv + ID(NeedId)) + opt(COLON(NeedColon) >> (
												OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(genericTypeConstraint + *(COMMA >> genericTypeConstraint)) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
											)))[ToGenericArgument];

					genericInfo				= (GENERIC + LT(NeedLt) >> (
												plist(genericArgument + *(COMMA >> genericArgument))
											) << GT(NeedGt))[ToGenericInfo];

					/*--------DECLARATIONS--------*/

					enumItem				= (ID(NeedId) + opt(EQ >> expression))[ToEnumItem];

					declaration				= ((USING + plist(ID(NeedId) + *(DOT >> ID(NeedId))) << SEMICOLON(NeedSemicolon)))[ToUsingNamespaceDecl]
											| ((opt(genericInfo) + accessor + (USING >> ID(NeedId)) + (EQ(NeedEq) >> type)) << SEMICOLON(NeedSemicolon))[ToTypeRenameDecl]
											| (NAMESPACE + plist(ID(NeedId) + *(DOT >> ID(NeedId))) + (OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *declaration << CLOSE_DECL_BRACE(NeedCloseDeclBrace)))[ToNamespaceDecl]
											| (accessor + ENUM + opt(SWITCH) + ID + (
												OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(opt(enumItem + *(COMMA >> enumItem))) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
												))[ToEnum]
											;
					unit					= (*declaration)[ToUnit];
				}

				static bool Blank(vint token)
				{
					return token<3;
				}

				Ptr<ManagedXUnit> Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)
				{
					List<RegexToken> tokens;
					lexer->Parse(code, codeIndex).ReadToEnd(tokens, Blank);
					for(vint i=0;i<tokens.Count();i++)
					{
						if(tokens[i].token==-1)
						{
							WString message=ManagedXErrorMessage::UnrecognizedToken(WString(tokens[i].reading, tokens[i].length));
							errors.Add(new LanguageException(message, tokens[i].lineIndex, tokens[i].lineStart, tokens[i].start, codeIndex));
							return 0;
						}
					}

					TokenInput<RegexToken> input(&tokens[0], tokens.Count());
					try
					{
						Ptr<ManagedXUnit> result=unit.ParseFull(input, false);
						if(result)
						{
							result->codeIndex=codeIndex;
						}
						return result;
					}
					catch(const CombinatorException<TokenInput<RegexToken>>& exception)
					{
						for(vint i=0;i<exception.GetGlobalInfo().errors.Count();i++)
						{
							Ptr<CombinatorError<TokenInput<RegexToken>>> error=exception.GetGlobalInfo().errors.Get(i);
							RegexToken position=error->GetPosition().Current();
							errors.Add(new LanguageException(error->Message(), position.lineIndex, position.lineStart, position.start, codeIndex));
						}
						return 0;
					}
				}
			};

			Ptr<ManagedXParser> ManagedXParser::Create()
			{
				return new ManagedXParserImpl;
			}
		}
	}
}