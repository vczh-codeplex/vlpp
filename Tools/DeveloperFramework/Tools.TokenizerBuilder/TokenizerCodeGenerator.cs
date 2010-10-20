using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Tools.TokenizerBuilder.Shape;

namespace Tools.TokenizerBuilder
{
    class TokenizerCodeGenerator
    {
        public static string GenerateCSharpCode(ShapeManager manager, string namespaceName, string className)
        {
            string[] colorIds = null;
            string[] stateIds = null;
            int[] charset = null;
            int[,] transitions = null;
            int[] finalStateTokenIds = null;

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

                charset = CodeGeneratorHelper.MergeCharset(arrows.Select(a => CodeGeneratorHelper.GetCharsetFromArrow(a.Name)).ToArray());

                finalStateTokenIds =
                    states
                    .Select(s => Array.FindIndex(colorIds, x => s.Type == StateType.Finish && s.Color == x))
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
                        transitions[i, j] = -1;
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
                            transitions[i, k] = end;
                        }
                    }
                }
            }
            return GenerateCSharpCodeInternal(namespaceName, className, colorIds, stateIds, charset, transitions, finalStateTokenIds);
        }

        private static string GenerateCSharpCodeInternal(string namespaceName, string className, string[] colorIds, string[] stateIds, int[] charset, int[,] transitions, int[] finalStateTokenIds)
        {
            StringBuilder builder = new StringBuilder();
            // header
            builder.AppendLine("using System;");
            builder.AppendLine("using System.Collections.Generic;");
            builder.AppendLine("using System.Linq;");
            builder.AppendLine("using System.Text;");
            builder.AppendLine("using Developer.LanguageProvider;");
            builder.AppendLine();
            builder.AppendLine("namespace " + namespaceName);
            builder.AppendLine("{");
            builder.AppendLine("    public partial class " + className + " : CodeTokenizer");
            builder.AppendLine("    {");

            // color ids
            for (int i = 0; i < colorIds.Length; i++)
            {
                builder.AppendLine("        public const int " + colorIds[i] + "Token = " + i.ToString() + ";");
            }
            builder.AppendLine();

            // state ids
            for (int i = 0; i < stateIds.Length; i++)
            {
                builder.AppendLine("        private const int " + stateIds[i] + "StateId = " + i.ToString() + ";");
            }
            builder.AppendLine();

            // state machine
            builder.AppendLine("        public " + className + "()");
            builder.AppendLine("        {");
            builder.AppendLine("            charset = new int[" + charset.Length.ToString() + "];");
            builder.AppendLine("            transitions = new int[" + transitions.GetLength(0).ToString() + "," + transitions.GetLength(1).ToString() + "];");
            builder.AppendLine("            finalStateTokenIds = new int[" + finalStateTokenIds.Length.ToString() + "];");
            builder.AppendLine();

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

            for (int i = 0; i < finalStateTokenIds.Length; i++)
            {
                builder.AppendLine("            finalStateTokenIds[" + i.ToString() + "] = " + finalStateTokenIds[i].ToString() + ";");
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
