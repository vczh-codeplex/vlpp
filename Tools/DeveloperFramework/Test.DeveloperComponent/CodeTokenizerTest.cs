using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Developer.LanguageProvider;
using Developer.WinFormControls.Core;

namespace Test.DeveloperComponent
{
    [TestClass]
    public class CodeTokenizerTest
    {
        [TestMethod]
        public void TestTokenizerParsing()
        {
            TestTokenizer tokenizer = new TestTokenizer();
            {
                string input = "abc def123.456ghi7...__xx;";
                List<CodeToken> tokens = tokenizer.Tokenize(input.ToCharArray());

                Assert.AreEqual(TestTokenizer.Id, tokens[0].Id);
                Assert.AreEqual(new TextPosition(0, 0), tokens[0].Start);
                Assert.AreEqual(new TextPosition(0, 3), tokens[0].End);
                Assert.AreEqual("abc", tokens[0].Value);

                Assert.AreEqual(-1, tokens[1].Id);
                Assert.AreEqual(new TextPosition(0, 3), tokens[1].Start);
                Assert.AreEqual(new TextPosition(0, 4), tokens[1].End);
                Assert.AreEqual(" ", tokens[1].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[2].Id);
                Assert.AreEqual(new TextPosition(0, 4), tokens[2].Start);
                Assert.AreEqual(new TextPosition(0, 10), tokens[2].End);
                Assert.AreEqual("def123", tokens[2].Value);

                Assert.AreEqual(-1, tokens[3].Id);
                Assert.AreEqual(new TextPosition(0, 10), tokens[3].Start);
                Assert.AreEqual(new TextPosition(0, 11), tokens[3].End);
                Assert.AreEqual(".", tokens[3].Value);

                Assert.AreEqual(TestTokenizer.Integer, tokens[4].Id);
                Assert.AreEqual(new TextPosition(0, 11), tokens[4].Start);
                Assert.AreEqual(new TextPosition(0, 14), tokens[4].End);
                Assert.AreEqual("456", tokens[4].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[5].Id);
                Assert.AreEqual(new TextPosition(0, 14), tokens[5].Start);
                Assert.AreEqual(new TextPosition(0, 18), tokens[5].End);
                Assert.AreEqual("ghi7", tokens[5].Value);

                Assert.AreEqual(-1, tokens[6].Id);
                Assert.AreEqual(new TextPosition(0, 18), tokens[6].Start);
                Assert.AreEqual(new TextPosition(0, 19), tokens[6].End);
                Assert.AreEqual(".", tokens[6].Value);

                Assert.AreEqual(-1, tokens[7].Id);
                Assert.AreEqual(new TextPosition(0, 19), tokens[7].Start);
                Assert.AreEqual(new TextPosition(0, 20), tokens[7].End);
                Assert.AreEqual(".", tokens[7].Value);

                Assert.AreEqual(-1, tokens[8].Id);
                Assert.AreEqual(new TextPosition(0, 20), tokens[8].Start);
                Assert.AreEqual(new TextPosition(0, 21), tokens[8].End);
                Assert.AreEqual(".", tokens[8].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[9].Id);
                Assert.AreEqual(new TextPosition(0, 21), tokens[9].Start);
                Assert.AreEqual(new TextPosition(0, 25), tokens[9].End);
                Assert.AreEqual("__xx", tokens[9].Value);

                Assert.AreEqual(-1, tokens[10].Id);
                Assert.AreEqual(new TextPosition(0, 25), tokens[10].Start);
                Assert.AreEqual(new TextPosition(0, 26), tokens[10].End);
                Assert.AreEqual(";", tokens[10].Value);
            }
            {
                string input = "abc def;123.456gh?7...__xx;";
                List<CodeToken> tokens = tokenizer.Tokenize(input.ToCharArray());

                Assert.AreEqual(TestTokenizer.Id, tokens[0].Id);
                Assert.AreEqual(new TextPosition(0, 0), tokens[0].Start);
                Assert.AreEqual(new TextPosition(0, 3), tokens[0].End);
                Assert.AreEqual("abc", tokens[0].Value);

                Assert.AreEqual(-1, tokens[1].Id);
                Assert.AreEqual(new TextPosition(0, 3), tokens[1].Start);
                Assert.AreEqual(new TextPosition(0, 4), tokens[1].End);
                Assert.AreEqual(" ", tokens[1].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[2].Id);
                Assert.AreEqual(new TextPosition(0, 4), tokens[2].Start);
                Assert.AreEqual(new TextPosition(0, 7), tokens[2].End);
                Assert.AreEqual("def", tokens[2].Value);

                Assert.AreEqual(-1, tokens[3].Id);
                Assert.AreEqual(new TextPosition(0, 7), tokens[3].Start);
                Assert.AreEqual(new TextPosition(0, 8), tokens[3].End);
                Assert.AreEqual(";", tokens[3].Value);

                Assert.AreEqual(TestTokenizer.Double, tokens[4].Id);
                Assert.AreEqual(new TextPosition(0, 8), tokens[4].Start);
                Assert.AreEqual(new TextPosition(0, 15), tokens[4].End);
                Assert.AreEqual("123.456", tokens[4].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[5].Id);
                Assert.AreEqual(new TextPosition(0, 15), tokens[5].Start);
                Assert.AreEqual(new TextPosition(0, 17), tokens[5].End);
                Assert.AreEqual("gh", tokens[5].Value);

                Assert.AreEqual(-1, tokens[6].Id);
                Assert.AreEqual(new TextPosition(0, 17), tokens[6].Start);
                Assert.AreEqual(new TextPosition(0, 18), tokens[6].End);
                Assert.AreEqual("?", tokens[6].Value);

                Assert.AreEqual(TestTokenizer.Integer, tokens[7].Id);
                Assert.AreEqual(new TextPosition(0, 18), tokens[7].Start);
                Assert.AreEqual(new TextPosition(0, 19), tokens[7].End);
                Assert.AreEqual("7", tokens[7].Value);

                Assert.AreEqual(-1, tokens[8].Id);
                Assert.AreEqual(new TextPosition(0, 19), tokens[8].Start);
                Assert.AreEqual(new TextPosition(0, 20), tokens[8].End);
                Assert.AreEqual(".", tokens[8].Value);

                Assert.AreEqual(-1, tokens[9].Id);
                Assert.AreEqual(new TextPosition(0, 20), tokens[9].Start);
                Assert.AreEqual(new TextPosition(0, 21), tokens[9].End);
                Assert.AreEqual(".", tokens[9].Value);

                Assert.AreEqual(-1, tokens[10].Id);
                Assert.AreEqual(new TextPosition(0, 21), tokens[10].Start);
                Assert.AreEqual(new TextPosition(0, 22), tokens[10].End);
                Assert.AreEqual(".", tokens[10].Value);

                Assert.AreEqual(TestTokenizer.Id, tokens[11].Id);
                Assert.AreEqual(new TextPosition(0, 22), tokens[11].Start);
                Assert.AreEqual(new TextPosition(0, 26), tokens[11].End);
                Assert.AreEqual("__xx", tokens[11].Value);

                Assert.AreEqual(-1, tokens[12].Id);
                Assert.AreEqual(new TextPosition(0, 26), tokens[12].Start);
                Assert.AreEqual(new TextPosition(0, 27), tokens[12].End);
                Assert.AreEqual(";", tokens[12].Value);
            }
        }

