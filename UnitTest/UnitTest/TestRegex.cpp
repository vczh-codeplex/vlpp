#include <stdlib.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Regex\RegexExpression.h"
#include "..\..\Library\Regex\RegexWriter.h"
#include "..\..\Library\Regex\RegexPure.h"
#include "..\..\Library\Regex\RegexRich.h"
#include "..\..\Library\Regex\Regex.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\CharFormat.h"
#include "..\..\Library\Stream\Accessor.h"
#include "..\..\Library\Collections\OperationCopyFrom.h"

using namespace vl;
using namespace vl::regex;
using namespace vl::regex_internal;
using namespace vl::stream;

extern WString GetPath();

/***********************************************************************
语法分析
***********************************************************************/

void RegexAssert(const wchar_t* input, RegexNode node, bool pure)
{
	Expression::Ref exp=ParseExpression(input);
	TEST_ASSERT(exp->IsEqual(node.expression.Obj()));
	TEST_ASSERT(exp->CanTreatAsPure()==pure);
}

TEST_CASE(TestRegexCharSetParsing)
{
	RegexAssert(L"a", rC(L'a'), true);
	RegexAssert(L"vczh", rC(L'v')+rC(L'c')+rC(L'z')+rC(L'h'), true);

	RegexAssert(L"/d", r_d(), true);
	RegexAssert(L"/l", r_l(), true);
	RegexAssert(L"/w", r_w(), true);
	RegexAssert(L"/D", !r_d(), true);
	RegexAssert(L"/L", !r_l(), true);
	RegexAssert(L"/W", !r_w(), true);
	RegexAssert(L"/r", rC(L'\r'), true);
	RegexAssert(L"/n", rC(L'\n'), true);
	RegexAssert(L"/t", rC(L'\t'), true);

	RegexAssert(L"\\d", r_d(), true);
	RegexAssert(L"\\l", r_l(), true);
	RegexAssert(L"\\w", r_w(), true);
	RegexAssert(L"\\D", !r_d(), true);
	RegexAssert(L"\\L", !r_l(), true);
	RegexAssert(L"\\W", !r_w(), true);
	RegexAssert(L"\\r", rC(L'\r'), true);
	RegexAssert(L"\\n", rC(L'\n'), true);
	RegexAssert(L"\\t", rC(L'\t'), true);

	RegexAssert(L"^", rBegin(), false);
	RegexAssert(L"$", rEnd(), false);
	RegexAssert(L"[abc]", rC(L'a')%rC(L'b')%rC(L'c'), true);
	RegexAssert(L"[0-9]", r_d(), true);
	RegexAssert(L"[a-zA-Z_]", r_l(), true);
	RegexAssert(L"[a-zA-Z0-9_]", r_w(), true);
	RegexAssert(L"[^0-9]", !r_d(), true);
	RegexAssert(L"[^a-zA-Z_]", !r_l(), true);
	RegexAssert(L"[^a-zA-Z0-9_]", !r_w(), true);
}

TEST_CASE(TestRegexLoopParsing)
{
	RegexAssert(L"/d+", r_d().Some(), true);
	RegexAssert(L"/d*", r_d().Any(), true);
	RegexAssert(L"/d?", r_d().Opt(), true);
	RegexAssert(L"/d{3}", r_d().Loop(3, 3), true);
	RegexAssert(L"/d{3,5}", r_d().Loop(3, 5), true);
	RegexAssert(L"/d{4,}", r_d().AtLeast(4), true);
	
	RegexAssert(L"\\d+", r_d().Some(), true);
	RegexAssert(L"\\d*", r_d().Any(), true);
	RegexAssert(L"\\d?", r_d().Opt(), true);
	RegexAssert(L"\\d{3}", r_d().Loop(3, 3), true);
	RegexAssert(L"\\d{3,5}", r_d().Loop(3, 5), true);
	RegexAssert(L"\\d{4,}", r_d().AtLeast(4), true);
}

TEST_CASE(TestRegexFunctionParsing)
{
	RegexAssert(L"(<name>/d+)", rCapture(L"name", r_d().Some()), true);
	RegexAssert(L"(<&name>)", rUsing(L"name"), false);
	RegexAssert(L"(<$name>)", rMatch(L"name"), false);
	RegexAssert(L"(<$name;3>)", rMatch(L"name", 3), false);
	RegexAssert(L"(<$3>)", rMatch(3), false);
	RegexAssert(L"(=/d+)", +r_d().Some(), false);
	RegexAssert(L"(!/d+)", -r_d().Some(), false);
	RegexAssert(L"(=\\d+)", +r_d().Some(), false);
	RegexAssert(L"(!\\d+)", -r_d().Some(), false);
}

TEST_CASE(TestRegexComplexParsing)
{
	RegexAssert(L"a+(bc)*", rC(L'a').Some()+(rC(L'b')+rC(L'c')).Any(), true);
	RegexAssert(L"(1+2)*(3+4)", (rC(L'1').Some()+rC(L'2')).Any()+(rC(L'3').Some()+rC(L'4')), true);
	RegexAssert(L"[a-zA-Z_][a-zA-Z0-9_]*", r_l()+r_w().Any(), true);
	RegexAssert(L"((<part>/d+).){3}(<part>/d+)", (rCapture(L"part", r_d().Some())+rC(L'.')).Loop(3, 3)+rCapture(L"part", r_d().Some()), true);
	RegexAssert(L"ab|ac", (rC(L'a')+rC(L'b'))|(rC(L'a')+rC(L'c')), true);
	RegexAssert(L"a(b|c)", rC(L'a')+(rC(L'b')|rC(L'c')), true);
	RegexAssert(L"/.*[/r/n/t]", rAnyChar().Any()+(rC(L'\r')%rC(L'\n')%rC(L'\t')), true);
	
	RegexAssert(L"((<part>\\d+).){3}(<part>\\d+)", (rCapture(L"part", r_d().Some())+rC(L'.')).Loop(3, 3)+rCapture(L"part", r_d().Some()), true);
	RegexAssert(L"\\.*[\\r\\n\\t]", rAnyChar().Any()+(rC(L'\r')%rC(L'\n')%rC(L'\t')), true);
}

