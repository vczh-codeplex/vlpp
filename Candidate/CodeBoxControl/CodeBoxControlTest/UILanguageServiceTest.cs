using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using CodeBoxControl.Core;

namespace CodeBoxControlTest
{
    [TestClass]
    public class UILanguageServiceTest
    {
        private CodeFormWindow window;

        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
            window = new CodeFormWindow();
        }

        [TestCleanup]
        public void Cleanup()
        {
            window.Close();
            Playback.Cleanup();
        }

        private void AssertColors(string colors, int index)
        {
            Assert.AreEqual(colors, window.Service.GetColorsOfLine(index).Select(c => c.ToString()).Aggregate("", (a, b) => (a + b)));
        }

        private void AssertFullCommentLine(int index)
        {
            string colors = Enumerable.Range(0, window.Service.GetLineLength(index)).Select(c => "3").Aggregate("", (a, b) => (a + b));
            AssertColors(colors, index);
        }

        [TestMethod]
        public void CSharpColorizerSimpleTest()
        {
            string[] data ={
                "using System;",                                // 00
                "3333322222222",
                "",                                             // 01
                "",
                "namespace TestProgram",                        // 02
                "333333333222222222222",
                "{",                                            // 03
                "2",
                "    // single line comment",                   // 04
                "22225555555555555555555555",
                "    static void Main(string[] args)",          // 05
                "22223333332333322222233333322222222",
                "    {",                                        // 06
                "22222",
                "        /* multiple line",                     // 07
                "222222225555555555555555",
                "        comment */",                           // 08
                "555555555555555555",
                "        Console.WriteLine(\"Hello, world\");", // 09
                "222222222222222222222222224444444444444422",
                "    }",                                        // 10
                "22222",
                "}",                                            // 11
                "2",
            };

            for (int i = 0; i < data.Length / 2; i++)
            {
                window.TypeAndEnter(data[i * 2]);
            }
            for (int i = 0; i < data.Length / 2; i++)
            {
                AssertColors(data[i * 2 + 1], i);
            }

            window.Move(new TextPosition(8, 18), false, false);
            window.PressBackspace(false, false);
            Assert.AreEqual("        comment *", window.GetString(new TextPosition(8, 0), new TextPosition(8, 17)));
            AssertFullCommentLine(8);
            AssertFullCommentLine(9);
            AssertFullCommentLine(10);
            AssertFullCommentLine(11);

            window.Type("/");
            Assert.AreEqual("        comment */", window.GetString(new TextPosition(8, 0), new TextPosition(8, 18)));
            AssertColors(data[8 * 2 + 1], 8);
            AssertColors(data[9 * 2 + 1], 9);
            AssertColors(data[10 * 2 + 1], 10);
            AssertColors(data[11 * 2 + 1], 11);

            window.Move(new TextPosition(9, 40), false, false);
            window.PressBackspace(false, false);
            AssertColors("00000000000000000000000000222222222222222", 9);
            AssertColors(data[10 * 2 + 1], 10);
            AssertColors(data[11 * 2 + 1], 11);

            window.Type("\"");
            AssertColors(data[9 * 2 + 1], 9);
            AssertColors(data[10 * 2 + 1], 10);
            AssertColors(data[11 * 2 + 1], 11);
        }
    }
}
