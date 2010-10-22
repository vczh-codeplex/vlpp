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
            T result = parser(tokens, ref currentToken, ref parseSuccess);
            Assert.AreEqual(tokens.Count, currentToken);
            return result;
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
            {
                NativeXVariableStatement s = parser("variable function int(double) converter;");
                Assert.AreEqual("converter", s.Name);
                Assert.IsNull(s.Initializer);
                NativeXFunctionType t = (NativeXFunctionType)s.Type;
                Assert.AreEqual("int", ((NativeXReferenceType)t.ReturnType).ReferencedName);
                Assert.AreEqual(1, t.Parameters.Count);
                Assert.AreEqual("double", ((NativeXReferenceType)t.Parameters[0]).ReferencedName);
            }
            {
                NativeXVariableStatement s = parser("variable vector<string> list = null;");
                Assert.AreEqual("list", s.Name);
                Assert.AreEqual("null", ((NativeXPrimitiveExpression)s.Initializer).Code);
                NativeXInstanciatedType t = (NativeXInstanciatedType)s.Type;
                Assert.AreEqual("vector", ((NativeXReferenceType)t.ElementType).ReferencedName);
                Assert.AreEqual(1, t.GenericArguments.Count);
                Assert.AreEqual("string", ((NativeXReferenceType)t.GenericArguments[0]).ReferencedName);
            }
        }

        private void TestParseIfStatementInternal(Func<string, NativeXIfStatement> parser)
        {
            {
                NativeXIfStatement s = parser("if(true)fuck();");
                Assert.AreEqual("true", ((NativeXPrimitiveExpression)s.Condition).Code);
                {
                    NativeXInvokeExpression e = (NativeXInvokeExpression)((NativeXExpressionStatement)s.TrueStatement).Expression;
                    Assert.AreEqual("fuck", ((NativeXReferenceExpression)e.Function).ReferencedName);
                    Assert.AreEqual(0, e.Arguments.Count);
                }
                Assert.IsNull(s.FalseStatement);
            }
            {
                NativeXIfStatement s = parser("if(true)fuck();else damn();");
                Assert.AreEqual("true", ((NativeXPrimitiveExpression)s.Condition).Code);
                {
                    NativeXInvokeExpression e = (NativeXInvokeExpression)((NativeXExpressionStatement)s.TrueStatement).Expression;
                    Assert.AreEqual("fuck", ((NativeXReferenceExpression)e.Function).ReferencedName);
                    Assert.AreEqual(0, e.Arguments.Count);
                }
                {
                    NativeXInvokeExpression e = (NativeXInvokeExpression)((NativeXExpressionStatement)s.FalseStatement).Expression;
                    Assert.AreEqual("damn", ((NativeXReferenceExpression)e.Function).ReferencedName);
                    Assert.AreEqual(0, e.Arguments.Count);
                }
            }
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
            NativeXCompositeStatement s = parser("{_1();_2();_3();_4();}");
            Assert.AreEqual(4, s.Statements.Count);
            for (int i = 1; i >= 4; i++)
            {
                Assert.AreEqual(i.ToString(), ((NativeXReferenceExpression)((NativeXExpressionStatement)s.Statements[i - 1]).Expression).ReferencedName);
            }
        }

        private void TestParseDoWhileStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
            NativeXWhileStatement s = parser("do x(); while(y);");
            NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.Statement).Expression;
            Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
            Assert.AreEqual(0, a.Arguments.Count);
            Assert.IsNull(s.BeginCondition);
            Assert.AreEqual("y", ((NativeXReferenceExpression)s.EndCondition).ReferencedName);
        }

        private void TestParseLoopStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
            NativeXWhileStatement s = parser("loop x();");
            NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.Statement).Expression;
            Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
            Assert.AreEqual(0, a.Arguments.Count);
            Assert.IsNull(s.BeginCondition);
            Assert.IsNull(s.EndCondition);
        }

        private void TestParseWhileDoStatementInternal(Func<string, NativeXWhileStatement> parser)
        {
            {
                NativeXWhileStatement s = parser("while(y) x();");
                NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.Statement).Expression;
                Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
                Assert.AreEqual(0, a.Arguments.Count);
                Assert.AreEqual("y", ((NativeXReferenceExpression)s.BeginCondition).ReferencedName);
                Assert.IsNull(s.EndCondition);
            }
            {
                NativeXWhileStatement s = parser("while(y) x(); when(z);");
                NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.Statement).Expression;
                Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
                Assert.AreEqual(0, a.Arguments.Count);
                Assert.AreEqual("y", ((NativeXReferenceExpression)s.BeginCondition).ReferencedName);
                Assert.AreEqual("z", ((NativeXReferenceExpression)s.EndCondition).ReferencedName);
            }
        }

        private void TestParseForStatementInternal(Func<string, NativeXForStatement> parser)
        {
            NativeXForStatement s = parser("for variable int i = 0; variable int j = 0; when (i - j > 5) with i+=2; j+=1; do x();");
            Assert.AreEqual(2, s.Initializer.Count);
            {
                NativeXVariableStatement s1 = (NativeXVariableStatement)s.Initializer[0];
                Assert.AreEqual("i", s1.Name);
                Assert.AreEqual("int", ((NativeXReferenceType)s1.Type).ReferencedName);
                Assert.AreEqual("0", ((NativeXPrimitiveExpression)s1.Initializer).Code);
            }
            {
                NativeXVariableStatement s2 = (NativeXVariableStatement)s.Initializer[1];
                Assert.AreEqual("j", s2.Name);
                Assert.AreEqual("int", ((NativeXReferenceType)s2.Type).ReferencedName);
                Assert.AreEqual("0", ((NativeXPrimitiveExpression)s2.Initializer).Code);
            }
            Assert.AreEqual(2, s.SideEffect.Count);
            {
                NativeXBinaryExpression e1 = (NativeXBinaryExpression)((NativeXExpressionStatement)s.SideEffect[0]).Expression;
                Assert.AreEqual("+=", e1.Operator);
                Assert.AreEqual("i", ((NativeXReferenceExpression)e1.LeftOperand).ReferencedName);
                Assert.AreEqual("2", ((NativeXPrimitiveExpression)e1.RightOperand).Code);
            }
            {
                NativeXBinaryExpression e2 = (NativeXBinaryExpression)((NativeXExpressionStatement)s.SideEffect[1]).Expression;
                Assert.AreEqual("+=", e2.Operator);
                Assert.AreEqual("j", ((NativeXReferenceExpression)e2.LeftOperand).ReferencedName);
                Assert.AreEqual("1", ((NativeXPrimitiveExpression)e2.RightOperand).Code);
            }
            {
                NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.Statement).Expression;
                Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
                Assert.AreEqual(0, a.Arguments.Count);
            }
            {
                NativeXBinaryExpression e1 = (NativeXBinaryExpression)s.Condition;
                Assert.AreEqual(">", e1.Operator);
                Assert.AreEqual("5", ((NativeXPrimitiveExpression)e1.RightOperand).Code);

                NativeXBinaryExpression e2 = (NativeXBinaryExpression)e1.LeftOperand;
                Assert.AreEqual("-", e2.Operator);
                Assert.AreEqual("i", ((NativeXReferenceExpression)e2.LeftOperand).ReferencedName);
                Assert.AreEqual("j", ((NativeXReferenceExpression)e2.RightOperand).ReferencedName);
            }
        }

        private void TestParseTryCatchStatementInternal(Func<string, NativeXTryCatchStatement> parser)
        {
            NativeXTryCatchStatement s = parser("try x(); catch y();");
            NativeXInvokeExpression a = (NativeXInvokeExpression)((NativeXExpressionStatement)s.TryStatement).Expression;
            Assert.AreEqual("x", ((NativeXReferenceExpression)a.Function).ReferencedName);
            Assert.AreEqual(0, a.Arguments.Count);
            NativeXInvokeExpression b = (NativeXInvokeExpression)((NativeXExpressionStatement)s.CatchStatement).Expression;
            Assert.AreEqual("y", ((NativeXReferenceExpression)b.Function).ReferencedName);
            Assert.AreEqual(0, b.Arguments.Count);
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

        private const string g1 = "generic<> ";
        private const string g2 = "generic<U>where U:I1 ";
        private const string g3 = "generic<U,V>where U:I1,V:I2 ";

        private readonly Action<NativeXDeclaration> vg1 = d =>
        {
            Assert.AreEqual(0, d.GenericParameters.Count);
            Assert.IsNull(d.GenericConstraints);
        };

        private readonly Action<NativeXDeclaration> vg2 = d =>
        {
            Assert.AreEqual(1, d.GenericParameters.Count);
            Assert.AreEqual("U", d.GenericParameters[0].ParameterName);
            Assert.AreEqual(1, d.GenericConstraints.Count);
            Assert.AreEqual("U", d.GenericConstraints[0].ParameterName);
            Assert.AreEqual("I1", d.GenericConstraints[0].ConceptName);
        };

        private readonly Action<NativeXDeclaration> vg3 = d =>
        {
            Assert.AreEqual(2, d.GenericParameters.Count);
            Assert.AreEqual("U", d.GenericParameters[0].ParameterName);
            Assert.AreEqual("V", d.GenericParameters[1].ParameterName);
            Assert.AreEqual(2, d.GenericConstraints.Count);
            Assert.AreEqual("U", d.GenericConstraints[0].ParameterName);
            Assert.AreEqual("I1", d.GenericConstraints[0].ConceptName);
            Assert.AreEqual("V", d.GenericConstraints[1].ParameterName);
            Assert.AreEqual("I2", d.GenericConstraints[1].ConceptName);
        };

        private void TestParseFunctionDeclarationInternal(Func<string, NativeXFunctionDeclaration> parser)
        {
            Action<NativeXFunctionDeclaration> vd_common = d =>
            {
                Assert.AreEqual("Sum", d.Name);
                Assert.AreEqual("int", ((NativeXReferenceType)d.ReturnType).ReferencedName);
                Assert.AreEqual(2, d.Parameters.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)((NativeXPointerType)d.Parameters[0].Type).ElementType).ReferencedName);
                Assert.AreEqual("numbers", d.Parameters[0].Name);
                Assert.AreEqual("int", ((NativeXReferenceType)d.Parameters[1].Type).ReferencedName);
                Assert.AreEqual("count", d.Parameters[1].Name);
            };
            Action<NativeXFunctionDeclaration> vd1 = d =>
            {
                Assert.IsFalse(d.Foreign);
                Assert.IsNull(d.Linking);

                NativeXCompositeStatement s = (NativeXCompositeStatement)d.Statement;
                Assert.AreEqual(1, s.Statements.Count);
                NativeXBinaryExpression e = (NativeXBinaryExpression)((NativeXExpressionStatement)s.Statements[0]).Expression;
                Assert.IsNotNull((NativeXFunctionResultExpression)e.LeftOperand);
                Assert.AreEqual("0", ((NativeXPrimitiveExpression)e.RightOperand).Code);
            };
            Action<NativeXFunctionDeclaration> vd2 = d =>
            {
                Assert.IsFalse(d.Foreign);
                Assert.IsNull(d.Statement);
                Assert.AreEqual("Utility", d.Linking.LinkingAssembly);
                Assert.AreEqual("Sum", d.Linking.LinkingSymbol);
            };
            Action<NativeXFunctionDeclaration> vd3 = d =>
            {
                Assert.IsTrue(d.Foreign);
                Assert.IsNull(d.Linking);
                Assert.IsNull(d.Statement);
            };
            string d1 = "function int Sum(int* numbers, int count){result = 0;}";
            string d2 = "function int Sum(int* numbers, int count) alias Utility.Sum;";
            string d3 = "foreign function int Sum(int* numbers, int count);";
            {
                NativeXFunctionDeclaration d = parser(d1);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXFunctionDeclaration d = parser(d2);
                vd_common(d);
                vd2(d);
            }
            {
                NativeXFunctionDeclaration d = parser(d3);
                vd_common(d);
                vd3(d);
            }
            {
                NativeXFunctionDeclaration d = parser(g1 + d1);
                vg1(d);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXFunctionDeclaration d = parser(g2 + d2);
                vg2(d);
                vd_common(d);
                vd2(d);
            }
            {
                NativeXFunctionDeclaration d = parser(g3 + d3);
                vg3(d);
                vd_common(d);
                vd3(d);
            }
        }

        private void TestParseTypeRenameDeclarationInternal(Func<string, NativeXTypeRenameDeclaration> parser)
        {
            Action<NativeXTypeRenameDeclaration> vd1 = d =>
            {
                Assert.AreEqual("IntList", d.Name);
                NativeXInstanciatedType t = (NativeXInstanciatedType)d.Type;
                Assert.AreEqual("List", t.ElementType.ReferencedName);
                Assert.AreEqual(1, t.GenericArguments.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)t.GenericArguments[0]).ReferencedName);
            };
            string d1 = "type IntList = List<int>;";
            {
                NativeXTypeRenameDeclaration d = parser(d1);
                vd1(d);
            }
            {
                NativeXTypeRenameDeclaration d = parser(g1 + d1);
                vg1(d);
                vd1(d);
            }
            {
                NativeXTypeRenameDeclaration d = parser(g2 + d1);
                vg2(d);
                vd1(d);
            }
            {
                NativeXTypeRenameDeclaration d = parser(g3 + d1);
                vg3(d);
                vd1(d);
            }
        }

        private void TestParseVariableDeclarationInternal(Func<string, NativeXVariableDeclaration> parser)
        {
            Action<NativeXVariableDeclaration> vd_common = d =>
            {
                Assert.AreEqual("a", d.Name);
                NativeXInstanciatedType t = (NativeXInstanciatedType)d.Type;
                Assert.AreEqual("List", t.ElementType.ReferencedName);
                Assert.AreEqual(1, t.GenericArguments.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)t.GenericArguments[0]).ReferencedName);
            };
            Action<NativeXVariableDeclaration> vd1 = d =>
            {
                Assert.IsNull(d.Initializer);
                Assert.IsNull(d.Linking);
            };
            Action<NativeXVariableDeclaration> vd2 = d =>
            {
                Assert.IsNull(d.Initializer);
                Assert.AreEqual("Utility", d.Linking.LinkingAssembly);
                Assert.AreEqual("Sum", d.Linking.LinkingSymbol);
            };
            Action<NativeXVariableDeclaration> vd3 = d =>
            {
                Assert.IsNull(d.Linking);
                NativeXBinaryExpression e = (NativeXBinaryExpression)d.Initializer;
                Assert.AreEqual("1", ((NativeXPrimitiveExpression)e.LeftOperand).Code);
                Assert.AreEqual("2", ((NativeXPrimitiveExpression)e.RightOperand).Code);
            };
            string d1 = "variable List<int> a;";
            string d2 = "variable List<int> a alias Utility.Sum;";
            string d3 = "variable List<int> a = 1+2;";
            {
                NativeXVariableDeclaration d = parser(d1);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXVariableDeclaration d = parser(d2);
                vd_common(d);
                vd2(d);
            }
            {
                NativeXVariableDeclaration d = parser(d3);
                vd_common(d);
                vd3(d);
            }
            {
                NativeXVariableDeclaration d = parser(g1 + d1);
                vg1(d);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXVariableDeclaration d = parser(g2 + d2);
                vg2(d);
                vd_common(d);
                vd2(d);
            }
            {
                NativeXVariableDeclaration d = parser(g3 + d3);
                vg3(d);
                vd_common(d);
                vd3(d);
            }
        }

        private void TestParseStructureDeclarationInternal(Func<string, NativeXStructureDeclaration> parser)
        {
            Action<NativeXStructureDeclaration> vd1 = d =>
            {
                Assert.AreEqual("List", d.Name);
                Assert.IsNull(d.Linking);
                Assert.IsNull(d.Members);
            };
            Action<NativeXStructureDeclaration> vd2 = d =>
            {
                Assert.AreEqual("List", d.Name);
                Assert.IsNull(d.Linking);
                Assert.AreEqual(2, d.Members.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)d.Members[0].Type).ReferencedName);
                Assert.AreEqual("count", d.Members[0].Name);
                Assert.AreEqual("int", ((NativeXReferenceType)((NativeXPointerType)d.Members[1].Type).ElementType).ReferencedName);
                Assert.AreEqual("items", d.Members[1].Name);
            };
            Action<NativeXStructureDeclaration> vd3 = d =>
            {
                Assert.AreEqual("List", d.Name);
                Assert.AreEqual("Utility", d.Linking.LinkingAssembly);
                Assert.AreEqual("List", d.Linking.LinkingSymbol);
                Assert.AreEqual(2, d.Members.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)d.Members[0].Type).ReferencedName);
                Assert.AreEqual("count", d.Members[0].Name);
                Assert.AreEqual("int", ((NativeXReferenceType)((NativeXPointerType)d.Members[1].Type).ElementType).ReferencedName);
                Assert.AreEqual("items", d.Members[1].Name);
            };
            string d1 = "structure List;";
            string d2 = "structure List{int count;int* items;}";
            string d3 = "structure List alias Utility.List {int count;int* items;}";
            {
                NativeXStructureDeclaration d = parser(d1);
                vd1(d);
            }
            {
                NativeXStructureDeclaration d = parser(d2);
                vd2(d);
            }
            {
                NativeXStructureDeclaration d = parser(d3);
                vd3(d);
            }
            {
                NativeXStructureDeclaration d = parser(g1 + d1);
                vg1(d);
                vd1(d);
            }
            {
                NativeXStructureDeclaration d = parser(g2 + d2);
                vg2(d);
                vd2(d);
            }
            {
                NativeXStructureDeclaration d = parser(g3 + d3);
                vg3(d);
                vd3(d);
            }
        }

        private void TestParseInstanceDeclarationInternal(Func<string, NativeXInstanceDeclaration> parser)
        {
            Action<NativeXInstanceDeclaration> vd1 = d =>
            {
                Assert.IsNull(d.Name);
                Assert.IsNull(d.Linking);
                Assert.AreEqual("int", d.Type.ReferencedName);
                Assert.AreEqual("Eq", d.ConceptName);
                Assert.IsNull(d.Functions);
            };
            Action<NativeXInstanceDeclaration> vd2 = d =>
            {
                Assert.IsNull(d.Name);
                Assert.IsNull(d.Linking);
                Assert.AreEqual("int", d.Type.ReferencedName);
                Assert.AreEqual("Eq", d.ConceptName);
                Assert.AreEqual(2, d.Functions.Count);
                Assert.AreEqual("Equal", d.Functions[0].Name);
                Assert.AreEqual("IntEqual", ((NativeXReferenceExpression)d.Functions[0].Expression).ReferencedName);
                Assert.AreEqual("NotEqual", d.Functions[1].Name);
                Assert.AreEqual("IntNotEqual", ((NativeXReferenceExpression)d.Functions[1].Expression).ReferencedName);
            };
            string d1 = "instance int : Eq;";
            string d2 = "instance int : Eq{Equal = IntEqual; NotEqual = IntNotEqual;}";
            {
                NativeXInstanceDeclaration d = parser(d1);
                vd1(d);
            }
            {
                NativeXInstanceDeclaration d = parser(d2);
                vd2(d);
            }
            {
                NativeXInstanceDeclaration d = parser(g1 + d1);
                vg1(d);
                vd1(d);
            }
            {
                NativeXInstanceDeclaration d = parser(g2 + d2);
                vg2(d);
                vd2(d);
            }
        }

        private void TestParseConceptDeclarationInternal(Func<string, NativeXConceptDeclaration> parser)
        {
            Action<NativeXType> vt = t =>
            {
                NativeXFunctionType ft = (NativeXFunctionType)t;
                Assert.AreEqual("bool", ((NativeXReferenceType)ft.ReturnType).ReferencedName);
                Assert.AreEqual(2, ft.Parameters.Count);
                Assert.AreEqual("int", ((NativeXReferenceType)ft.Parameters[0]).ReferencedName);
                Assert.AreEqual("int", ((NativeXReferenceType)ft.Parameters[1]).ReferencedName);
            };
            Action<NativeXConceptDeclaration> vd_common = d =>
            {
                Assert.AreEqual("Eq", d.Name);
                Assert.AreEqual("T", d.ConceptType);
                Assert.AreEqual(2, d.Functions.Count);
                Assert.AreEqual("Equal", d.Functions[0].Name);
                vt(d.Functions[0].Type);
                Assert.AreEqual("NotEqual", d.Functions[1].Name);
                vt(d.Functions[1].Type);
            };
            Action<NativeXConceptDeclaration> vd1 = d =>
            {
                Assert.IsNull(d.Linking);
            };
            Action<NativeXConceptDeclaration> vd2 = d =>
            {
                Assert.AreEqual("Utility", d.Linking.LinkingAssembly);
                Assert.AreEqual("Eq", d.Linking.LinkingSymbol);
            };
            string d1 = "concept T : Eq{Equal = function bool(int,int); NotEqual = function bool(int,int);}";
            string d2 = "concept T : Eq alias Utility.Eq{Equal = function bool(int,int); NotEqual = function bool(int,int);}";
            {
                NativeXConceptDeclaration d = parser(d1);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXConceptDeclaration d = parser(d2);
                vd_common(d);
                vd2(d);
            }
            {
                NativeXConceptDeclaration d = parser(g1 + d1);
                vg1(d);
                vd_common(d);
                vd1(d);
            }
            {
                NativeXConceptDeclaration d = parser(g2 + d2);
                vg2(d);
                vd_common(d);
                vd2(d);
            }
        }

        [TestMethod]
        public void TestParseDeclaration()
        {
            TestParseFunctionDeclarationInternal(s => (NativeXFunctionDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
            TestParseTypeRenameDeclarationInternal(s => (NativeXTypeRenameDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
            TestParseVariableDeclarationInternal(s => (NativeXVariableDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
            TestParseStructureDeclarationInternal(s => (NativeXStructureDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
            TestParseInstanceDeclarationInternal(s => (NativeXInstanceDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
            TestParseConceptDeclarationInternal(s => (NativeXConceptDeclaration)Parse(s, NativeXCodeParser.ParseDeclaration));
        }

        #endregion

        [TestMethod]
        public void RunParserGenerator()
        {
            NativeXCodeParserGrammar.Build();
        }
    }
}