TEST_CASE(TestRegexCompleteParsingA)
{
	WString code=L"(<#part>/d+)(<#capture>(<section>(<&part>)))((<&capture>).){3}(<&capture>)";
	RegexExpression::Ref regex=ParseRegexExpression(code);

	Expression::Ref part=r_d().Some().expression;
	Expression::Ref capture=rCapture(L"section", rUsing(L"part")).expression;
	Expression::Ref main=((rUsing(L"capture")+rC(L'.')).Loop(3, 3)+rUsing(L"capture")).expression;

	TEST_ASSERT(regex->definitions.Count()==2);
	TEST_ASSERT(regex->definitions.Keys()[0]==L"capture");
	TEST_ASSERT(regex->definitions.Keys()[1]==L"part");

	TEST_ASSERT(regex->definitions[L"part"]->IsEqual(part.Obj()));
	TEST_ASSERT(regex->definitions[L"capture"]->IsEqual(capture.Obj()));
	TEST_ASSERT(regex->expression->IsEqual(main.Obj()));
}

TEST_CASE(TestRegexCompleteParsingB)
{
	WString code=L"((<part>\\d+).){3}(<part>\\d+)";
	RegexExpression::Ref regex=ParseRegexExpression(code);

	Expression::Ref main=((rCapture(L"part", r_d().Some())+rC(L'.')).Loop(3, 3)+rCapture(L"part", r_d().Some())).expression;

	TEST_ASSERT(regex->definitions.Count()==0);
	TEST_ASSERT(regex->expression->IsEqual(main.Obj()));
}

/***********************************************************************
字符集合正规化
***********************************************************************/

void NormalizedRegexAssert(const wchar_t* input, RegexNode node)
{
	CharRange::List subsets;
	Expression::Ref exp=ParseExpression(input);
	exp->NormalizeCharSet(subsets);
	TEST_ASSERT(exp->IsEqual(node.expression.Obj()));

	subsets.Clear();
	exp->CollectCharSet(subsets);
	exp->ApplyCharSet(subsets);
	TEST_ASSERT(exp->IsEqual(node.expression.Obj()));
}

TEST_CASE(TestCharSetNormalization)
{
	NormalizedRegexAssert(L"[a-g][h-n]", rC(L'a', L'g')+rC(L'h', L'n'));
	NormalizedRegexAssert(L"[a-g][g-n]", (rC(L'a', L'f')%rC(L'g'))+(rC(L'g')%rC(L'h', L'n')));
	NormalizedRegexAssert(L"/w+vczh", (
		rC(L'0', L'9')%rC(L'A', L'Z')%rC(L'_')%rC(L'a', L'b')%rC(L'c')%rC(L'd', L'g')%rC(L'h')%rC(L'i', L'u')%rC(L'v')%rC(L'w', L'y')%rC(L'z')
		).Some()+rC(L'v')+rC(L'c')+rC(L'z')+rC(L'h'));
	NormalizedRegexAssert(L"[0-2][1-3][2-4]", (rC(L'0')%rC(L'1')%rC(L'2'))+(rC(L'1')%rC(L'2')%rC(L'3'))+(rC(L'2')%rC(L'3')%rC(L'4')));
	NormalizedRegexAssert(L"[^C-X][A-Z]", (rC(1, L'A'-1)%rC(L'A', L'B')%rC(L'Y', L'Z')%rC(L'Z'+1, 65535))+(rC(L'A', L'B')%rC(L'C', L'X')%rC(L'Y', L'Z')));
}

/***********************************************************************
正则表达式解引用
***********************************************************************/

void MergedRegexAssert(const wchar_t* input, RegexNode node)
{
	RegexExpression::Ref regex=ParseRegexExpression(input);
	Expression::Ref exp=regex->Merge();
	TEST_ASSERT(exp->IsEqual(node.expression.Obj()));
}

TEST_CASE(TestRegexExpressionMerging)
{
	const wchar_t* code=L"(<#part>/d+)(<#capture>(<section>(<&part>)))((<&capture>).){3}(<&capture>)";
	RegexNode node=(rCapture(L"section", r_d().Some())+rC(L'.')).Loop(3, 3)+rCapture(L"section", r_d().Some());
	MergedRegexAssert(code, node);
}

/***********************************************************************
epsilon-NFA-DFA
***********************************************************************/

