using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;
using Developer.WinFormControls.Core;
using System.Windows.Forms;
using Developer.LanguageProvider;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;

namespace Test.DeveloperComponent
{
    [TestClass]
    public class NativeXIntellisenseTest
    {
        private CodeFormWindow window;

        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
            window = new CodeFormWindow("NativeX", "NativeX Form");
        }

        [TestCleanup]
        public void Cleanup()
        {
            window.Close();
            Playback.Cleanup();
        }

        [TestMethod]
        public void InputCode()
        {
            string code = File.ReadAllText("NativeXIntellisenseInputCode.txt");
            Regex regex = new Regex(@"(\$|\#|\&)\[.*?\]");
            var matches = regex.Matches(code);
            int start = 0;

            foreach (Match match in matches)
            {
                string text = code.Substring(start, match.Index - start);
                if (text != "")
                {
                    window.Type(text);
                }
                string intellisense = match.Value.Substring(2, match.Value.Length - 3);
                if (match.Value[0] == '&')
                {
                    Keyboard.SendKeys("{" + intellisense + "}");
                    start = match.Index + match.Length;
                    continue;
                }
                if (match.Value[0] == '$')
                {
                    window.Type(intellisense.Substring(0, 1));
                }
                Thread.Sleep(100);

                List<string> items = window.Service.GetPopupedItems().ToList();
                Assert.IsTrue(items.Contains(intellisense));

                string hitPrefix = Enumerable.Range(1, intellisense.Length)
                    .Select(i => intellisense.Substring(0, i))
                    .Where(s => items.Where(item => item.StartsWith(s)).Count() == 1)
                    .Min();
                if (hitPrefix == null)
                {
                    hitPrefix = intellisense;
                }
                if (match.Value[0] == '$')
                {
                    hitPrefix = hitPrefix.Substring(1);
                }
                if (hitPrefix != "")
                {
                    window.Type(hitPrefix);
                }

                start = match.Index + match.Length;
                char nextChar = start == code.Length ? '\0' : code[start];
                if (nextChar == '\0' || nextChar == '\r' || nextChar == '\n')
                {
                    window.PressEnter(false, false);
                }
                else
                {
                    if (nextChar != '$')
                    {
                        window.Type(nextChar.ToString());
                    }
                    start++;
                }
            }

            if (start != code.Length)
            {
                string text = code.Substring(start, code.Length - start);
                window.Type(text);
            }
        }
    }
}
