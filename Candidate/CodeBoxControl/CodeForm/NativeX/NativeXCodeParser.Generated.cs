using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeForm.NativeX
{
    public static class NativeXCodeParser
    {
        public static CodeForm.NativeX.SyntaxTree.NativeXExpression ParseExpression(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
            {
                CodeForm.NativeX.SyntaxTree.NativeXExpression result1 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    result1 = ParseEXP0(tokens, ref currentIndex1, ref parseSuccess);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXExpression ParseEXP0(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
            {
                CodeForm.NativeX.SyntaxTree.NativeXExpression result1 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXCastingExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXCastingExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndexCopy2 = currentIndex1;
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == "(")
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
                                CodeForm.NativeX.SyntaxTree.NativeXExpression result3 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
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
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == ")")
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression ParseResult(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression);
            System.String CodeMember0 = default(System.String);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "result")
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXFunctionResultExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression ParseException(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression);
            System.String CodeMember0 = default(System.String);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "exception")
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXExceptionExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXCastingExpression ParseCasting(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXCastingExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXCastingExpression);
            CodeForm.NativeX.SyntaxTree.NativeXType TypeMember0 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
            CodeForm.NativeX.SyntaxTree.NativeXExpression OperandMember0 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "cast")
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "<")
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
                    CodeForm.NativeX.SyntaxTree.NativeXType TypeMember1 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
                    {
                        CodeForm.NativeX.SyntaxTree.NativeXType result2 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ">")
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
                    CodeForm.NativeX.SyntaxTree.NativeXExpression OperandMember1 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
                    {
                        CodeForm.NativeX.SyntaxTree.NativeXExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXCastingExpression>();
                result.Type = TypeMember0;
                result.Operand = OperandMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXType ParseType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXType result = default(CodeForm.NativeX.SyntaxTree.NativeXType);
            {
                CodeForm.NativeX.SyntaxTree.NativeXReferenceType result1 = default(CodeForm.NativeX.SyntaxTree.NativeXReferenceType);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    result1 = ParseReferenceType(tokens, ref currentIndex1, ref parseSuccess);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXType>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXReferenceType ParseReferenceType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXReferenceType result = default(CodeForm.NativeX.SyntaxTree.NativeXReferenceType);
            System.String ReferencedNameMember0 = default(System.String);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == CodeForm.NativeX.NativeXTokenizer.IdToken)
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXReferenceType>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression ParsePrimitive(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression);
            System.String CodeMember0 = default(System.String);
            {
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeForm.NativeX.NativeXTokenizer.StringToken)
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeForm.NativeX.NativeXTokenizer.NumberToken)
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == "true")
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == "false")
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == "null")
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Value == "exception")
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXPrimitiveExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXExpression ParseReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
            {
                CodeForm.NativeX.SyntaxTree.NativeXExpression result1 = default(CodeForm.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression);
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
                        CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression result2 = default(CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression);
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression ParseInstanceFunctionReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
            System.String ConceptNameMember0 = default(System.String);
            CodeForm.NativeX.SyntaxTree.NativeXType TypeMember0 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
            System.String FunctionNameMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ConceptNameMember1 = default(System.String);
                    {
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeForm.NativeX.NativeXTokenizer.IdToken)
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "<")
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
                    CodeForm.NativeX.SyntaxTree.NativeXType TypeMember1 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
                    {
                        CodeForm.NativeX.SyntaxTree.NativeXType result2 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ">")
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ":")
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ":")
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
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeForm.NativeX.NativeXTokenizer.IdToken)
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXInstanceFunctionExpression>();
                result.ConceptName = ConceptNameMember0;
                result.Type = TypeMember0;
                result.FunctionName = FunctionNameMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression ParseInstanciatedReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression);
            System.String ReferencedNameMember0 = default(System.String);
            CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember0 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ReferencedNameMember1 = default(System.String);
                    {
                        CodeBoxControl.CodeProvider.CodeToken result2 = default(CodeBoxControl.CodeProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            if (currentIndex2 < tokens.Count && tokens[currentIndex2].Id == CodeForm.NativeX.NativeXTokenizer.IdToken)
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == "<")
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
                    CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember1 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType>);
                    {
                        CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType> result2 = default(CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new CodeBoxControl.CodeProvider.CodeNodeList<CodeForm.NativeX.SyntaxTree.NativeXType>();
                            {
                                CodeForm.NativeX.SyntaxTree.NativeXType result3 = default(CodeForm.NativeX.SyntaxTree.NativeXType);
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
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Value == ">")
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXInstanciatedExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.GenericArguments = GenericArgumentsMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression ParseIdentifierReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            CodeBoxControl.Core.TextPosition start = CodeBoxControl.CodeProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression result = default(CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression);
            System.String ReferencedNameMember0 = default(System.String);
            {
                CodeBoxControl.CodeProvider.CodeToken result1 = default(CodeBoxControl.CodeProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    if (currentIndex1 < tokens.Count && tokens[currentIndex1].Id == CodeForm.NativeX.NativeXTokenizer.IdToken)
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
                if (result == null) result = CodeNode.Create<CodeForm.NativeX.SyntaxTree.NativeXReferenceExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = CodeBoxControl.CodeProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

    }
}
