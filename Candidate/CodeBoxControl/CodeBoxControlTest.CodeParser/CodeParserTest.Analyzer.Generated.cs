using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest.CodeParser
{
    public static class CodeParserAnalyzer
    {
        public static CodeBoxControlTest.CodeParser.Expression ParseExpression(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = currentToken;
                {
                    int currentIndex2 = currentIndex1;
                    parseSuccess = false;
                    result1 = ParseTerm(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                    }
                    else
                    {
                        goto LABEL_0;
                    }
                    while (true)
                    {
                        parseSuccess = false;
                        ParseTerm(tokens, ref currentIndex2, ref parseSuccess);
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeBoxControlTest.CodeParser.BinaryExpression result2 = CodeNode.Create<CodeBoxControlTest.CodeParser.BinaryExpression>();
                            result2.Left = result1;
                            result1 = result2;
                        }
                        else
                        {
                            break;
                        }
                    }
                    parseSuccess = true;
                LABEL_0: ;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseTerm(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = currentToken;
                {
                    int currentIndex2 = currentIndex1;
                    parseSuccess = false;
                    result1 = ParseFactor(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                    }
                    else
                    {
                        goto LABEL_0;
                    }
                    while (true)
                    {
                        parseSuccess = false;
                        ParseFactor(tokens, ref currentIndex2, ref parseSuccess);
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeBoxControlTest.CodeParser.BinaryExpression result2 = CodeNode.Create<CodeBoxControlTest.CodeParser.BinaryExpression>();
                            result2.Left = result1;
                            result1 = result2;
                        }
                        else
                        {
                            break;
                        }
                    }
                    parseSuccess = true;
                LABEL_0: ;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseFactor(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    CodeBoxControlTest.CodeParser.NumberExpression result2 = default(CodeBoxControlTest.CodeParser.NumberExpression);
                    int currentIndex2 = currentIndex1;
                    parseSuccess = false;
                    result2 = ParseNumber(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        result1 = result2;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    CodeBoxControlTest.CodeParser.FunctionExpression result2 = default(CodeBoxControlTest.CodeParser.FunctionExpression);
                    int currentIndex2 = currentIndex1;
                    parseSuccess = false;
                    result2 = ParseFunction(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        result1 = result2;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    CodeBoxControlTest.CodeParser.Expression result2 = default(CodeBoxControlTest.CodeParser.Expression);
                    int currentIndex2 = currentIndex1;
                    parseSuccess = false;
                    result2 = ParseBracket(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        result1 = result2;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
            return result;
        }

        public static CodeBoxControlTest.CodeParser.NumberExpression ParseNumber(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControlTest.CodeParser.NumberExpression result = default(CodeBoxControlTest.CodeParser.NumberExpression);
            System.Double NumberMember0 = default(System.Double);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                parseSuccess = false;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == CodeBoxControlTest.CodeParser.CodeParserTokenizer.NumberToken)
                {
                    result1 = tokens[currentIndex1];
                    currentIndex1++;
                    parseSuccess = true;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    NumberMember0 = System.Double.Parse(result1.Value);
                }
            }
            if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.NumberExpression>();
            result.Number = NumberMember0;
            return result;
        }

        public static CodeBoxControlTest.CodeParser.FunctionExpression ParseFunction(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
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
                    parseSuccess = false;
                    if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeBoxControlTest.CodeParser.CodeParserTokenizer.IdToken)
                    {
                        result2 = tokens[currentIndex2];
                        currentIndex2++;
                        parseSuccess = true;
                    }
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        NameMember1 = result2.Value;
                    }
                }
                NameMember0 = NameMember1;
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                parseSuccess = false;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                {
                    currentIndex1++;
                    parseSuccess = true;
                }
                if (parseSuccess)
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
                    result2 = new CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>();
                    {
                        CodeBoxControlTest.CodeParser.Expression result3 = default(CodeBoxControlTest.CodeParser.Expression);
                        int currentIndex3 = currentIndex2;
                        parseSuccess = false;
                        result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                        if (parseSuccess)
                        {
                            currentIndex2 = currentIndex3;
                            result2.Add(result3);
                        }
                        while (true)
                        {
                            int currentIndexCopy3 = currentIndex2;
                            parseSuccess = false;
                            if (currentIndex3 < tokens.Count && tokens[currentIndex3].Value == ",")
                            {
                                currentIndex3++;
                                parseSuccess = true;
                            }
                            if (parseSuccess)
                            {
                                currentIndexCopy3 = currentIndex3;
                            }
                            else
                            {
                                goto LABEL_1;
                            }
                            parseSuccess = false;
                            result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                            if (parseSuccess)
                            {
                                currentIndexCopy3 = currentIndex3;
                            }
                            else
                            {
                                goto LABEL_1;
                            }
                            currentIndex2 = currentIndexCopy3;
                            result2.Add(result3);
                        }
                    LABEL_1: ; parseSuccess = true;
                    }
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        ParametersMember1 = result2;
                    }
                }
                ParametersMember0 = ParametersMember1;
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                parseSuccess = false;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                {
                    currentIndex1++;
                    parseSuccess = true;
                }
                if (parseSuccess)
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

        public static CodeBoxControlTest.CodeParser.Expression ParseBracket(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                parseSuccess = false;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                {
                    currentIndex1++;
                    parseSuccess = true;
                }
                if (parseSuccess)
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
                    parseSuccess = false;
                    result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                    if (parseSuccess)
                    {
                        currentIndex1 = currentIndex2;
                        result1 = result2;
                    }
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                parseSuccess = false;
                if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                {
                    currentIndex1++;
                    parseSuccess = true;
                }
                if (parseSuccess)
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