void PrintAutomaton(WString fileName, Automaton::Ref automaton)
{
	FileStream file(GetPath()+fileName, FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream output(file, encoder);
	StreamWriter writer(output);

	wchar_t intbuf[20]={0};
	for(int i=0;i<automaton->states.Count();i++)
	{
		State* state=automaton->states[i].Obj();
		if(automaton->startState==state)
		{
			writer.WriteString(L"[START]");
		}
		if(state->finalState)
		{
			writer.WriteString(L"[FINISH]");
		}
		writer.WriteString(L"State<");
		_itow_s(i, intbuf, sizeof(intbuf)/sizeof(*intbuf), 10);
		writer.WriteString(intbuf);
		writer.WriteLine(L">");
		for(int j=0;j<state->transitions.Count();j++)
		{
			Transition* transition=state->transitions[j];
			_itow_s(automaton->states.IndexOf(transition->target), intbuf, sizeof(intbuf)/sizeof(*intbuf), 10);
			writer.WriteString(L"    To State<");
			writer.WriteString(intbuf);
			writer.WriteString(L"> : ");
			switch(transition->type)
			{
			case Transition::Chars:
				writer.WriteString(L"<Char : ");
				_itow_s(transition->range.begin, intbuf, sizeof(intbuf)/sizeof(*intbuf), 10);
				writer.WriteString(intbuf);
				writer.WriteString(L"[");
				writer.WriteChar(transition->range.begin);
				writer.WriteString(L"] - ");
				_itow_s(transition->range.end, intbuf, sizeof(intbuf)/sizeof(*intbuf), 10);
				writer.WriteString(intbuf);
				writer.WriteString(L"[");
				writer.WriteChar(transition->range.end);
				writer.WriteLine(L"]>");
				break;
			case Transition::Epsilon:
				writer.WriteLine(L"<Epsilon>");
				break;
			case Transition::BeginString:
				writer.WriteLine(L"^");
				break;
			case Transition::EndString:
				writer.WriteLine(L"$");
				break;
			case Transition::Nop:
				writer.WriteLine(L"<Nop>");
				break;
			case Transition::Capture:
				writer.WriteString(L"<Capture : ");
				writer.WriteString(automaton->captureNames[transition->capture]);
				writer.WriteLine(L" >");
				break;
			case Transition::Match:
				writer.WriteString(L"<Match : ");
				writer.WriteString(automaton->captureNames[transition->capture]);
				writer.WriteString(L";");
				_itow_s(transition->index, intbuf, sizeof(intbuf)/sizeof(*intbuf), 10);
				writer.WriteString(intbuf);
				writer.WriteLine(L" >");
				break;
			case Transition::Positive:
				writer.WriteLine(L"<Positive>");
				break;
			case Transition::Negative:
				writer.WriteLine(L"<Negative>");
				break;
			case Transition::NegativeFail:
				writer.WriteLine(L"<NegativeFail>");
				break;
			case Transition::End:
				writer.WriteLine(L"<End>");
				break;
			}
		}
	}
}

void CompareToBaseline(WString fileName)
{
	WString generatedPath=GetPath()+fileName;
	WString baselinePath=GetPath()+L"Baseline\\"+fileName;

	FileStream generatedFile(generatedPath, FileStream::ReadOnly);
	FileStream baselineFile(baselinePath, FileStream::ReadOnly);

	BomDecoder generatedDecoder;
	BomDecoder baselineDecoder;

	DecoderStream generatedStream(generatedFile, generatedDecoder);
	DecoderStream baselineStream(baselineFile, baselineDecoder);

	StreamReader generatedReader(generatedStream);
	StreamReader baselineReader(baselineStream);

	TEST_ASSERT(generatedReader.ReadToEnd()==baselineReader.ReadToEnd());
}

void PrintRegex(WString name, WString code, bool compareToBaseline=true)
{
	RegexExpression::Ref regex=ParseRegexExpression(code);
	Expression::Ref expression=regex->Merge();
	CharRange::List subsets;
	expression->NormalizeCharSet(subsets);

	Dictionary<State*, State*> nfaStateMap;
	Group<State*, State*> dfaStateMap;
	Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
	Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, RichEpsilonChecker, nfaStateMap.Wrap());
	Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());

	PrintAutomaton(name+L".eNfa.txt", eNfa);
	PrintAutomaton(name+L".Nfa.txt", nfa);
	PrintAutomaton(name+L".Dfa.txt", dfa);

	if(compareToBaseline)
	{
		CompareToBaseline(name+L".eNfa.txt");
		CompareToBaseline(name+L".Nfa.txt");
		CompareToBaseline(name+L".Dfa.txt");
	}
}

TEST_CASE(TestEpsilonNfa)
{
	PrintRegex(L"Regex纯数字",			L"/d");
	PrintRegex(L"Regex整数",			L"(/+|-)?/d+");
	PrintRegex(L"Regex小数",			L"(/+|-)?/d+(./d+)?");
	PrintRegex(L"Regex字符串",			L"\"([^\\\\\"]|\\\\\\.)*\"");
	PrintRegex(L"Regex注释",			L"///*([^*]|/*+[^*//])*/*+//");
	PrintRegex(L"Regex简化IP地址",		L"(<#sec>(<sec>/d+))((<&sec>).){3}(<&sec>)");
	PrintRegex(L"Regex重复字符串",		L"^(<sec>/.+)(<$sec>)+$");
	PrintRegex(L"Regex预查",			L"/d+(=/w+)(!vczh)");
}

/***********************************************************************
纯匹配
***********************************************************************/

