#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			bool ManagedXParserImpl::Blank(vint token)
			{
				return token<3;
			}

			void ManagedXParserImpl::LoadLexer()
			{
				List<WString> tokens;
				tokens.Add(L"/s+");
				tokens.Add(L"////[^\r\n]*(\r\n|\n)?");
				tokens.Add(L"///*([^*//]+|/*+[^//])*/*+//");

				/*--------KEYWORDS--------*/

				TYPEKEYWORD				= CreateToken(tokens, L"sbyte|byte|short|word|int|uint|long|ulong|char|string|float|double|bool|object|void|intptr|uintptr");
				VAR						= CreateToken(tokens, L"var");
				DYNAMIC					= CreateToken(tokens, L"dynamic");
				FUNCTION				= CreateToken(tokens, L"function");
				EVENT					= CreateToken(tokens, L"event");
					
				SWITCH					= CreateToken(tokens, L"switch");
				THIS					= CreateToken(tokens, L"this");
				BASE					= CreateToken(tokens, L"base");
				NEW						= CreateToken(tokens, L"new");
				VALUE					= CreateToken(tokens, L"value");
				AS						= CreateToken(tokens, L"as");
				IS						= CreateToken(tokens, L"is");
				RESULT					= CreateToken(tokens, L"result");
				TYPEOF					= CreateToken(tokens, L"typeof");

				EXIT					= CreateToken(tokens, L"exit");
				BREAK					= CreateToken(tokens, L"break");
				CONTINUE				= CreateToken(tokens, L"continue");
				IF						= CreateToken(tokens, L"if");
				ELSE					= CreateToken(tokens, L"else");
				WHILE					= CreateToken(tokens, L"while");
				DO						= CreateToken(tokens, L"do");
				WHEN					= CreateToken(tokens, L"when");
				FOR						= CreateToken(tokens, L"for");
				TRY						= CreateToken(tokens, L"try");
				CATCH					= CreateToken(tokens, L"catch");
				FINALLY					= CreateToken(tokens, L"finally");
				THROW					= CreateToken(tokens, L"throw");
				LOCK					= CreateToken(tokens, L"lock");
				CASE					= CreateToken(tokens, L"case");
				DEFAULT					= CreateToken(tokens, L"default");
				RETURN					= CreateToken(tokens, L"return");
										
				GLOBAL					= CreateToken(tokens, L"global");
				NAMESPACE				= CreateToken(tokens, L"namespace");
				USING					= CreateToken(tokens, L"using");
				GENERIC					= CreateToken(tokens, L"generic");
				ENUM					= CreateToken(tokens, L"enum");
				CLASS					= CreateToken(tokens, L"class");
				STRUCT					= CreateToken(tokens, L"struct");
				INTERFACE				= CreateToken(tokens, L"interface");
				GET						= CreateToken(tokens, L"get");
				SET						= CreateToken(tokens, L"set");
				IMPLICIT				= CreateToken(tokens, L"implicit");
				EXPLICIT				= CreateToken(tokens, L"explicit");
				CONSTRUCTOR				= CreateToken(tokens, L"constructor");
				DESTRUCTOR				= CreateToken(tokens, L"destructor");
					

				IN						= CreateToken(tokens, L"in");
				OUT						= CreateToken(tokens, L"out");
				PARAMS					= CreateToken(tokens, L"params");
				REF						= CreateToken(tokens, L"ref");
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
				LAMBDA					= CreateToken(tokens, L"=>");
					
				OPEQ					= CreateToken(tokens, L"(/+|-|/*|//|%|&&|&|/|/||/||/^|<<|>>)=");

				LE						= CreateToken(tokens, L">=");
				GE						= CreateToken(tokens, L"<=");
				EE						= CreateToken(tokens, L"==");
				NE						= CreateToken(tokens, L"!=");
				EQ						= CreateToken(tokens, L"=");
				LT						= CreateToken(tokens, L"<");
				GT						= CreateToken(tokens, L">");
				QQ						= CreateToken(tokens, L"/?/?");
				QT						= CreateToken(tokens, L"/?");
					
				INC_DEC					= CreateToken(tokens, L"/+/+|--");
				NOT_BITNOT				= CreateToken(tokens, L"!|~");
				ADD_SUB					= CreateToken(tokens, L"/+|-");
				MUL_DIV_MOD				= CreateToken(tokens, L"/*|//|%");

				AND						= CreateToken(tokens, L"&&");
				BITAND					= CreateToken(tokens, L"&");
				OR						= CreateToken(tokens, L"/|/|");
				BITOR					= CreateToken(tokens, L"/|");
				XOR						= CreateToken(tokens, L"/^");

				OPEN_DECL_BRACE			= CreateToken(tokens, L"/{");
				CLOSE_DECL_BRACE		= CreateToken(tokens, L"/}");
				OPEN_ARRAY_BRACE		= CreateToken(tokens, L"/[");
				CLOSE_ARRAY_BRACE		= CreateToken(tokens, L"/]");
				OPEN_EXP_BRACE			= CreateToken(tokens, L"/(");
				CLOSE_EXP_BRACE			= CreateToken(tokens, L"/)");

				/*--------LEXICAL ANALYZER--------*/

				lexer=new RegexLexer(tokens.Wrap());
			}
		}
	}
}