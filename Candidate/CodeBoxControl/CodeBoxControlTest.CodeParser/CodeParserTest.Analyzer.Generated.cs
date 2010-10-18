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
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseTerm(tokens, ref currentIndex2, ref parseSuccess);
                        }
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
                            System.String OperatorMember2 = default(System.String);
                            CodeBoxControlTest.CodeParser.Expression RightMember2 = default(CodeBoxControlTest.CodeParser.Expression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                CodeBoxControlTest.CodeParser.Expression RightMember3 = default(CodeBoxControlTest.CodeParser.Expression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            int currentIndex5 = -1;
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    CodeBoxControl.CodeProvider.CodeToken result6 = default(CodeBoxControl.CodeProvider.CodeToken);
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        parseSuccess = false;
                                                        if (currentIndex6 < tokens.Count && tokens[currentIndex6].Value == "+")
                                                        {
                                                            result6 = tokens[currentIndex6];
                                                            currentIndex6++;
                                                            parseSuccess = true;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = result6.Value;
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    CodeBoxControl.CodeProvider.CodeToken result6 = default(CodeBoxControl.CodeProvider.CodeToken);
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        parseSuccess = false;
                                                        if (currentIndex6 < tokens.Count && tokens[currentIndex6].Value == "-")
                                                        {
                                                            result6 = tokens[currentIndex6];
                                                            currentIndex6++;
                                                            parseSuccess = true;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = result6.Value;
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            goto LABEL_FAIL_3;
                                        LABEL_SUCCESS_2: ;
                                        LABEL_FAIL_3: ;
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        CodeBoxControlTest.CodeParser.Expression RightMember4 = default(CodeBoxControlTest.CodeParser.Expression);
                                        {
                                            CodeBoxControlTest.CodeParser.Expression result5 = default(CodeBoxControlTest.CodeParser.Expression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseTerm(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightMember4 = result5;
                                            }
                                        }
                                        RightMember3 = RightMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightMember2 = RightMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                CodeBoxControlTest.CodeParser.BinaryExpression result2 = CodeNode.Create<CodeBoxControlTest.CodeParser.BinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.Right = RightMember2;
                                result2.Left = result1;
                                result2.Start = result1.Start;
                                result2.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseTerm(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseFactor(tokens, ref currentIndex2, ref parseSuccess);
                        }
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
                            System.String OperatorMember2 = default(System.String);
                            CodeBoxControlTest.CodeParser.Expression RightMember2 = default(CodeBoxControlTest.CodeParser.Expression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                CodeBoxControlTest.CodeParser.Expression RightMember3 = default(CodeBoxControlTest.CodeParser.Expression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            int currentIndex5 = -1;
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    CodeBoxControl.CodeProvider.CodeToken result6 = default(CodeBoxControl.CodeProvider.CodeToken);
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        parseSuccess = false;
                                                        if (currentIndex6 < tokens.Count && tokens[currentIndex6].Value == "*")
                                                        {
                                                            result6 = tokens[currentIndex6];
                                                            currentIndex6++;
                                                            parseSuccess = true;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = result6.Value;
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    CodeBoxControl.CodeProvider.CodeToken result6 = default(CodeBoxControl.CodeProvider.CodeToken);
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        parseSuccess = false;
                                                        if (currentIndex6 < tokens.Count && tokens[currentIndex6].Value == "/")
                                                        {
                                                            result6 = tokens[currentIndex6];
                                                            currentIndex6++;
                                                            parseSuccess = true;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = result6.Value;
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            goto LABEL_FAIL_3;
                                        LABEL_SUCCESS_2: ;
                                        LABEL_FAIL_3: ;
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        CodeBoxControlTest.CodeParser.Expression RightMember4 = default(CodeBoxControlTest.CodeParser.Expression);
                                        {
                                            CodeBoxControlTest.CodeParser.Expression result5 = default(CodeBoxControlTest.CodeParser.Expression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseFactor(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightMember4 = result5;
                                            }
                                        }
                                        RightMember3 = RightMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightMember2 = RightMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                CodeBoxControlTest.CodeParser.BinaryExpression result2 = CodeNode.Create<CodeBoxControlTest.CodeParser.BinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.Right = RightMember2;
                                result2.Left = result1;
                                result2.Start = result1.Start;
                                result2.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseFactor(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        CodeBoxControlTest.CodeParser.NumberExpression result2 = default(CodeBoxControlTest.CodeParser.NumberExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseNumber(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        CodeBoxControlTest.CodeParser.FunctionExpression result2 = default(CodeBoxControlTest.CodeParser.FunctionExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseFunction(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        CodeBoxControlTest.CodeParser.Expression result2 = default(CodeBoxControlTest.CodeParser.Expression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseBracket(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
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
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeBoxControlTest.CodeParser.NumberExpression ParseNumber(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.NumberExpression result = default(CodeBoxControlTest.CodeParser.NumberExpression);
            System.Double NumberMember0 = default(System.Double);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == CodeBoxControlTest.CodeParser.CodeParserTokenizer.NumberToken)
                    {
                        result1 = tokens[currentIndex1];
                        currentIndex1++;
                        parseSuccess = true;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    NumberMember0 = System.Double.Parse(result1.Value);
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.NumberExpression>();
                result.Number = NumberMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeBoxControlTest.CodeParser.FunctionExpression ParseFunction(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.FunctionExpression result = default(CodeBoxControlTest.CodeParser.FunctionExpression);
            System.String NameMember0 = default(System.String);
            CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> ParametersMember0 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String NameMember1 = default(System.String);
                    {
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeBoxControlTest.CodeParser.CodeParserTokenizer.IdToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                    {
                        currentIndex1++;
                        parseSuccess = true;
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
                {
                    CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> ParametersMember1 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
                    {
                        CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression> result2 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new CodeBoxControl.CodeProvider.CodeNodeList<CodeBoxControlTest.CodeParser.Expression>();
                            {
                                CodeBoxControlTest.CodeParser.Expression result3 = default(CodeBoxControlTest.CodeParser.Expression);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        parseSuccess = false;
                                        if (currentIndex3 < tokens.Count && tokens[currentIndex3].Value == ",")
                                        {
                                            currentIndex3++;
                                            parseSuccess = true;
                                        }
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                                    }
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
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ParametersMember1 = result2;
                        }
                    }
                    ParametersMember0 = ParametersMember1;
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                    {
                        currentIndex1++;
                        parseSuccess = true;
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
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.FunctionExpression>();
                result.Name = NameMember0;
                result.Parameters = ParametersMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeBoxControlTest.CodeParser.Expression ParseBracket(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeBoxControlTest.CodeParser.Expression result = default(CodeBoxControlTest.CodeParser.Expression);
            {
                CodeBoxControlTest.CodeParser.Expression result1 = default(CodeBoxControlTest.CodeParser.Expression);
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "(")
                    {
                        currentIndex1++;
                        parseSuccess = true;
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
                {
                    {
                        CodeBoxControlTest.CodeParser.Expression result2 = default(CodeBoxControlTest.CodeParser.Expression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
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
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ")")
                    {
                        currentIndex1++;
                        parseSuccess = true;
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
                currentToken = currentIndexCopy1;
                result = result1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeBoxControlTest.CodeParser.Expression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

    }
}
