using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace TokenizerBuilder
{
    static class CodeGeneratorHelper
    {
        private static Regex charSetPattern = new Regex(@"^((?<a>\\.|[^\\])\-(?<b>\\.|[^\\])|(?<c>\\.|[^\\]))+$");

        private static char Escape(string text)
        {
            switch (text)
            {
                case @"\n":
                    return '\n';
                case @"\r":
                    return '\r';
                case @"\t":
                    return '\t';
                case @"\\":
                    return '\\';
                case @"\^":
                    return '^';
                case @"\.":
                    return '.';
            }
            if (text.Length != 1)
            {
                throw new ArgumentException("Char cannot be \"" + text + "\".");
            }
            else
            {
                return text[0];
            }
        }

        public static bool[] GetCharsetFromArrow(string arrowName)
        {
            bool revert = arrowName.StartsWith("^");
            if (revert)
            {
                arrowName = arrowName.Substring(1);
            }
            Match match = charSetPattern.Match(arrowName);
            if (match.Success)
            {
                bool[] result = new bool[char.MaxValue + 1];
                CaptureCollection acc = match.Groups["a"].Captures;
                CaptureCollection bcc = match.Groups["b"].Captures;
                CaptureCollection ccc = match.Groups["c"].Captures;
                foreach (Capture c in ccc)
                {
                    switch (c.Value)
                    {
                        case @"\.":
                            for (int i = 0; i < result.Length; i++)
                            {
                                result[i] = true;
                            }
                            break;
                        default:
                            result[Escape(c.Value)] = true;
                            break;
                    }
                }
                for (int i = 0; i < acc.Count; i++)
                {
                    char a = Escape(acc[i].Value);
                    char b = Escape(bcc[i].Value);
                    for (char x = a; x <= b; x++)
                    {
                        result[x] = true;
                    }
                }
                if (revert)
                {
                    for (int i = 0; i <= char.MaxValue; i++)
                    {
                        result[i] = !result[i];
                    }
                }
                return result;
            }
            else
            {
                throw new ArgumentException("Transition \"" + arrowName + "\" is not valid.");
            }
        }

        public static int[] MergeCharset(bool[][] arrows)
        {
            List<string> charsetName = new List<string>();
            int[] result = new int[char.MaxValue + 1];
            for (int x = char.MinValue; x <= char.MaxValue; x++)
            {
                if (arrows.All(a => !a[x]))
                {
                    result[x] = -1;
                }
                else
                {
                    string name = arrows.Select(a => a[x].ToString()).Aggregate("", (a, b) => a + "," + b);
                    int index = charsetName.IndexOf(name);
                    if (index == -1)
                    {
                        index = charsetName.Count;
                        charsetName.Add(name);
                    }
                    result[x] = index;
                }
            }
            return result;
        }
    }
}
