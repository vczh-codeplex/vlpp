#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

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
				if(input.Length()>=1 && input[0]==L'@')
				{
					if(input.Length()>=3 && input[1]==L'\"')
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
ManagedXParserImpl
***********************************************************************/

			ManagedXParserImpl::ManagedXParserImpl()
			{
				/*--------LOAD RULES--------*/

				LoadLexer();
				LoadAttributes();
				LoadTypes();
				LoadStatements();
				LoadExpressions();
				LoadDeclarations();
			}

			Ptr<ManagedXUnit> ManagedXParserImpl::Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)
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

			Ptr<ManagedXParser> ManagedXParser::Create()
			{
				return new ManagedXParserImpl;
			}
		}
	}
}