﻿using System;
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
            {
                CodeBoxControlTest.CodeParser.BinaryExpression result1 = default(CodeBoxControlTest.CodeParser.BinaryExpression);
                int currentIndex1 = currentToken;
                if (currentToken != currentIndex1)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseTerm(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.BinaryExpression result1 = default(CodeBoxControlTest.CodeParser.BinaryExpression);
                int currentIndex1 = currentToken;
                if (currentToken != currentIndex1)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseFactor(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.NumberExpression ParseNumber(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.NumberExpression result = default(CodeBoxControlTest.CodeParser.NumberExpression);
            System.Double NumberMember0 = default(System.Double);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == 0)
                {
                    result1 = tokens[currentIndex1];
                    currentIndex1++;
                }
                if (currentToken != currentIndex1)
                {
                    currentToken = currentIndex1;
                    NumberMember0 = System.Double.Parse(result1.Value);
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.NumberExpression>();
            result.Number = NumberMember0;
            return result;
        }

        public static CodeBoxControlTest.CodeParser.FunctionExpression ParseFunction(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.FunctionExpression result = default(CodeBoxControlTest.CodeParser.FunctionExpression);
            System.String NameMember0 = default(System.String);
            CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> ParametersMember0 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                System.String NameMember1 = default(System.String);
                {
                    CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                    int currentIndex2 = currentIndex1;
                    if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == 1)
                    {
                        result2 = tokens[currentIndex2];
                        currentIndex2++;
                    }
                    if (currentIndex1 != currentIndex2)
                    {
                        currentIndex1 = currentIndex2;
                        NameMember1 = result2.Value;
                    }
                }
                NameMember0 = NameMember1;
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                {
                    currentIndex1++;
                }
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> ParametersMember1 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
                {
                    CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> result2 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
                    int currentIndex2 = currentIndex1;
                    if (currentIndex1 != currentIndex2)
                    {
                        currentIndex1 = currentIndex2;
                        ParametersMember1 = result2;
                    }
                }
                ParametersMember0 = ParametersMember1;
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                {
                    currentIndex1++;
                }
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
            LABEL_0: ;
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.FunctionExpression>();
            result.Name = NameMember0;
            result.Parameters = ParametersMember0;
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseBracket(List<CodeToken> tokens, ref int currentToken)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                {
                    currentIndex1++;
                }
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    CodeBoxControlTest.CodeParser.Expression result2 = default(CodeBoxControlTest.CodeParser.Expression);
                    int currentIndex2 = currentIndex1;
                    result2 = ParseExpression(tokens, ref currentIndex2);
                    if (currentIndex1 != currentIndex2)
                    {
                        currentIndex1 = currentIndex2;
                        result1 = result2;
                    }
                }
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                {
                    currentIndex1++;
                }
                if (currentIndexCopy1 != currentIndex1)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                result = result1;
            LABEL_0: ;
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

    }
}