void RunPureInterpretor(const wchar_t* code, const wchar_t* input, int start, int length)
{
	CharRange::List subsets;
	Dictionary<State*, State*> nfaStateMap;
	Group<State*, State*> dfaStateMap;
	PureResult matchResult;

	RegexExpression::Ref regex=ParseRegexExpression(code);
	Expression::Ref expression=regex->Merge();
	expression->NormalizeCharSet(subsets);
	Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
	Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, PureEpsilonChecker, nfaStateMap.Wrap());
	Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());

	PureInterpretor interpretor(dfa, subsets);
	bool expectedSuccessful=start!=-1;
	TEST_ASSERT(interpretor.Match(input, input, matchResult)==expectedSuccessful);
	if(expectedSuccessful)
	{
		TEST_ASSERT(start==matchResult.start);
		TEST_ASSERT(length==matchResult.length);
	}
}

TEST_CASE(TestPureInterpretor)
{
	RunPureInterpretor(L"/d", L"abcde12345abcde", 5, 1);
	RunPureInterpretor(L"/d", L"12345abcde", 0, 1);
	RunPureInterpretor(L"/d", L"vczh", -1, 0);

	RunPureInterpretor(L"(/+|-)?/d+", L"abcde12345abcde", 5, 5);
	RunPureInterpretor(L"(/+|-)?/d+", L"abcde+12345abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+", L"abcde-12345abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+", L"12345abcde", 0, 5);
	RunPureInterpretor(L"(/+|-)?/d+", L"+12345abcde", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+", L"-12345abcde", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+", L"-+vczh+-", -1, 0);

	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345abcde", 5, 5);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345.abcde", 5, 5);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345.abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345.abcde", 5, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345.54321abcde", 5, 11);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345.54321abcde", 5, 12);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345.54321abcde", 5, 12);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"12345", 0, 5);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"12345.", 0, 5);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345.", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345.", 0, 6);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"12345.54321", 0, 11);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345.54321", 0, 12);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345.54321", 0, 12);
	RunPureInterpretor(L"(/+|-)?/d+(./d+)?", L"-+vczh+-", -1, 0);

	RunPureInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh\"is\"genius", 4, 4);
	RunPureInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh\"i\\r\\ns\"genius", 4, 8);
	RunPureInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh is genius", -1, 0);

	RunPureInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh/*is*/genius", 4, 6);
	RunPureInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh/***is***/genius", 4, 10);
	RunPureInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh is genius", -1, 0);
}

/***********************************************************************
富匹配
***********************************************************************/

Ptr<RichInterpretor> BuildRichInterpretor(const wchar_t* code)
{
	CharRange::List subsets;
	Dictionary<State*, State*> nfaStateMap;
	Group<State*, State*> dfaStateMap;

	RegexExpression::Ref regex=ParseRegexExpression(code);
	Expression::Ref expression=regex->Merge();
	expression->NormalizeCharSet(subsets);
	Automaton::Ref eNfa=expression->GenerateEpsilonNfa();
	Automaton::Ref nfa=EpsilonNfaToNfa(eNfa, RichEpsilonChecker, nfaStateMap.Wrap());
	Automaton::Ref dfa=NfaToDfa(nfa, dfaStateMap.Wrap());

	return new RichInterpretor(dfa);
}

void RunRichInterpretor(const wchar_t* code, const wchar_t* input, int start, int length)
{
	RichResult matchResult;
	Ptr<RichInterpretor> interpretor=BuildRichInterpretor(code);
	bool expectedSuccessful=start!=-1;
	TEST_ASSERT(interpretor->Match(input, input, matchResult)==expectedSuccessful);
	if(expectedSuccessful)
	{
		TEST_ASSERT(start==matchResult.start);
		TEST_ASSERT(length==matchResult.length);
	}
}

TEST_CASE(TestRichInterpretorSimple)
{
	RunRichInterpretor(L"/d", L"abcde12345abcde", 5, 1);
	RunRichInterpretor(L"/d", L"12345abcde", 0, 1);
	RunRichInterpretor(L"/d", L"vczh", -1, 0);

	RunRichInterpretor(L"(/+|-)?/d+", L"abcde12345abcde", 5, 5);
	RunRichInterpretor(L"(/+|-)?/d+", L"abcde+12345abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+", L"abcde-12345abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+", L"12345abcde", 0, 5);
	RunRichInterpretor(L"(/+|-)?/d+", L"+12345abcde", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+", L"-12345abcde", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+", L"-+vczh+-", -1, 0);

	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345abcde", 5, 5);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345.abcde", 5, 5);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345.abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345.abcde", 5, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde12345.54321abcde", 5, 11);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde+12345.54321abcde", 5, 12);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"abcde-12345.54321abcde", 5, 12);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"12345", 0, 5);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"12345.", 0, 5);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345.", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345.", 0, 6);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"12345.54321", 0, 11);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"+12345.54321", 0, 12);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"-12345.54321", 0, 12);
	RunRichInterpretor(L"(/+|-)?/d+(./d+)?", L"-+vczh+-", -1, 0);

	RunRichInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh\"is\"genius", 4, 4);
	RunRichInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh\"i\\r\\ns\"genius", 4, 8);
	RunRichInterpretor(L"\"([^\\\\\"]|\\\\\\.)*\"", L"vczh is genius", -1, 0);

	RunRichInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh/*is*/genius", 4, 6);
	RunRichInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh/***is***/genius", 4, 10);
	RunRichInterpretor(L"///*([^*]|/*+[^*//])*/*+//", L"vczh is genius", -1, 0);
}

