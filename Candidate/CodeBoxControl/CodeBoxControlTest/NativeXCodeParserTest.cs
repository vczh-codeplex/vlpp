using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeForm.NativeX.SyntaxTree;
using CodeBoxControl.CodeProvider;
using CodeForm.NativeX;

namespace CodeBoxControlTest
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
            Assert.AreEqual("result", parser("result").Code);
        }

        private void TestParseExceptionInternal(Func<string, NativeXExceptionExpression> parser)
        {
            Assert.AreEqual("exception", parser("exception").Code);
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
                Assert.AreEqual("result", ((NativeXFunctionResultExpression)e.Operand).Code);
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
            TestParseCastingInternal(s => (NativeXCastingExpression)Parse(s, NativeXCodeParser.ParseCasting));
        }
    }
}
