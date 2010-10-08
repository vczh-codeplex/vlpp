using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TokenizerBuilder.Shape;
using System.Text.RegularExpressions;

namespace TokenizerBuilder
{
    class ColorizerCodeGenerator
    {
        private static Regex charSetPattern = new Regex(@"^((?<a>\\.|[^\\])\-(?<b>\\.|[^\\])|(?<c>\\.|[^\\]))+$");

        public static string GenerateCSharpCode(ShapeManager manager, string className)
        {
            string[] colorIds = null;
            string[] stateIds = null;
            int[] charset = null;
            int[,] transitions = null;
            bool[] finalStates = null;
            int[] stateColors = null;

            {
                List<StateShape> states = manager.Shapes[StateShape.StateShapeProprity]
                    .Cast<StateShape>()
                    .Where(s => s.Type == StateType.Start)
                    .Concat(
                        manager.Shapes[StateShape.StateShapeProprity]
                        .Cast<StateShape>()
                        .Where(s => s.Type != StateType.Start)
                    )
                    .ToList();

                List<ArrowShape> arrows = manager.Shapes[ArrowShape.ArrowShapeProprity]
                    .Cast<ArrowShape>()
                    .ToList();

                colorIds = manager.Shapes[StateShape.StateShapeProprity]
                    .Cast<StateShape>()
                    .Where(s => s.Type == StateType.Finish && !string.IsNullOrEmpty(s.Color))
                    .Select(s => s.Color)
                    .Distinct()
                    .ToArray();

                stateIds = states
                    .Select(s => s.Name)
                    .ToArray();

                charset = MergeCharset(arrows.Select(a => GetCharsetFromArrow(a.Name)).ToArray());

                finalStates = states
                    .Select(s => s.Type == StateType.Finish)
                    .ToArray();

                stateColors = states
                    .Select(s => Array.FindIndex(colorIds, x => x == s.Color) + 1)
                    .ToArray();

                int fakeCharset = charset.Max() + 1;
                for (int i = 0; i < charset.Length; i++)
                {
                    if (charset[i] == -1)
                    {
                        charset[i] = fakeCharset;
                    }
                }
                transitions = new int[states.Count, fakeCharset + 1];
                for (int i = 0; i < states.Count; i++)
                {
                    StateShape state = states[i];
                    for (int j = 0; j <= fakeCharset; j++)
                    {
                        transitions[i, j] = 0;
                    }
                    for (int j = 0; j < state.OutArrows.Count; j++)
                    {
                        ArrowShape arrow = state.OutArrows[j];
                        int[] charsets = GetCharsetFromArrow(arrow.Name)
                            .Select((b, c) => Tuple.Create(b, c))
                            .Where(t => t.Item1)
                            .Select(t => charset[t.Item2])
                            .Distinct()
                            .ToArray();
                        foreach (int k in charsets)
                        {
                            transitions[i, k] = states.IndexOf(arrow.End);
                        }
                    }
                }
            }
            return GenerateCSharpCodeInternal(className, colorIds, stateIds, charset, transitions, finalStates, stateColors);
        }

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

        private static bool[] GetCharsetFromArrow(string arrowName)
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
                    result[Escape(c.Value)] = true;
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

        private static int[] MergeCharset(bool[][] arrows)
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

        private static string GenerateCSharpCodeInternal(string className, string[] colorIds, string[] stateIds, int[] charset, int[,] transitions, bool[] finalStates, int[] stateColors)
        {
            StringBuilder builder = new StringBuilder();
            // header
            builder.AppendLine("using System;");
            builder.AppendLine("using System.Collections.Generic;");
            builder.AppendLine("using System.Linq;");
            builder.AppendLine("using System.Text;");
            builder.AppendLine("using CodeBoxControl;");
            builder.AppendLine("using System.Drawing;");
            builder.AppendLine();
            builder.AppendLine("namespace " + className + "Namespace");
            builder.AppendLine("{");
            builder.AppendLine("    partial class " + className + " : ITextEditorColorizer");
            builder.AppendLine("    {");

            // color ids
            builder.AppendLine("        public const int NormalColorId = 0;");
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("        public const int " + colorIds[i] + "ColorId = NormalColorId + " + (i + 1).ToString() + ";");
            }
            builder.AppendLine();

            // colors
            builder.AppendLine("        private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);");
            builder.AppendLine("        private readonly Color NormalColor = Color.FromArgb(0, 0, 0);");
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("        private readonly Color " + colorIds[i] + "Color = Color.FromArgb(0, 0, 0);");
            }
            builder.AppendLine();

            // state ids
            builder.AppendLine("        private const int StartState = 0;");
            for (int i = 1; i < stateIds.Length; i++)
            {
                builder.AppendLine("        private const int " + stateIds[i] + "StateId = " + i.ToString() + ";");
            }
            builder.AppendLine();