TEST_CASE(TestRichInterpretorBeginEndString)
{
	RunRichInterpretor(L"/d+", L"abc1234abc", 3, 4);
	RunRichInterpretor(L"/d+", L"1234abc", 0, 4);
	RunRichInterpretor(L"/d+", L"abc1234", 3, 4);
	RunRichInterpretor(L"/d+", L"1234", 0, 4);
	
	RunRichInterpretor(L"^/d+", L"abc1234abc", -1, 0);
	RunRichInterpretor(L"^/d+", L"1234abc", 0, 4);
	RunRichInterpretor(L"^/d+", L"abc1234", -1, 0);
	RunRichInterpretor(L"^/d+", L"1234", 0, 4);
	
	RunRichInterpretor(L"/d+$", L"abc1234abc", -1, 0);
	RunRichInterpretor(L"/d+$", L"1234abc", -1, 0);
	RunRichInterpretor(L"/d+$", L"abc1234", 3, 4);
	RunRichInterpretor(L"/d+$", L"1234", 0, 4);
	
	RunRichInterpretor(L"^/d+$", L"abc1234abc", -1, 0);
	RunRichInterpretor(L"^/d+$", L"1234abc", -1, 0);
	RunRichInterpretor(L"^/d+$", L"abc1234", -1, 0);
	RunRichInterpretor(L"^/d+$", L"1234", 0, 4);
}

TEST_CASE(TestRichInterpretorLoop)
{
	RunRichInterpretor(L"/d+", L"abcde12345abcde", 5, 5);
	RunRichInterpretor(L"/d+?", L"abcde12345abcde", 5, 1);
	RunRichInterpretor(L"/d+a", L"abcde12345abcde", 5, 6);
	RunRichInterpretor(L"/d+?a", L"abcde12345abcde", 5, 6);
}

TEST_CASE(TestRichInterpretorCapture)
{
	{
		const wchar_t* code=L"(<number>/d+)";
		const wchar_t* input=L"abcde123456abcde";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		int index=regex->CaptureNames().IndexOf(L"number");
		TEST_ASSERT(index==0);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==5);
		TEST_ASSERT(result.length==6);
		TEST_ASSERT(result.captures.Count()==1);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==5);
		TEST_ASSERT(result.captures[0].length==6);
	}
	{
		const wchar_t* code=L"(<#sec>(<sec>/d+))((<&sec>).){3}(<&sec>)";
		const wchar_t* input=L"196.128.0.1";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		int index=regex->CaptureNames().IndexOf(L"sec");
		TEST_ASSERT(index==0);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==0);
		TEST_ASSERT(result.length==11);
		TEST_ASSERT(result.captures.Count()==4);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==0);
		TEST_ASSERT(result.captures[0].length==3);
		TEST_ASSERT(result.captures[1].capture==0);
		TEST_ASSERT(result.captures[1].start==4);
		TEST_ASSERT(result.captures[1].length==3);
		TEST_ASSERT(result.captures[2].capture==0);
		TEST_ASSERT(result.captures[2].start==8);
		TEST_ASSERT(result.captures[2].length==1);
		TEST_ASSERT(result.captures[3].capture==0);
		TEST_ASSERT(result.captures[3].start==10);
		TEST_ASSERT(result.captures[3].length==1);
	}
	{
		const wchar_t* code=L"(<sec>/d+?)(<$sec>)+";
		const wchar_t* input=L"98765123123123123123123";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		int index=regex->CaptureNames().IndexOf(L"sec");
		TEST_ASSERT(index==0);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==5);
		TEST_ASSERT(result.length==18);
		TEST_ASSERT(result.captures.Count()==1);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==5);
		TEST_ASSERT(result.captures[0].length==3);
	}
	{
		const wchar_t* code=L"(<sec>/d+)(<$sec>)+";
		const wchar_t* input=L"98765123123123123123123";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		int index=regex->CaptureNames().IndexOf(L"sec");
		TEST_ASSERT(index==0);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==5);
		TEST_ASSERT(result.length==18);
		TEST_ASSERT(result.captures.Count()==1);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==5);
		TEST_ASSERT(result.captures[0].length==9);
	}
}

TEST_CASE(TestRichInterpretorPrematching)
{
	{
		const wchar_t* code=L"win(=2000)";
		const wchar_t* input=L"win98win2000winxp";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==5);
		TEST_ASSERT(result.length==3);
		TEST_ASSERT(result.captures.Count()==0);
	}
	{
		const wchar_t* code=L"win(!98)";
		const wchar_t* input=L"win98win2000winxp";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==5);
		TEST_ASSERT(result.length==3);
		TEST_ASSERT(result.captures.Count()==0);
	}
}

