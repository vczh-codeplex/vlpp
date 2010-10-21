using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX;

namespace Test.DeveloperComponent.NativeX
{
    [TestClass]
    public class NativeXCodeParserTest
    {
        private delegate T ParserMethod<T>(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess);

        private T Parse<T>(string input, ParserMethod<T> parser)
            where T : NativeXNode
        {
            List<CodeToken> tokens = new NativeXTokenizer().Tokenize(input.ToCharArray())
                .Where(t => t.Id == NativeXTokenizer.IdToken
                    || t.Id == NativeXTokenizer.NumberToken
                    || t.Id == NativeXTokenizer.OperatorToken
                    || t.Id == NativeXTokenizer.StringToken
                    )
                .ToList();
            int currentToken = 0;
            bool parseSuccess = false;
            return parser(tokens, ref currentToken, ref parseSuccess);
        }

        #region EXPRESSION

        private void TestParsePrimitiveInternal(Func<string, NativeXPrimitiveExpression> parser)
        {
            Assert.AreEqual("1", parser("1").Code);
            Assert.AreEqual("2.3", parser("2.3").Code);
            Assert.AreEqual("\"abc\"", parser("\"abc\"").Code);
            Assert.AreEqual("L\"abc\"", parser("L\"abc\"").Code);
            Assert.AreEqual("'abc'", parser("'abc'").Code);
            Assert.AreEqual("L'abc'", parser("L'abc'").Code);
            Assert.AreEqual("true", parser("true").Code);
            Assert.AreEqual("false", parser("false").Code);
            Assert.AreEqual("null", parser("null").Code);
        }

        private void TestParseIdentifierReference(Func<string, NativeXReferenceExpression> parser)
        {
            Assert.AreEqual("abc", parser("abc").ReferencedName);
            Assert.AreEqual("def", parser("def").ReferencedName);
            Assert.AreEqual("exceptionx", parser("exceptionx").ReferencedName);
        }

        private void TestParseInstanceFunctionReferenceInternal(Func<string, NativeXInstanceFunctionExpression> parser)
        {
            {
                var e = parser("Eq<int>::Equal");
                Assert.AreEqual("Eq", e.ConceptName);
                Assert.AreEqual("Equal", e.FunctionName);
                Assert.AreEqual("int", ((NativeXReferenceType)e.Type).ReferencedName);
            }
            {
                var e = parser("Eq<int>::NotEqual");
                Assert.AreEqual("Eq", e.ConceptName);
                Assert.AreEqual("NotEqual", e.FunctionName);
                Assert.AreEqual("int", ((NativeXReferenceType)e.Type).ReferencedName);
            }
        }

