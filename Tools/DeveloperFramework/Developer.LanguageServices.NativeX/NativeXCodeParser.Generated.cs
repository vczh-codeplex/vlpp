using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    public static class NativeXCodeParser
    {
        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseExpression(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    result1 = ParseEXP13(tokens, ref currentIndex1, ref parseSuccess);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP13(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP12(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "+=" || tokens[currentIndex5].Value == "-=" || tokens[currentIndex5].Value == "*=" || tokens[currentIndex5].Value == "/=" || tokens[currentIndex5].Value == "%=" || tokens[currentIndex5].Value == "<<=" || tokens[currentIndex5].Value == ">>=" || tokens[currentIndex5].Value == "&=" || tokens[currentIndex5].Value == "/=" || tokens[currentIndex5].Value == "&&=" || tokens[currentIndex5].Value == "||=" || tokens[currentIndex5].Value == "="))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP12(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP12(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP11(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "||"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP11(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP11(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP10(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "&&"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP10(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP10(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP9(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "|"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP9(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP9(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP8(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "^"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP8(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP8(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP7(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "&"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP7(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP7(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP6(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "==" || tokens[currentIndex5].Value == "!="))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP6(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP6(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP5(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "<" || tokens[currentIndex5].Value == "<=" || tokens[currentIndex5].Value == ">" || tokens[currentIndex5].Value == ">="))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP5(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP5(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP4(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "<<" || tokens[currentIndex5].Value == ">>"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP4(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP4(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP3(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "+" || tokens[currentIndex5].Value == "-"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP3(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP3(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP2(tokens, ref currentIndex2, ref parseSuccess);
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
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                if (currentIndex5 < tokens.Count && (tokens[currentIndex5].Value == "*" || tokens[currentIndex5].Value == "/" || tokens[currentIndex5].Value == "%"))
                                                {
                                                    result5 = tokens[currentIndex5];
                                                    currentIndex5++;
                                                    parseSuccess = true;
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
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
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP2(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
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
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP2(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseEXP1(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseUnary(tokens, ref currentIndex2, ref parseSuccess);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP1(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP0(tokens, ref currentIndex2, ref parseSuccess);
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
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "["))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
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
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseExpression(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    SubscribeMember4 = result5;
                                                }
                                            }
                                            SubscribeMember3 = SubscribeMember4;
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
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "]"))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
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
                                    SubscribeMember2 = SubscribeMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXSubscribeExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXSubscribeExpression>();
                                    result2.Subscribe = SubscribeMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "("))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                            {
                                                Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> result5 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    result5 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                                                    {
                                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                                        int currentIndex6 = currentIndex5;
                                                        {
                                                            parseSuccess = false;
                                                            result6 = ParseExpression(tokens, ref currentIndex6, ref parseSuccess);
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndex5 = currentIndex6;
                                                            result5.Add(result6);
                                                            result5.Start = result6.Start;
                                                            result5.End = result6.End;
                                                        }
                                                        while (true)
                                                        {
                                                            int currentIndexCopy6 = currentIndex5;
                                                            {
                                                                parseSuccess = false;
                                                                if (currentIndex6 < tokens.Count && (tokens[currentIndex6].Value == ","))
                                                                {
                                                                    currentIndex6++;
                                                                    parseSuccess = true;
                                                                }
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_3;
                                                            }
                                                            {
                                                                parseSuccess = false;
                                                                result6 = ParseExpression(tokens, ref currentIndex6, ref parseSuccess);
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_3;
                                                            }
                                                            currentIndex5 = currentIndexCopy6;
                                                            result5.Add(result6);
                                                            result5.End = result6.End;
                                                        }
                                                    LABEL_3: ; parseSuccess = true;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    ArgumentsMember4 = result5;
                                                }
                                            }
                                            ArgumentsMember3 = ArgumentsMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == ")"))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_2: ;
                                    }
                                    ArgumentsMember2 = ArgumentsMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXInvokeExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInvokeExpression>();
                                    result2.Arguments = ArgumentsMember2;
                                    result2.Function = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String MemberNameMember2 = default(System.String);
                                {
                                    System.String MemberNameMember3 = default(System.String);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "."))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_4;
                                        }
                                        {
                                            System.String MemberNameMember4 = default(System.String);
                                            {
                                                Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    if (currentIndex5 < tokens.Count && tokens[currentIndex5].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                                                    {
                                                        result5 = tokens[currentIndex5];
                                                        currentIndex5++;
                                                        parseSuccess = true;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    MemberNameMember4 = result5.Value;
                                                }
                                            }
                                            MemberNameMember3 = MemberNameMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_4;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_4: ;
                                    }
                                    MemberNameMember2 = MemberNameMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXMemberExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXMemberExpression>();
                                    result2.MemberName = MemberNameMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String MemberNameMember2 = default(System.String);
                                {
                                    System.String MemberNameMember3 = default(System.String);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "->"))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_5;
                                        }
                                        {
                                            System.String MemberNameMember4 = default(System.String);
                                            {
                                                Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    if (currentIndex5 < tokens.Count && tokens[currentIndex5].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                                                    {
                                                        result5 = tokens[currentIndex5];
                                                        currentIndex5++;
                                                        parseSuccess = true;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    MemberNameMember4 = result5.Value;
                                                }
                                            }
                                            MemberNameMember3 = MemberNameMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_5;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_5: ;
                                    }
                                    MemberNameMember2 = MemberNameMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerMemberExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerMemberExpression>();
                                    result2.MemberName = MemberNameMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String OperatorMember2 = default(System.String);
                                {
                                    System.String OperatorMember3 = default(System.String);
                                    {
                                        Developer.LanguageProvider.CodeToken result4 = default(Developer.LanguageProvider.CodeToken);
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "++" || tokens[currentIndex4].Value == "--"))
                                            {
                                                result4 = tokens[currentIndex4];
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex2 = currentIndex4;
                                            OperatorMember3 = result4.Value;
                                        }
                                    }
                                    OperatorMember2 = OperatorMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPostUnaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPostUnaryExpression>();
                                    result2.Operator = OperatorMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            break;
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP0(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseResult(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseException(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseCasting(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParsePrimitive(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReference(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndexCopy2 = currentIndex1;
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && (tokens[currentIndex2].Value == "("))
                            {
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        {
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2 = result3;
                                }
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && (tokens[currentIndex2].Value == ")"))
                            {
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        currentIndex1 = currentIndexCopy2;
                        result1 = result2;
                    LABEL_2: ;
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression ParseResult(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression);
            System.String CodeMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "result"))
                    {
                        result1 = tokens[currentIndex1];
                        currentIndex1++;
                        parseSuccess = true;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    CodeMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression ParseException(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression);
            System.String CodeMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "exception"))
                    {
                        result1 = tokens[currentIndex1];
                        currentIndex1++;
                        parseSuccess = true;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    CodeMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression ParseCasting(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "cast"))
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "<"))
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
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ">"))
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "("))
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
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperandMember1 = result2;
                        }
                    }
                    OperandMember0 = OperandMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ")"))
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression>();
                result.Type = TypeMember0;
                result.Operand = OperandMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ParseType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParsePrimitiveType(tokens, ref currentIndex2, ref parseSuccess);
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
                            {
                                {
                                    parseSuccess = false;
                                    if (currentIndex2 < tokens.Count && (tokens[currentIndex2].Value == "*"))
                                    {
                                        currentIndex2++;
                                        parseSuccess = true;
                                    }
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerType result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerType>();
                                    result2.ElementType = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "["))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
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
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParsePrimitive(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    SizeMember4 = result5;
                                                }
                                            }
                                            SizeMember3 = SizeMember4;
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
                                            parseSuccess = false;
                                            if (currentIndex4 < tokens.Count && (tokens[currentIndex4].Value == "]"))
                                            {
                                                currentIndex4++;
                                                parseSuccess = true;
                                            }
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
                                    SizeMember2 = SizeMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXArrayType result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXArrayType>();
                                    result2.Size = SizeMember2;
                                    result2.ElementType = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            break;
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ParsePrimitiveType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseFunctionType(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanciatedType(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReferenceType(tokens, ref currentIndex2, ref parseSuccess);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType ParseFunctionType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> ParametersMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "function"))
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
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReturnTypeMember1 = result2;
                        }
                    }
                    ReturnTypeMember0 = ReturnTypeMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "("))
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
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> ParametersMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                                        if (currentIndex3 < tokens.Count && (tokens[currentIndex3].Value == ","))
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
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ")"))
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType>();
                result.ReturnType = ReturnTypeMember0;
                result.Parameters = ParametersMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType ParseInstanciatedType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ElementTypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ElementTypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReferenceType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ElementTypeMember1 = result2;
                        }
                    }
                    ElementTypeMember0 = ElementTypeMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "<"))
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
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                                        if (currentIndex3 < tokens.Count && (tokens[currentIndex3].Value == ","))
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
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                            GenericArgumentsMember1 = result2;
                        }
                    }
                    GenericArgumentsMember0 = GenericArgumentsMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ">"))
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType>();
                result.ElementType = ElementTypeMember0;
                result.GenericArguments = GenericArgumentsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ParseReferenceType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
            System.String ReferencedNameMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                    {
                        result1 = tokens[currentIndex1];
                        currentIndex1++;
                        parseSuccess = true;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    ReferencedNameMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression ParsePrimitive(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
            System.String CodeMember0 = default(System.String);
            {
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.StringToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.NumberToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && (tokens[currentIndex2].Value == "true" || tokens[currentIndex2].Value == "false" || tokens[currentIndex2].Value == "null"))
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanceFunctionReference(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanciatedReference(tokens, ref currentIndex2, ref parseSuccess);
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
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseIdentifierReference(tokens, ref currentIndex2, ref parseSuccess);
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression ParseInstanceFunctionReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
            System.String ConceptNameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String FunctionNameMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ConceptNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConceptNameMember1 = result2.Value;
                        }
                    }
                    ConceptNameMember0 = ConceptNameMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "<"))
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
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ">"))
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ":"))
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
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ":"))
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
                    System.String FunctionNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            FunctionNameMember1 = result2.Value;
                        }
                    }
                    FunctionNameMember0 = FunctionNameMember1;
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression>();
                result.ConceptName = ConceptNameMember0;
                result.Type = TypeMember0;
                result.FunctionName = FunctionNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression ParseInstanciatedReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression);
            System.String ReferencedNameMember0 = default(System.String);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ReferencedNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReferencedNameMember1 = result2.Value;
                        }
                    }
                    ReferencedNameMember0 = ReferencedNameMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == "<"))
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
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                                        if (currentIndex3 < tokens.Count && (tokens[currentIndex3].Value == ","))
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
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
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
                            GenericArgumentsMember1 = result2;
                        }
                    }
                    GenericArgumentsMember0 = GenericArgumentsMember1;
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
                    if (currentIndex1 < tokens.Count && (tokens[currentIndex1].Value == ">"))
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.GenericArguments = GenericArgumentsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression ParseIdentifierReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression);
            System.String ReferencedNameMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken)
                    {
                        result1 = tokens[currentIndex1];
                        currentIndex1++;
                        parseSuccess = true;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    ReferencedNameMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression ParseUnary(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression);
            System.String OperatorMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String OperatorMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && (tokens[currentIndex2].Value == "++" || tokens[currentIndex2].Value == "--" || tokens[currentIndex2].Value == "&" || tokens[currentIndex2].Value == "*" || tokens[currentIndex2].Value == "-" || tokens[currentIndex2].Value == "!" || tokens[currentIndex2].Value == "~"))
                            {
                                result2 = tokens[currentIndex2];
                                currentIndex2++;
                                parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperatorMember1 = result2.Value;
                        }
                    }
                    OperatorMember0 = OperatorMember1;
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
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseEXP2(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperandMember1 = result2;
                        }
                    }
                    OperandMember0 = OperandMember1;
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
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression>();
                result.Operator = OperatorMember0;
                result.Operand = OperandMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

    }
}
