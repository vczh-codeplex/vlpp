using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Tools.TokenizerBuilder.Shape;

namespace Tools.TokenizerBuilder
{
    class ColorizerCodeGenerator
    {
        public static string GenerateCSharpCode(ShapeManager manager, string namespaceName, string className, bool commentOutColors)
        {
            string[] colorIds = null;
            bool[] partialStates = null;
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
                    .Skip(1)
                    .Select(s => s.Name)
                    .ToArray();

                partialStates = stateIds.Select(c => c.EndsWith("*")).ToArray();
                stateIds = stateIds.Select(s => s.EndsWith("*") ? s.Substring(0, s.Length - 1) : s).ToArray();

                charset = CodeGeneratorHelper.MergeCharset(arrows.Select(a => CodeGeneratorHelper.GetCharsetFromArrow(a.Name)).ToArray());

                finalStates = new bool[] { false, true }.Concat(
                        states
                        .Skip(1)
                        .Select(s => s.Type == StateType.Finish)
                    )
                    .ToArray();

                stateColors = new int[] { 0, 0 }.Concat(
                        states
                        .Skip(1)
                        .Select(s => Array.FindIndex(colorIds, x => x == FindStateColor(s)) + 1)
                    )
                    .ToArray();

                int fakeCharset = charset.Max() + 1;
                for (int i = 0; i < charset.Length; i++)
                {
                    if (charset[i] == -1)
                    {
                        charset[i] = fakeCharset;
                    }
                }
                transitions = new int[states.Count + 1, fakeCharset + 1];
                for (int i = 0; i <= states.Count; i++)
                {
                    if (i == 1)
                    {
                        for (int j = 0; j <= fakeCharset; j++)
                        {
                            transitions[i, j] = 0;
                        }
                    }
                    else
                    {
                        StateShape state = states[i == 0 ? 0 : i - 1];
                        if (stateColors[i] == 0)
                        {
                            for (int j = 0; j <= fakeCharset; j++)
                            {
                                transitions[i, j] = 1;
                            }
                        }
                        else
                        {
                            for (int j = 0; j <= fakeCharset; j++)
                            {
                                transitions[i, j] = 0;
                            }
                        }
                        for (int j = 0; j < state.OutArrows.Count; j++)
                        {
                            ArrowShape arrow = state.OutArrows[j];
                            int[] charsets = CodeGeneratorHelper.GetCharsetFromArrow(arrow.Name)
                                .Select((b, c) => Tuple.Create(b, c))
                                .Where(t => t.Item1)
                                .Select(t => charset[t.Item2])
                                .Distinct()
                                .ToArray();
                            foreach (int k in charsets)
                            {
                                int end = states.IndexOf(arrow.End);
                                transitions[i, k] = end == 0 ? 0 : end + 1;
                            }
                        }
                    }
                }
            }
            return GenerateCSharpCodeInternal(namespaceName, className, commentOutColors, colorIds, stateIds, partialStates, charset, transitions, finalStates, stateColors);
        }

        private static string FindStateColor(StateShape state)
        {
            List<StateShape> closure = new List<StateShape>();
            int current = 0;
            closure.Add(state);
            while (current < closure.Count)
            {
                StateShape currentState = closure[current];
                if (currentState.Type != StateType.Partial && currentState.Type != StateType.Start)
                {
                    foreach (StateShape newState in currentState.OutArrows.Select(a => a.End))
                    {
                        if (!closure.Contains(newState))
                        {
                            closure.Add(newState);
                        }
                    }
                }
                current++;
            }

            string[] colors = closure
                .Where(s => s.Type == StateType.Finish)
                .Select(s => s.Color)
                .Distinct()
                .ToArray();
            if (colors.Length == 0)
            {
                return "";
            }
            else if (colors.Length == 1)
            {
                return colors[0];
            }
            else
            {
                throw new ArgumentException("State \"" + state.Name + "\" transites to final states of different colors.");
            }
        }

        private static string GenerateCSharpCodeInternal(string namespaceName, string className, bool commentOutColors, string[] colorIds, string[] stateIds, bool[] partialStates, int[] charset, int[,] transitions, bool[] finalStates, int[] stateColors)
        {
            StringBuilder builder = new StringBuilder();
            // header
            builder.AppendLine("using System;");
            builder.AppendLine("using System.Collections.Generic;");
            builder.AppendLine("using System.Linq;");
            builder.AppendLine("using System.Text;");
            builder.AppendLine("using Developer.WinFormControls;");
            builder.AppendLine("using System.Drawing;");
            builder.AppendLine();
            builder.AppendLine("namespace " + namespaceName);
            builder.AppendLine("{");
            builder.AppendLine("    public partial class " + className + " : ITextEditorColorizer");
            builder.AppendLine("    {");

            // color ids
            builder.AppendLine("        " + (commentOutColors ? "//" : "") + "public const int NormalColorId = 0;");
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("        public const int " + colorIds[i] + "ColorId = NormalColorId + " + (i + 1).ToString() + ";");
            }
            builder.AppendLine();

            // colors
            builder.AppendLine("        " + (commentOutColors ? "//" : "") + "private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);");
            builder.AppendLine("        " + (commentOutColors ? "//" : "") + "private readonly Color NormalColor = Color.FromArgb(0, 0, 0);");
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("        " + (commentOutColors ? "//" : "") + "private readonly Color " + colorIds[i] + "Color = Color.FromArgb(0, 0, 0);");
            }
            builder.AppendLine();

            // state ids
            builder.AppendLine("        private const int StartState = 0;");
            builder.AppendLine("        private const int NormalState = 1;");
            for (int i = 0; i < stateIds.Length; i++)
            {
                builder.AppendLine("        private const int " + stateIds[i] + "StateId = " + (i + 2).ToString() + ";");
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
            builder.AppendLine("            // It is recommended to use another partial class to store all customized code.");
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
            builder.AppendLine("                    if(state == StartState)");
            builder.AppendLine("                    {");
            builder.AppendLine("                        state = transitions[state, charset[items[i]]];");
            builder.AppendLine("                    }");
            builder.AppendLine("                }");
            builder.AppendLine("                else");
            builder.AppendLine("                {");
            builder.AppendLine("                    lastFinalState = state;");
            builder.AppendLine("                }");
            builder.AppendLine();
            builder.AppendLine("                if (i == length || lastFinalState != state && lastFinalState != StartState)");
            builder.AppendLine("                {");
            builder.AppendLine("                    int color = stateColors[lastFinalState];");
            if (partialStates.Any(t => t))
            {
                builder.AppendLine("                    switch (lastFinalState)");
                builder.AppendLine("                    {");
                for (int i = 0; i < partialStates.Length; i++)
                {
                    if (partialStates[i])
                    {
                        builder.AppendLine("                        case " + stateIds[i] + "StateId:");
                        builder.AppendLine("                            // You should write your own IsValid" + stateIds[i] + " implementation.");
                        builder.AppendLine("                            color = IsValid" + stateIds[i] + "(new string(items, itemStart, Math.Min(i, length) - itemStart)) ? stateColors[lastFinalState] : NormalColorId;");
                        builder.AppendLine("                            break;");
                    }
                }
                builder.AppendLine("                    }");
            }
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
                builder.AppendLine("            stateColors[" + i.ToString() + "] = NormalColorId + " + stateColors[i].ToString() + ";");
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