        private void TestParseInstanciatedReferenceInternal(Func<string, NativeXInstanciatedExpression> parser)
        {
            {
                var e = parser("Compare<>");
                Assert.AreEqual("Compare", e.ReferencedName);
                Assert.AreEqual(0, e.GenericArguments.Count);
            }
            {
                var e = parser("Compare<int>");
                Assert.AreEqual("Compare", e.ReferencedName);
                Assert.AreEqual(1, e.GenericArguments.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)e.GenericArguments[0]).ReferencedName);
            }
            {
                var e = parser("Compare<int,double,float>");
                Assert.AreEqual("Compare", e.ReferencedName);
                Assert.AreEqual(3, e.GenericArguments.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)e.GenericArguments[0]).ReferencedName);
                Assert.AreEqual("double", ((NativeXReferenceType)e.GenericArguments[1]).ReferencedName);
                Assert.AreEqual("float", ((NativeXReferenceType)e.GenericArguments[2]).ReferencedName);
            }
        }

        private void TestParseResultInternal(Func<string, NativeXFunctionResultExpression> parser)
        {
            Assert.IsNotNull(parser("result"));
        }

        private void TestParseExceptionInternal(Func<string, NativeXExceptionExpression> parser)
        {
            Assert.IsNotNull(parser("exception"));
        }

        private void TestParseCastingInternal(Func<string, NativeXCastingExpression> parser)
        {
            {
                var e = parser("cast<int>(1)");
                Assert.AreEqual("int", ((NativeXReferenceType)e.Type).ReferencedName);
                Assert.AreEqual("1", ((NativeXPrimitiveExpression)e.Operand).Code);
            }
            {
                var e = parser("cast<double>(result)");
                Assert.AreEqual("double", ((NativeXReferenceType)e.Type).ReferencedName);
                Assert.IsNotNull((NativeXFunctionResultExpression)e.Operand);
            }
        }

        private void TestParseExp1(Func<string, NativeXExpression> parser)
        {
            {
                NativeXSubscribeExpression e = (NativeXSubscribeExpression)parser("a[b][c]");
                Assert.AreEqual("c", ((NativeXReferenceExpression)e.Subscribe).ReferencedName);
                NativeXSubscribeExpression e2 = (NativeXSubscribeExpression)e.Operand;
                Assert.AreEqual("b", ((NativeXReferenceExpression)e2.Subscribe).ReferencedName);
                Assert.AreEqual("a", ((NativeXReferenceExpression)e2.Operand).ReferencedName);
            }
            {
                NativeXInvokeExpression e = (NativeXInvokeExpression)parser("a()(b)(c,d)");
                Assert.AreEqual(2, e.Arguments.Count);
                Assert.AreEqual("c", ((NativeXReferenceExpression)e.Arguments[0]).ReferencedName);
                Assert.AreEqual("d", ((NativeXReferenceExpression)e.Arguments[1]).ReferencedName);
                NativeXInvokeExpression e2 = (NativeXInvokeExpression)e.Function;
                Assert.AreEqual(1, e2.Arguments.Count);
                Assert.AreEqual("b", ((NativeXReferenceExpression)e2.Arguments[0]).ReferencedName);
                NativeXInvokeExpression e3 = (NativeXInvokeExpression)e2.Function;
                Assert.AreEqual(0, e3.Arguments.Count);
                Assert.AreEqual("a", ((NativeXReferenceExpression)e3.Function).ReferencedName);
            }
            {
                NativeXPointerMemberExpression e = (NativeXPointerMemberExpression)parser("a.b->c");
                Assert.AreEqual("c", e.MemberName);
                NativeXMemberExpression e2 = (NativeXMemberExpression)e.Operand;
                Assert.AreEqual("b", e2.MemberName);
                Assert.AreEqual("a", ((NativeXReferenceExpression)e2.Operand).ReferencedName);
            }
        }

        private void TestParseUnaryExp(Func<string, NativeXUnaryExpression> parser)
        {
            {
                NativeXExpression e = parser("++--&*-!~a++--");
                {
                    string[] operators = { "++", "--", "&", "*", "-", "!", "~" };
                    foreach (string op in operators)
                    {
                        NativeXUnaryExpression unary = (NativeXUnaryExpression)e;
                        Assert.AreEqual(op, unary.Operator);
                        e = unary.Operand;
                    }
                }
                {
                    string[] operators = { "--", "++" };
                    foreach (string op in operators)
                    {
                        NativeXPostUnaryExpression unary = (NativeXPostUnaryExpression)e;
                        Assert.AreEqual(op, unary.Operator);
                        e = unary.Operand;
                    }
                }
                {
                    Assert.AreEqual("a", ((NativeXReferenceExpression)e).ReferencedName);
                }
            }
        }

        private void TestParseBinaryExp(Func<string, NativeXBinaryExpression> parser)
        {
            NativeXBinaryExpression a = parser("a=(1+2)+3*4");
            Assert.AreEqual("=", a.Operator);
            {
                Assert.AreEqual("a", ((NativeXReferenceExpression)a.LeftOperand).ReferencedName);
            }
            {
                NativeXBinaryExpression b = (NativeXBinaryExpression)a.RightOperand;
                Assert.AreEqual("+", b.Operator);
                {
                    NativeXBinaryExpression c = (NativeXBinaryExpression)b.LeftOperand;
                    Assert.AreEqual("+", c.Operator);
                    Assert.AreEqual("1", ((NativeXPrimitiveExpression)c.LeftOperand).Code);
                    Assert.AreEqual("2", ((NativeXPrimitiveExpression)c.RightOperand).Code);
                }
                {
                    NativeXBinaryExpression c = (NativeXBinaryExpression)b.RightOperand;
                    Assert.AreEqual("*", c.Operator);
                    Assert.AreEqual("3", ((NativeXPrimitiveExpression)c.LeftOperand).Code);
                    Assert.AreEqual("4", ((NativeXPrimitiveExpression)c.RightOperand).Code);
                }
            }
        }

        [TestMethod]
        public void TestParsePrimitive()
        {
            TestParsePrimitiveInternal(s => Parse(s, NativeXCodeParser.ParsePrimitive));
        }

        [TestMethod]
        public void TestParseIdentifierReference()
        {
            TestParseIdentifierReference(s => Parse(s, NativeXCodeParser.ParseIdentifierReference));
        }

        [TestMethod]
        public void TestParseInstanceFunctionReference()
        {
            TestParseInstanceFunctionReferenceInternal(s => Parse(s, NativeXCodeParser.ParseInstanceFunctionReference));
        }

        [TestMethod]
        public void TestParseInstanciatedReference()
        {
            TestParseInstanciatedReferenceInternal(s => Parse(s, NativeXCodeParser.ParseInstanciatedReference));
        }

        [TestMethod]
        public void TestParseResult()
        {
            TestParseResultInternal(s => Parse(s, NativeXCodeParser.ParseResult));
        }

        [TestMethod]
        public void TestParseException()
        {
            TestParseExceptionInternal(s => Parse(s, NativeXCodeParser.ParseException));
        }

        [TestMethod]
        public void TestParseCasting()
        {
            TestParseCastingInternal(s => Parse(s, NativeXCodeParser.ParseCasting));
        }

        [TestMethod]
        public void TestExpression()
        {
            TestParsePrimitiveInternal(s => (NativeXPrimitiveExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseIdentifierReference(s => (NativeXReferenceExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseInstanceFunctionReferenceInternal(s => (NativeXInstanceFunctionExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseInstanciatedReferenceInternal(s => (NativeXInstanciatedExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseResultInternal(s => (NativeXFunctionResultExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseExceptionInternal(s => (NativeXExceptionExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseCastingInternal(s => (NativeXCastingExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseExp1(s => Parse(s, NativeXCodeParser.ParseExpression));
            TestParseUnaryExp(s => (NativeXUnaryExpression)Parse(s, NativeXCodeParser.ParseExpression));
            TestParseBinaryExp(s => (NativeXBinaryExpression)Parse(s, NativeXCodeParser.ParseExpression));
        }

        #endregion

        #region TYPE

        private void TestReferenceTypeInternal(Func<string, NativeXReferenceType> parser)
        {
            Assert.AreEqual("int", parser("int").ReferencedName);
        }

        private void TestFunctionTypeInternal(Func<string, NativeXFunctionType> parser)
        {
            {
                NativeXFunctionType t = parser("function void()");
                Assert.AreEqual("void", ((NativeXReferenceType)t.ReturnType).ReferencedName);
                Assert.AreEqual(0, t.Parameters.Count);
            }
            {
                NativeXFunctionType t = parser("function int(double*)");
                Assert.AreEqual("int", ((NativeXReferenceType)t.ReturnType).ReferencedName);
                Assert.AreEqual(1, t.Parameters.Count);
                Assert.AreEqual("double", ((NativeXReferenceType)((NativeXPointerType)t.Parameters[0]).ElementType).ReferencedName);
            }
            {
                NativeXFunctionType t = parser("function int(double*,string[1])");
                Assert.AreEqual("int", ((NativeXReferenceType)t.ReturnType).ReferencedName);
                Assert.AreEqual(2, t.Parameters.Count);
                Assert.AreEqual("double", ((NativeXReferenceType)((NativeXPointerType)t.Parameters[0]).ElementType).ReferencedName);
                Assert.AreEqual("string", ((NativeXReferenceType)((NativeXArrayType)t.Parameters[1]).ElementType).ReferencedName);
            }
        }

        private void TestInstanciatedTypeInternal(Func<string, NativeXInstanciatedType> parser)
        {
            {
                NativeXInstanciatedType t = parser("void<>");
                Assert.AreEqual("void", ((NativeXReferenceType)t.ElementType).ReferencedName);
                Assert.AreEqual(0, t.GenericArguments.Count);
            }
            {
                NativeXInstanciatedType t = parser("int<double*>");
                Assert.AreEqual("int", ((NativeXReferenceType)t.ElementType).ReferencedName);
                Assert.AreEqual(1, t.GenericArguments.Count);
                Assert.AreEqual("double", ((NativeXReferenceType)((NativeXPointerType)t.GenericArguments[0]).ElementType).ReferencedName);
            }
            {
                NativeXInstanciatedType t = parser("int<double*,string[1]>");
                Assert.AreEqual("int", ((NativeXReferenceType)t.ElementType).ReferencedName);
                Assert.AreEqual(2, t.GenericArguments.Count);
                Assert.AreEqual("double", ((NativeXReferenceType)((NativeXPointerType)t.GenericArguments[0]).ElementType).ReferencedName);
                Assert.AreEqual("string", ((NativeXReferenceType)((NativeXArrayType)t.GenericArguments[1]).ElementType).ReferencedName);
            }
        }

        private void TestPointerTypeInternal(Func<string, NativeXPointerType> parser)
        {
            NativeXPointerType t = parser("double*");
            Assert.AreEqual("double", ((NativeXReferenceType)t.ElementType).ReferencedName);
        }

        private void TestArrayTypeInternal(Func<string, NativeXArrayType> parser)
        {
            NativeXArrayType t = parser("string[1]");
            Assert.AreEqual("string", ((NativeXReferenceType)t.ElementType).ReferencedName);
            Assert.AreEqual("1", ((NativeXPrimitiveExpression)t.Size).Code);
        }

        [TestMethod]
        public void TestReferenceType()
        {
            TestReferenceTypeInternal(s => Parse(s, NativeXCodeParser.ParseReferenceType));
        }

        [TestMethod]
        public void TestFunctionType()
        {
            TestFunctionTypeInternal(s => Parse(s, NativeXCodeParser.ParseFunctionType));
        }

        [TestMethod]
        public void TestInstanciatedType()
        {
            TestInstanciatedTypeInternal(s => Parse(s, NativeXCodeParser.ParseInstanciatedType));
        }

        [TestMethod]
        public void TestType()
        {
            TestReferenceTypeInternal(s => (NativeXReferenceType)Parse(s, NativeXCodeParser.ParseType));
            TestFunctionTypeInternal(s => (NativeXFunctionType)Parse(s, NativeXCodeParser.ParseType));
            TestInstanciatedTypeInternal(s => (NativeXInstanciatedType)Parse(s, NativeXCodeParser.ParseType));
            TestPointerTypeInternal(s => (NativeXPointerType)Parse(s, NativeXCodeParser.ParseType));
            TestArrayTypeInternal(s => (NativeXArrayType)Parse(s, NativeXCodeParser.ParseType));
        }

        #endregion

        #region STATEMENT

        private void TestParseEmptyStatementInternal(Func<string, NativeXEmptyStatement> parser)
        {
            Assert.IsNotNull(parser(";"));
        }

        private void TestParseVariableStatementInternal(Func<string, NativeXVariableStatement> parser)
        {
        }

        private void TestParseIfStatementInternal(Func<string, NativeXIfStatement> parser)
        {
        }

        private void TestParseBreakStatementInternal(Func<string, NativeXBreakStatement> parser)
        {
            Assert.IsNotNull(parser("break;"));
        }

        private void TestParseContinueStatementInternal(Func<string, NativeXContinueStatement> parser)
        {
            Assert.IsNotNull(parser("continue;"));
        }

        private void TestParseExitStatementInternal(Func<string, NativeXReturnStatement> parser)
        {
            Assert.IsNotNull(parser("exit;"));
        }

        private void TestParseCompositeStatementInternal(Func<string, NativeXCompositeStatement> parser)
        {
        }

        private void TestParseDoWhileStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
        }

        private void TestParseLoopStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
        }

        private void TestParseWhileDoStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
        }

        private void TestParseForStatementInternal(Func<string, NativeXForStatement> parser)
        {
        }

        private void TestParseTryCatchStatementInternal(Func<string, NativeXTryCatchStatement> parser)
        {
        }

        private void TestParseThrowStatementInternal(Func<string, NativeXThrowStatement> parser)
        {
            Assert.IsNotNull(parser("throw;"));
            NativeXThrowStatement s = parser("throw 1;");
            Assert.AreEqual("1", ((NativeXPrimitiveExpression)s.ExceptionExpression).Code);
        }

        private void TestParseExpressionStatementInternal(Func<string, NativeXExpressionStatement> parser)
        {
            NativeXExpressionStatement s = parser("1+2;");
            NativeXBinaryExpression a = (NativeXBinaryExpression)s.Expression;
            Assert.AreEqual("1", ((NativeXPrimitiveExpression)a.LeftOperand).Code);
            Assert.AreEqual("2", ((NativeXPrimitiveExpression)a.RightOperand).Code);
        }

        [TestMethod]
        public void TestParseStatement()
        {
            TestParseEmptyStatementInternal(s => Parse(s, NativeXCodeParser.ParseEmptyStatement));
            TestParseVariableStatementInternal(s => Parse(s, NativeXCodeParser.ParseVariableStatement));
            TestParseIfStatementInternal(s => Parse(s, NativeXCodeParser.ParseIfStatement));
            TestParseBreakStatementInternal(s => Parse(s, NativeXCodeParser.ParseBreakStatement));
            TestParseContinueStatementInternal(s => Parse(s, NativeXCodeParser.ParseContinueStatement));
            TestParseExitStatementInternal(s => Parse(s, NativeXCodeParser.ParseExitStatement));
            TestParseCompositeStatementInternal(s => Parse(s, NativeXCodeParser.ParseCompositeStatement));
            TestParseDoWhileStatementInternal(s => Parse(s, NativeXCodeParser.ParseDoWhileStatement));
            TestParseLoopStatementInternal(s => Parse(s, NativeXCodeParser.ParseLoopStatement));
            TestParseWhileDoStatementInternal(s => Parse(s, NativeXCodeParser.ParseWhileDoStatement));
            TestParseForStatementInternal(s => Parse(s, NativeXCodeParser.ParseForStatement));
            TestParseTryCatchStatementInternal(s => Parse(s, NativeXCodeParser.ParseTryCatchStatement));
            TestParseThrowStatementInternal(s => Parse(s, NativeXCodeParser.ParseThrowStatement));
            TestParseExpressionStatementInternal(s => Parse(s, NativeXCodeParser.ParseExpressionStatement));

            TestParseEmptyStatementInternal(s => (NativeXEmptyStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseVariableStatementInternal(s => (NativeXVariableStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseIfStatementInternal(s => (NativeXIfStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseBreakStatementInternal(s => (NativeXBreakStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseContinueStatementInternal(s => (NativeXContinueStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseExitStatementInternal(s => (NativeXReturnStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseCompositeStatementInternal(s => (NativeXCompositeStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseDoWhileStatementInternal(s => (NativeXWhileStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseLoopStatementInternal(s => (NativeXWhileStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseWhileDoStatementInternal(s => (NativeXWhileStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseForStatementInternal(s => (NativeXForStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseTryCatchStatementInternal(s => (NativeXTryCatchStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseThrowStatementInternal(s => (NativeXThrowStatement)Parse(s, NativeXCodeParser.ParseStatement));
            TestParseExpressionStatementInternal(s => (NativeXExpressionStatement)Parse(s, NativeXCodeParser.ParseStatement));
        }

        #endregion

        #region DECLARATION
        #endregion

        [TestMethod]
        public void RunParserGenerator()
        {
            NativeXCodeParserGrammar.Build();
        }
    }
}