TEST_CASE(TestRichInterpretorChaos)
{
	{
		const wchar_t* code=L"^(<a>/w+?)(<b>/w+?)((<$a>)(<$b>))+(<$a>)/w{6}$";
		const wchar_t* input=L"vczhgeniusvczhgeniusvczhgeniusvczhgenius";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		TEST_ASSERT(regex->CaptureNames().IndexOf(L"a")==0);
		TEST_ASSERT(regex->CaptureNames().IndexOf(L"b")==1);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==0);
		TEST_ASSERT(result.length==40);
		TEST_ASSERT(result.captures.Count()==2);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==0);
		TEST_ASSERT(result.captures[0].length==4);
		TEST_ASSERT(result.captures[1].capture==1);
		TEST_ASSERT(result.captures[1].start==4);
		TEST_ASSERT(result.captures[1].length==6);
	}
	{
		const wchar_t* code=L"^/d+./d*?(<sec>/d+?)(<$sec>)+$";
		const wchar_t* input=L"1428.57142857142857142857";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);
		TEST_ASSERT(regex->CaptureNames().IndexOf(L"sec")==0);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==0);
		TEST_ASSERT(result.length==25);
		TEST_ASSERT(result.captures.Count()==1);
		TEST_ASSERT(result.captures[0].capture==0);
		TEST_ASSERT(result.captures[0].start==7);
		TEST_ASSERT(result.captures[0].length==6);
	}
	{
		const wchar_t* code=L"^/d+./d*?(?/d+?)(<$0>)+$";
		const wchar_t* input=L"1428.57142857142857142857";
		RichResult result;
		Ptr<RichInterpretor> regex=BuildRichInterpretor(code);

		TEST_ASSERT(regex->Match(input, input, result)==true);
		TEST_ASSERT(result.start==0);
		TEST_ASSERT(result.length==25);
		TEST_ASSERT(result.captures.Count()==1);
		TEST_ASSERT(result.captures[0].capture==-1);
		TEST_ASSERT(result.captures[0].start==7);
		TEST_ASSERT(result.captures[0].length==6);
	}
}

/***********************************************************************
接口
***********************************************************************/