            // color items
            builder.AppendLine("        private TextEditorColorItem[] colorItems = new TextEditorColorItem[NormalColorId + " + (colorIds.Length + 1).ToString() + "];");
            builder.AppendLine("        private int[] charset = new int[" + charset.Length.ToString() + "];");
            builder.AppendLine("        private int[,] transitions = new int[" + transitions.GetLength(0).ToString() + "," + transitions.GetLength(1).ToString() + "];");
            builder.AppendLine("        private bool[] finalStates = new bool[" + finalStates.Length.ToString() + "];");
            builder.AppendLine("        private int[] stateColors = new int[" + stateColors.Length.ToString() + "];");
            builder.AppendLine();
            builder.AppendLine("        public TextEditorColorItem[] ColorItems");
            builder.AppendLine("        {");
            builder.AppendLine("            get");
            builder.AppendLine("            {");
            builder.AppendLine("                return this.colorItems;");
            builder.AppendLine("            }");
            builder.AppendLine("        }");
            builder.AppendLine();
            builder.AppendLine("        public " + className + "()");
            builder.AppendLine("        {");
            builder.AppendLine("            this.colorItems[NormalColorId] = new TextEditorColorItem()");
            builder.AppendLine("            {");
            builder.AppendLine("                Text = NormalColor,");
            builder.AppendLine("                HighlightText = NormalColor,");
            builder.AppendLine("                Highlight = HighlightColor");
            builder.AppendLine("            };");
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("            this.colorItems[" + colorIds[i] + "ColorId] = new TextEditorColorItem()");
                builder.AppendLine("            {");
                builder.AppendLine("                Text = " + colorIds[i] + "Color,");
                builder.AppendLine("                HighlightText = " + colorIds[i] + "Color,");
                builder.AppendLine("                Highlight = HighlightColor");
                builder.AppendLine("            };");
            }
            builder.AppendLine("            // You should write your own CreateAdditionalColors() implementation to add additional colors.");
            builder.AppendLine("            // You can modify the NormalColorId and put all additional colors ids before NormalColorId.");
            builder.AppendLine("            CreateAdditionalColors();");
            builder.AppendLine("            CreateStateMachine();");
            builder.AppendLine("        }");

            // transition
            builder.AppendLine("        public int ColorizeLine(char[] items, int length, int initialState, int[] colors)");
            builder.AppendLine("        {");
            builder.AppendLine("            int state = initialState;");
            builder.AppendLine("            int itemStart = 0;");
            builder.AppendLine("            int lastFinalState = StartState;");
            builder.AppendLine();
            builder.AppendLine("            for (int i = 0; i <= length; i++)");
            builder.AppendLine("            {");
            builder.AppendLine("                if (i != length)");
            builder.AppendLine("                {");
            builder.AppendLine("                    state = transitions[state, charset[items[i]]];");
            builder.AppendLine("                }");
            builder.AppendLine();
            builder.AppendLine("                if (i == length || lastFinalState != state && lastFinalState != StartState)");
            builder.AppendLine("                {");
            builder.AppendLine("                    int color = stateColors[lastFinalState];");
            builder.AppendLine("                    for (int j = itemStart; j < i; j++)");
            builder.AppendLine("                    {");
            builder.AppendLine("                        colors[j] = color;");
            builder.AppendLine("                    }");
            builder.AppendLine("                    itemStart = i;");
            builder.AppendLine("                }");
            builder.AppendLine("                lastFinalState = finalStates[state] ? state : StartState;");
            builder.AppendLine("            }");
            builder.AppendLine();
            builder.AppendLine("            return transitions[state, charset['\\n']];");
            builder.AppendLine("        }");
            builder.AppendLine();

            // state machine
            builder.AppendLine("        private void CreateStateMachine()");
            builder.AppendLine("        {");

            int oldCharsetStart = 0;
            int oldCharsetValue = charset[0];
            for (int i = 1; i <= charset.Length; i++)
            {
                if (i == charset.Length || charset[i] != oldCharsetValue)
                {
                    builder.AppendLine("            for (int i = " + oldCharsetStart.ToString() + "; i < " + i.ToString() + "; i++)");
                    builder.AppendLine("                charset[i] = " + oldCharsetValue.ToString() + ";");
                    if (i != charset.Length)
                    {
                        oldCharsetStart = i;
                        oldCharsetValue = charset[i];
                    }
                }
            }
            builder.AppendLine();

            for (int i = 0; i < finalStates.Length; i++)
            {
                builder.AppendLine("            finalStates[" + i.ToString() + "] = " + (finalStates[i] ? "true" : "false") + ";");
            }
            builder.AppendLine();

            for (int i = 0; i < stateColors.Length; i++)
            {
                builder.AppendLine("            stateColors[" + i.ToString() + "] = " + stateColors[i].ToString() + ";");
            }
            builder.AppendLine();

            for (int i = 0; i < transitions.GetLength(0); i++)
            {
                for (int j = 0; j < transitions.GetLength(1); j++)
                {
                    builder.AppendLine("            transitions[" + i.ToString() + ", " + j.ToString() + "] = " + transitions[i, j].ToString() + ";");
                }
            }
            builder.AppendLine("        }");

            // final
            builder.AppendLine("    }");
            builder.AppendLine("}");
            return builder.ToString();
        }
    }
}
