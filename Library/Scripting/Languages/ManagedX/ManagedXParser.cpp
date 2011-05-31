#include "ManagedXParser.h"
#include "ManagedXErrorMessage.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"

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
				type->name=WString(input.reading, input.length);
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
					return memberType;
				}
				else
				{
					CHECK_ERROR(false, L"ToLrecType(Ptr<ManagedType>, Ptr<ManagedType>)#未知类型。");
				}
			}

			Ptr<ManagedType> ToMemberTypeLrec(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=WString(input.reading, input.length);
				return type;
			}

/***********************************************************************
Extended Types
***********************************************************************/

/***********************************************************************
Basic Expressions
***********************************************************************/

/***********************************************************************
Extended Expressions
***********************************************************************/

/***********************************************************************
Basic Declaration Fragments
***********************************************************************/

/***********************************************************************
Basic Statements
***********************************************************************/

/***********************************************************************
Extended Statements
***********************************************************************/

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

			Ptr<ManagedDeclaration> ToTypeRenameDecl(const ParsingPair<ParsingPair<declatt::Accessor, RegexToken>, Ptr<ManagedType>>& input)
			{
				Ptr<ManagedTypeRenameDeclaration> decl=CreateNode<ManagedTypeRenameDeclaration>(input.First().Second());
				decl->accessor=input.First().First();
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
			ERROR_HANDLER(NeedOpenDeclBrace,				RegexToken)
			ERROR_HANDLER(NeedCloseDeclBrace,				RegexToken)

/***********************************************************************
语法分析器
***********************************************************************/

			typedef Node<TokenInput<RegexToken>, RegexToken>					TokenType;
			typedef Node<TokenInput<RegexToken>, declatt::Accessor>				AccessorNode;
			typedef Node<TokenInput<RegexToken>, declatt::Inheritation>			InheritationNode;
			typedef Node<TokenInput<RegexToken>, declatt::MemberType>			MemberTypeNode;
			typedef Node<TokenInput<RegexToken>, declatt::DataType>				DataTypeNode;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedType>>				TypeNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedDeclaration>>		DeclarationNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedXUnit>>				UnitRule;

			class ManagedXParserImpl : public ManagedXParser
			{
			protected:
				Ptr<RegexLexer>						lexer;

				/*--------KEYWORDS--------*/

				TokenType							SBYTE, BYTE, SHORT, WORD, INT, UINT, LONG, ULONG, CHAR, STRING, FLOAT, DOUBLE, BOOL, OBJECT, VOID, INTPTR, UINTPTR;
				
				TokenType							GLOBAL;
				TokenType							NAMESPACE;
				TokenType							USING;

				TokenType							PUBLIC, PROTECTED, PRIVATE, INTERNAL;
				TokenType							SEALED, ABSTRACT, VIRTUAL, OVERRIDE;
				TokenType							STATIC, CONST, READONLY;

				/*--------OBJECTS--------*/

				TokenType							ID;

				/*--------SYMBOLS--------*/

				TokenType							DOT;
				TokenType							COLON;
				TokenType							SEMICOLON;
				TokenType							EQ;
				TokenType							OPEN_DECL_BRACE;
				TokenType							CLOSE_DECL_BRACE;

				/*--------RULES--------*/

				AccessorNode						accessor;
				InheritationNode					inheritation;
				MemberTypeNode						memberType;
				DataTypeNode						dataType;

				TypeNode							type, primitiveType;

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

					SBYTE				= CreateToken(tokens, L"sbyte");
					BYTE				= CreateToken(tokens, L"byte");
					SHORT				= CreateToken(tokens, L"short");
					WORD				= CreateToken(tokens, L"word");
					INT					= CreateToken(tokens, L"int");
					UINT				= CreateToken(tokens, L"uint");
					LONG				= CreateToken(tokens, L"long");
					ULONG				= CreateToken(tokens, L"ulong");
					CHAR				= CreateToken(tokens, L"char");
					STRING				= CreateToken(tokens, L"string");
					FLOAT				= CreateToken(tokens, L"float");
					DOUBLE				= CreateToken(tokens, L"double");
					BOOL				= CreateToken(tokens, L"bool");
					OBJECT				= CreateToken(tokens, L"object");
					VOID				= CreateToken(tokens, L"void");
					INTPTR				= CreateToken(tokens, L"intptr");
					UINTPTR				= CreateToken(tokens, L"uintptr");
										
					GLOBAL				= CreateToken(tokens, L"global");
					NAMESPACE			= CreateToken(tokens, L"namespace");
					USING				= CreateToken(tokens, L"using");

					PUBLIC				= CreateToken(tokens, L"public");
					PROTECTED			= CreateToken(tokens, L"protected");
					PRIVATE				= CreateToken(tokens, L"private");
					INTERNAL			= CreateToken(tokens, L"internal");
					SEALED				= CreateToken(tokens, L"sealed");
					ABSTRACT			= CreateToken(tokens, L"abstract");
					VIRTUAL				= CreateToken(tokens, L"virtual");
					OVERRIDE			= CreateToken(tokens, L"override");
					STATIC				= CreateToken(tokens, L"static");
					CONST				= CreateToken(tokens, L"const");
					READONLY			= CreateToken(tokens, L"readonly");

					/*--------OBJECTS--------*/

					ID					= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");

					/*--------SYMBOLS--------*/

					DOT					= CreateToken(tokens, L".");
					COLON				= CreateToken(tokens, L":");
					SEMICOLON			= CreateToken(tokens, L";");
					EQ					= CreateToken(tokens, L"=");
					OPEN_DECL_BRACE		= CreateToken(tokens, L"/{");
					CLOSE_DECL_BRACE	= CreateToken(tokens, L"/}");

					/*--------LEXICAL ANALYZER--------*/

					lexer=new RegexLexer(tokens.Wrap());

					/*--------ATTRIBUTES--------*/

					accessor			= def(	let(PUBLIC, declatt::Public)
											|	let(PROTECTED+INTERNAL, declatt::ProtectedInternal)
											|	let(PROTECTED, declatt::Protected)
											|	let(PRIVATE, declatt::Private)
											|	let(INTERNAL, declatt::Internal)
											,	declatt::Private);

					inheritation		= def(	let(SEALED, declatt::Sealed)
											|	let(ABSTRACT, declatt::Abstract)
											|	let(VIRTUAL, declatt::Virtual)
											|	let(OVERRIDE, declatt::Override)
											,	declatt::Normal);

					memberType			= def(let(STATIC, declatt::Static), declatt::Instance);

					dataType			= def(	let(CONST, declatt::Constant)
											|	let(READONLY, declatt::Readonly)
											,	declatt::Variable);

					/*--------SYNTACTICAL ANALYZER--------*/

					primitiveType		= (SBYTE|BYTE|SHORT|WORD|INT|UINT|LONG|ULONG|CHAR|STRING|FLOAT|DOUBLE|BOOL|OBJECT|VOID|INTPTR|UINTPTR)[ToKeywordType]
										| ID[ToReferenceType]
										| ((GLOBAL + COLON(NeedColon) + COLON(NeedColon)) >> ID(NeedId))[ToGlobalType]
										;
					type				= lrec(primitiveType + *( (DOT >> ID)[ToMemberTypeLrec]
																), ToLrecType);

					declaration			= ((USING + plist(ID(NeedId) + *(DOT >> ID)) << SEMICOLON(NeedSemicolon)))[ToUsingNamespaceDecl]
										| ((accessor + (USING >> ID(NeedId)) + (EQ(NeedEq) >> type)) << SEMICOLON(NeedSemicolon))[ToTypeRenameDecl]
										| (NAMESPACE + plist(ID(NeedId) + *(DOT >> ID)) + (OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *declaration << CLOSE_DECL_BRACE(NeedCloseDeclBrace)))[ToNamespaceDecl]
										;
					unit				= (*declaration)[ToUnit];
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