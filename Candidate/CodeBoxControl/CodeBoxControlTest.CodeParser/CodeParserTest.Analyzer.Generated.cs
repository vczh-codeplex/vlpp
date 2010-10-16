using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest.CodeParser
{
    public static class CodeParserAnalyzer
    {
        public static CodeBoxControlTest.CodeParser.Expression ParseExpression(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseTerm(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseFactor(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            return result;
        }

        public static CodeBoxControlTest.CodeParser.NumberExpression ParseNumber(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.NumberExpression result = default(CodeBoxControlTest.CodeParser.NumberExpression);
            System.Double NumberMember0 = default(System.Double);
            return result;
        }

        public static CodeBoxControlTest.CodeParser.FunctionExpression ParseFunction(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.FunctionExpression result = default(CodeBoxControlTest.CodeParser.FunctionExpression);
            System.String NameMember0 = default(System.String);
            CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> ParametersMember0 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseBracket(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            return result;
        }

    }
}