void TestRegexMatchPosition(bool preferPure)
{
	Regex regex(L"/d+", preferPure);
	TEST_ASSERT(regex.IsPureMatch()==preferPure);
	TEST_ASSERT(regex.IsPureTest()==preferPure);
	TEST_ASSERT(regex.TestHead(L"123abc")==true);
	TEST_ASSERT(regex.TestHead(L"abc123abc")==false);
	TEST_ASSERT(regex.TestHead(L"abc")==false);
	TEST_ASSERT(regex.Test(L"123abc")==true);
	TEST_ASSERT(regex.Test(L"abc123abc")==true);
	TEST_ASSERT(regex.Test(L"abc")==false);

	RegexMatch::Ref match;
	RegexMatch::List matches;

	match=regex.MatchHead(L"123abc");
	TEST_ASSERT(match);
	TEST_ASSERT(match->Success()==true);
	TEST_ASSERT(match->Result().Start()==0);
	TEST_ASSERT(match->Result().Length()==3);
	TEST_ASSERT(match->Result().Value()==L"123");

	match=regex.MatchHead(L"abc123abc");
	TEST_ASSERT(!match);

	match=regex.Match(L"123abc");
	TEST_ASSERT(match);
	TEST_ASSERT(match->Success()==true);
	TEST_ASSERT(match->Result().Start()==0);
	TEST_ASSERT(match->Result().Length()==3);
	TEST_ASSERT(match->Result().Value()==L"123");

	match=regex.Match(L"abc123abc");
	TEST_ASSERT(match);
	TEST_ASSERT(match->Success()==true);
	TEST_ASSERT(match->Result().Start()==3);
	TEST_ASSERT(match->Result().Length()==3);
	TEST_ASSERT(match->Result().Value()==L"123");

	matches.Clear();
	regex.Search(L"12abc34def56", matches);
	TEST_ASSERT(matches.Count()==3);
	TEST_ASSERT(matches[0]->Success()==true);
	TEST_ASSERT(matches[0]->Result().Start()==0);
	TEST_ASSERT(matches[0]->Result().Length()==2);
	TEST_ASSERT(matches[0]->Result().Value()==L"12");
	TEST_ASSERT(matches[1]->Success()==true);
	TEST_ASSERT(matches[1]->Result().Start()==5);
	TEST_ASSERT(matches[1]->Result().Length()==2);
	TEST_ASSERT(matches[1]->Result().Value()==L"34");
	TEST_ASSERT(matches[2]->Success()==true);
	TEST_ASSERT(matches[2]->Result().Start()==10);
	TEST_ASSERT(matches[2]->Result().Length()==2);
	TEST_ASSERT(matches[2]->Result().Value()==L"56");

	matches.Clear();
	regex.Split(L"12abc34def56", false, matches);
	TEST_ASSERT(matches.Count()==2);
	TEST_ASSERT(matches[0]->Success()==false);
	TEST_ASSERT(matches[0]->Result().Start()==2);
	TEST_ASSERT(matches[0]->Result().Length()==3);
	TEST_ASSERT(matches[0]->Result().Value()==L"abc");
	TEST_ASSERT(matches[1]->Success()==false);
	TEST_ASSERT(matches[1]->Result().Start()==7);
	TEST_ASSERT(matches[1]->Result().Length()==3);
	TEST_ASSERT(matches[1]->Result().Value()==L"def");

	matches.Clear();
	regex.Split(L"12abc34def56", true, matches);
	TEST_ASSERT(matches.Count()==4);
	TEST_ASSERT(matches[0]->Success()==false);
	TEST_ASSERT(matches[0]->Result().Start()==0);
	TEST_ASSERT(matches[0]->Result().Length()==0);
	TEST_ASSERT(matches[0]->Result().Value()==L"");
	TEST_ASSERT(matches[1]->Success()==false);
	TEST_ASSERT(matches[1]->Result().Start()==2);
	TEST_ASSERT(matches[1]->Result().Length()==3);
	TEST_ASSERT(matches[1]->Result().Value()==L"abc");
	TEST_ASSERT(matches[2]->Success()==false);
	TEST_ASSERT(matches[2]->Result().Start()==7);
	TEST_ASSERT(matches[2]->Result().Length()==3);
	TEST_ASSERT(matches[2]->Result().Value()==L"def");
	TEST_ASSERT(matches[3]->Success()==false);
	TEST_ASSERT(matches[3]->Result().Start()==12);
	TEST_ASSERT(matches[3]->Result().Length()==0);
	TEST_ASSERT(matches[3]->Result().Value()==L"");

	matches.Clear();
	regex.Cut(L"12abc34def56", false, matches);
	TEST_ASSERT(matches.Count()==5);
	TEST_ASSERT(matches[0]->Success()==true);
	TEST_ASSERT(matches[0]->Result().Start()==0);
	TEST_ASSERT(matches[0]->Result().Length()==2);
	TEST_ASSERT(matches[0]->Result().Value()==L"12");
	TEST_ASSERT(matches[1]->Success()==false);
	TEST_ASSERT(matches[1]->Result().Start()==2);
	TEST_ASSERT(matches[1]->Result().Length()==3);
	TEST_ASSERT(matches[1]->Result().Value()==L"abc");
	TEST_ASSERT(matches[2]->Success()==true);
	TEST_ASSERT(matches[2]->Result().Start()==5);
	TEST_ASSERT(matches[2]->Result().Length()==2);
	TEST_ASSERT(matches[2]->Result().Value()==L"34");
	TEST_ASSERT(matches[3]->Success()==false);
	TEST_ASSERT(matches[3]->Result().Start()==7);
	TEST_ASSERT(matches[3]->Result().Length()==3);
	TEST_ASSERT(matches[3]->Result().Value()==L"def");
	TEST_ASSERT(matches[4]->Success()==true);
	TEST_ASSERT(matches[4]->Result().Start()==10);
	TEST_ASSERT(matches[4]->Result().Length()==2);
	TEST_ASSERT(matches[4]->Result().Value()==L"56");

	matches.Clear();
	regex.Cut(L"12abc34def56", true, matches);
	TEST_ASSERT(matches.Count()==7);
	TEST_ASSERT(matches[0]->Success()==false);
	TEST_ASSERT(matches[0]->Result().Start()==0);
	TEST_ASSERT(matches[0]->Result().Length()==0);
	TEST_ASSERT(matches[0]->Result().Value()==L"");
	TEST_ASSERT(matches[1]->Success()==true);
	TEST_ASSERT(matches[1]->Result().Start()==0);
	TEST_ASSERT(matches[1]->Result().Length()==2);
	TEST_ASSERT(matches[1]->Result().Value()==L"12");
	TEST_ASSERT(matches[2]->Success()==false);
	TEST_ASSERT(matches[2]->Result().Start()==2);
	TEST_ASSERT(matches[2]->Result().Length()==3);
	TEST_ASSERT(matches[2]->Result().Value()==L"abc");
	TEST_ASSERT(matches[3]->Success()==true);
	TEST_ASSERT(matches[3]->Result().Start()==5);
	TEST_ASSERT(matches[3]->Result().Length()==2);
	TEST_ASSERT(matches[3]->Result().Value()==L"34");
	TEST_ASSERT(matches[4]->Success()==false);
	TEST_ASSERT(matches[4]->Result().Start()==7);
	TEST_ASSERT(matches[4]->Result().Length()==3);
	TEST_ASSERT(matches[4]->Result().Value()==L"def");
	TEST_ASSERT(matches[5]->Success()==true);
	TEST_ASSERT(matches[5]->Result().Start()==10);
	TEST_ASSERT(matches[5]->Result().Length()==2);
	TEST_ASSERT(matches[5]->Result().Value()==L"56");
	TEST_ASSERT(matches[6]->Success()==false);
	TEST_ASSERT(matches[6]->Result().Start()==12);
	TEST_ASSERT(matches[6]->Result().Length()==0);
	TEST_ASSERT(matches[6]->Result().Value()==L"");

	matches.Clear();
	regex.Cut(L"XY12abc34def56ZW", true, matches);
	TEST_ASSERT(matches.Count()==7);
	TEST_ASSERT(matches[0]->Success()==false);
	TEST_ASSERT(matches[0]->Result().Start()==0);
	TEST_ASSERT(matches[0]->Result().Length()==2);
	TEST_ASSERT(matches[0]->Result().Value()==L"XY");
	TEST_ASSERT(matches[1]->Success()==true);
	TEST_ASSERT(matches[1]->Result().Start()==2);
	TEST_ASSERT(matches[1]->Result().Length()==2);
	TEST_ASSERT(matches[1]->Result().Value()==L"12");
	TEST_ASSERT(matches[2]->Success()==false);
	TEST_ASSERT(matches[2]->Result().Start()==4);
	TEST_ASSERT(matches[2]->Result().Length()==3);
	TEST_ASSERT(matches[2]->Result().Value()==L"abc");
	TEST_ASSERT(matches[3]->Success()==true);
	TEST_ASSERT(matches[3]->Result().Start()==7);
	TEST_ASSERT(matches[3]->Result().Length()==2);
	TEST_ASSERT(matches[3]->Result().Value()==L"34");
	TEST_ASSERT(matches[4]->Success()==false);
	TEST_ASSERT(matches[4]->Result().Start()==9);
	TEST_ASSERT(matches[4]->Result().Length()==3);
	TEST_ASSERT(matches[4]->Result().Value()==L"def");
	TEST_ASSERT(matches[5]->Success()==true);
	TEST_ASSERT(matches[5]->Result().Start()==12);
	TEST_ASSERT(matches[5]->Result().Length()==2);
	TEST_ASSERT(matches[5]->Result().Value()==L"56");
	TEST_ASSERT(matches[6]->Success()==false);
	TEST_ASSERT(matches[6]->Result().Start()==14);
	TEST_ASSERT(matches[6]->Result().Length()==2);
	TEST_ASSERT(matches[6]->Result().Value()==L"ZW");
}

