#include <math.h>
#include "..\..\..\..\Library\Console.h"
#include "..\..\..\..\Library\Combinator\TokenCombinator.h"
#include "..\..\..\..\Library\Collections\Operation.h"

using namespace vl;
using namespace vl::console;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::combinator;

double Convert(const RegexToken& input)
{
	return wtof(WString(input.reading, input.length));
}

double Negative(const double& input)
{
	return -input;
}

double Operator(const double& left, const ParsingPair<RegexToken, double>& right)
{
	switch(*right.First().reading)
	{
	case L'+':
		return left+right.Second();
	case L'-':
		return left-right.Second();
	case L'*':
		return left*right.Second();
	case L'/':
		return left/right.Second();
	default:
		return 0;
	}
}

double Call(const ParsingPair<RegexToken, double>& input)
{
	WString name(input.First().reading, input.First().length);
	double parameter=input.Second();

	if(name==L"sin")
	{
		return sin(parameter);
	}
	else if(name==L"cos")
	{
		return cos(parameter);
	}
	else if(name==L"tan")
	{
		return tan(parameter);
	}
	else if(name==L"cot")
	{
		return 1/tan(parameter);
	}
	else if(name==L"sec")
	{
		return 1/cos(parameter);
	}
	else if(name==L"csc")
	{
		return 1/sin(parameter);
	}
	else if(name==L"exp")
	{
		return exp(parameter);
	}
	else if(name==L"ln")
	{
		return log(parameter);
	}
	else if(name==L"abs")
	{
		return abs(parameter);
	}
	else if(name==L"sqrt")
	{
		return sqrt(parameter);
	}
	else if(name==L"sqr")
	{
		return parameter*parameter;
	}
	else
	{
		throw Exception(L"Function "+name+L" not exists.");
	}
}

bool IsNotBlank(RegexToken token)
{
	return token.token>0;
}

int wmain(int argc, wchar_t* argv[])
{
	List<WString> patterns;
	const int BLANK		= patterns.Add(L"/s+");
	const int ADD		= patterns.Add(L"/+|-");
	const int MUL		= patterns.Add(L"/*|//");
	const int NUMBER	= patterns.Add(L"/d+(./d+)?");
	const int ID		= patterns.Add(L"[a-zA-Z_]/w*");
	const int OPEN		= patterns.Add(L"/(");
	const int CLOSE		= patterns.Add(L"/)");
	RegexLexer lexer(patterns.Wrap());

	Rule<TokenInput<RegexToken>, double> factor, term, exp;

	factor	= tk(NUMBER)[Convert]
			| (tk(L"-") >> factor)[Negative]
			| (tk(OPEN) >> exp << tk(CLOSE))
			| (tk(ID) + (tk(L"(") >> exp << tk(L")")))[Call]
			;
	term	= lrec(factor + *(tk(MUL) + factor), Operator);
	exp		= lrec(term + *(tk(ADD) + term), Operator);

	Console::WriteLine(L"Input an expression like (1+2)*abs(-3-4)");
	while(true)
	{
		Console::Write(L"\r\nexpression>");
		WString line=Console::Read();
		if(line==L"")
		{
			break;
		}

		try
		{
			List<RegexToken> tokens;
			CopyFrom(tokens.Wrap(), lexer.Parse(line)>>Where(IsNotBlank));
			FOREACH(RegexToken, token, tokens.Wrap())
			{
				if(token.token==-1)
				{
					throw Exception(L"Syntax error. Unknown token: \""+WString(token.reading, token.length)+L"\".");
				}
			}
			try
			{
				TokenInput<RegexToken> input(&tokens[0], tokens.Count());
				double result=exp.ParseFull(input, false);
				Console::WriteLine(L"Result is "+ftow(result));
			}
			catch(const CombinatorException<TokenInput<RegexToken>>& e)
			{
				if(e.GetInput().Available())
				{
					throw Exception(L"Syntax error. First occurs at \""+WString(e.GetInput().Current().reading)+L"\".");
				}
				else
				{
					throw Exception(L"Syntax error. Expression is not complete.");
				}
			}
		}
		catch(const Exception& e)
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(e.Message());
			Console::SetColor(true, true, true, false);
		}
	}
	return 0;
}