        class TestTokenizer : CodeTokenizer
        {
            public const int Id = 0;
            public const int Integer = 1;
            public const int Double = 2;

            public TestTokenizer()
            {
                charset = new int[char.MaxValue + 1];
                for (char c = '0'; c <= '9'; c++)
                    charset[c] = 1;
                charset['.'] = 2;
                for (char c = 'a'; c <= 'z'; c++)
                    charset[c] = 3;
                for (char c = 'A'; c <= 'Z'; c++)
                    charset[c] = 3;
                charset['_'] = 3;

                transitions = new int[5, 4];

                transitions[0, 0] = -1;
                transitions[0, 1] = 1;
                transitions[0, 2] = -1;
                transitions[0, 3] = 4;

                transitions[1, 0] = -1;
                transitions[1, 1] = 1;
                transitions[1, 2] = 2;
                transitions[1, 3] = -1;

                transitions[2, 0] = -1;
                transitions[2, 1] = 3;
                transitions[2, 2] = -1;
                transitions[2, 3] = -1;

                transitions[3, 0] = -1;
                transitions[3, 1] = 3;
                transitions[3, 2] = -1;
                transitions[3, 3] = -1;

                transitions[4, 0] = -1;
                transitions[4, 1] = 4;
                transitions[4, 2] = -1;
                transitions[4, 3] = 4;

                finalStateTokenIds = new int[] { -1, Integer, -1, Double, Id };
            }
        }
    }
}
