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

        [TestMethod]
        public void TestParserPrimitive()
        {
            Assert.AreEqual("1", Parse("1", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("2.3", Parse("2.3", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("\"abc\"", Parse("\"abc\"", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("L\"abc\"", Parse("L\"abc\"", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("'abc'", Parse("'abc'", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("L'abc'", Parse("L'abc'", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("true", Parse("true", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("false", Parse("false", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("exception", Parse("exception", NativeXCodeParser.ParsePrimitive).Code);
            Assert.AreEqual("null", Parse("null", NativeXCodeParser.ParsePrimitive).Code);
        }
    }
}