TEST_CASE(TestRegexMatchPosition)
{
	TestRegexMatchPosition(true);
	TestRegexMatchPosition(false);
}

TEST_CASE(TestRegexCapture)
{
	{
		Regex regex(L"^(<a>/w+?)(<b>/w+?)((<$a>)(<$b>))+(<$a>)/w{6}$", true);
		TEST_ASSERT(regex.IsPureMatch()==false);
		TEST_ASSERT(regex.IsPureTest()==false);

		RegexMatch::Ref match=regex.Match(L"vczhgeniusvczhgeniusvczhgeniusvczhgenius");
		TEST_ASSERT(match);
		TEST_ASSERT(match->Success()==true);
		TEST_ASSERT(match->Result().Start()==0);
		TEST_ASSERT(match->Result().Length()==40);
		TEST_ASSERT(match->Result().Value()==L"vczhgeniusvczhgeniusvczhgeniusvczhgenius");
		TEST_ASSERT(match->Groups().Keys().Count()==2);
		TEST_ASSERT(match->Groups().Keys()[0]==L"a");
		TEST_ASSERT(match->Groups().Keys()[1]==L"b");
		TEST_ASSERT(match->Groups()[L"a"].Count()==1);
		TEST_ASSERT(match->Groups()[L"a"][0].Start()==0);
		TEST_ASSERT(match->Groups()[L"a"][0].Length()==4);
		TEST_ASSERT(match->Groups()[L"a"][0].Value()==L"vczh");
		TEST_ASSERT(match->Groups()[L"b"].Count()==1);
		TEST_ASSERT(match->Groups()[L"b"][0].Start()==4);
		TEST_ASSERT(match->Groups()[L"b"][0].Length()==6);
		TEST_ASSERT(match->Groups()[L"b"][0].Value()==L"genius");
	}
	{
		Regex regex(L"^(?/d+).(?/d+).(?/d+).(<$0>).(<$1>).(<$2>)$");
		RegexMatch::Ref match;

		match=regex.MatchHead(L"12.34.56.12.56.34");
		TEST_ASSERT(!match);

		match=regex.MatchHead(L"123.4.56.123.4.56");
		TEST_ASSERT(match);
		TEST_ASSERT(match->Success()==true);
		TEST_ASSERT(match->Result().Start()==0);
		TEST_ASSERT(match->Result().Length()==17);
		TEST_ASSERT(match->Result().Value()==L"123.4.56.123.4.56");
		TEST_ASSERT(match->Captures().Count()==3);
		TEST_ASSERT(match->Captures()[0].Start()==0);
		TEST_ASSERT(match->Captures()[0].Length()==3);
		TEST_ASSERT(match->Captures()[0].Value()==L"123");
		TEST_ASSERT(match->Captures()[1].Start()==4);
		TEST_ASSERT(match->Captures()[1].Length()==1);
		TEST_ASSERT(match->Captures()[1].Value()==L"4");
		TEST_ASSERT(match->Captures()[2].Start()==6);
		TEST_ASSERT(match->Captures()[2].Length()==2);
		TEST_ASSERT(match->Captures()[2].Value()==L"56");
	}
}

/***********************************************************************
词法分析
***********************************************************************/

TEST_CASE(TestRegexLexer)
{
	List<WString> codes;
	codes.Add(L"/d+");
	codes.Add(L"/s+");
	codes.Add(L"[a-zA-Z_]/w*");
	RegexLexer lexer(codes.Wrap());

	List<RegexToken> tokens;
	CopyFrom(tokens.Wrap(), lexer.Parse(L"vczh is$$a&&genius  1234"));

	TEST_ASSERT(tokens.Count()==9);
	TEST_ASSERT(tokens[0].start==0);
	TEST_ASSERT(tokens[0].length==4);
	TEST_ASSERT(tokens[0].token==2);
	TEST_ASSERT(tokens[1].start==4);
	TEST_ASSERT(tokens[1].length==1);
	TEST_ASSERT(tokens[1].token==1);
	TEST_ASSERT(tokens[2].start==5);
	TEST_ASSERT(tokens[2].length==2);
	TEST_ASSERT(tokens[2].token==2);
	TEST_ASSERT(tokens[3].start==7);
	TEST_ASSERT(tokens[3].length==2);
	TEST_ASSERT(tokens[3].token==-1);
	TEST_ASSERT(tokens[4].start==9);
	TEST_ASSERT(tokens[4].length==1);
	TEST_ASSERT(tokens[4].token==2);
	TEST_ASSERT(tokens[5].start==10);
	TEST_ASSERT(tokens[5].length==2);
	TEST_ASSERT(tokens[5].token==-1);
	TEST_ASSERT(tokens[6].start==12);
	TEST_ASSERT(tokens[6].length==6);
	TEST_ASSERT(tokens[6].token==2);
	TEST_ASSERT(tokens[7].start==18);
	TEST_ASSERT(tokens[7].length==2);
	TEST_ASSERT(tokens[7].token==1);
	TEST_ASSERT(tokens[8].start==20);
	TEST_ASSERT(tokens[8].length==4);
	TEST_ASSERT(tokens[8].token==0);
}