using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Drawing;

namespace CodeForm
{
    class CSharpColorizer : ITextEditorColorizer
    {
        private const int NormalColorId = 0;
        private const int KeywordColorId = 1;
        private const int StringColorId = 2;
        private const int CommentColorId = 3;

        private const int StartState = 0;
        private const int IdState = 1;
        private const int InStringState = 2;
        private const int InStringEscapingState = 3;
        private const int InCharState = 4;
        private const int InCharEscapingState = 5;
        private const int StringState = 6;
        private const int CommentStartState = 7;
        private const int SingleLineCommentState = 8;
        private const int InMultipleLineCommentState = 9;
        private const int InMultipleLineCommentWaitingToFinishState = 10;
        private const int MultipleLineCommentState = 11;
        private const int NormalState = 12;

        private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        private readonly Color NormalColor = Color.FromArgb(0, 0, 0);
        private readonly Color KeywordColor = Color.FromArgb(0, 0, 255);
        private readonly Color StringColor = Color.FromArgb(163, 21, 21);
        private readonly Color CommentColor = Color.FromArgb(0, 128, 0);

        private TextEditorColorItem[] colorItems = null;

        public TextEditorColorItem[] ColorItems
        {
            get
            {
                return this.colorItems;
            }
        }

        public CSharpColorizer()
        {
            this.colorItems = new TextEditorColorItem[4];
            this.colorItems[NormalColorId] = new TextEditorColorItem()
            {
                Text = NormalColor,
                HighlightText = NormalColor,
                Highlight = HighlightColor
            };
            this.colorItems[KeywordColorId] = new TextEditorColorItem()
            {
                Text = KeywordColor,
                HighlightText = KeywordColor,
                Highlight = HighlightColor
            };
            this.colorItems[StringColorId] = new TextEditorColorItem()
            {
                Text = StringColor,
                HighlightText = StringColor,
                Highlight = HighlightColor
            };
            this.colorItems[CommentColorId] = new TextEditorColorItem()
            {
                Text = CommentColor,
                HighlightText = CommentColor,
                Highlight = HighlightColor
            };
        }

        public int ColorizeLine(char[] items, int length, int initialState, int[] colors)
        {
            int state = initialState;
            int itemStart = 0;
            int lastFinalState = StartState;

            for (int i = 0; i <= length; i++)
            {
                if (i == length)
                {
                    lastFinalState = FinalColorState(state);
                }
                else
                {
                    state = NextState(state, items[i]);
                }

                if (i == length || lastFinalState != state && lastFinalState != StartState)
                {
                    if (lastFinalState == IdState)
                    {
                        char[] chars = new char[i - itemStart];
                        Array.Copy(items, itemStart, chars, 0, i - itemStart);
                        string id = new string(chars);
                        if (!IsKeyword(id))
                        {
                            lastFinalState = NormalState;
                        }
                    }

                    int color = FinalStateToColor(lastFinalState);
                    for (int j = itemStart; j < i; j++)
                    {
                        colors[j] = color;
                    }
                    itemStart = i;
                }
                lastFinalState = IsFinalState(state) ? state : StartState;
            }

            return FinalState(state);
        }

        private int NextState(int state, char input)
        {
            switch (state)
            {
                case IdState:
                    if (!('a' <= input && input <= 'z' || 'A' <= input && input <= 'Z' || '0' <= input && input <= '9' || input == '_'))
                    {
                        state = StartState;
                    }
                    break;
                case StringState:
                    state = StartState;
                    break;
                case MultipleLineCommentState:
                    state = StartState;
                    break;
                case NormalState:
                    state = StartState;
                    break;
            }
            switch (state)
            {
                case StartState:
                    if (input == '"')
                        return InStringState;
                    else if (input == '\'')
                        return InCharState;
                    else if ('a' <= input && input <= 'z' || 'A' <= input && input <= 'Z' || input == '_' || input == '#')
                        return IdState;
                    else if (input == '/')
                        return CommentStartState;
                    else
                        return NormalState;
                case IdState:
                    return IdState;
                case InStringState:
                    if (input == '\\')
                        return InStringEscapingState;
                    else if (input == '"')
                        return StringState;
                    else
                        return InStringState;
                case InStringEscapingState:
                    return InStringState;
                case InCharState:
                    if (input == '\\')
                        return InCharEscapingState;
                    else if (input == '\'')
                        return StringState;
                    else
                        return InCharState;
                case InCharEscapingState:
                    return InCharState;
                case CommentStartState:
                    if (input == '/')
                        return SingleLineCommentState;
                    else if (input == '*')
                        return InMultipleLineCommentState;
                    else
                        return NormalState;
                case SingleLineCommentState:
                    return SingleLineCommentState;
                case InMultipleLineCommentState:
                    if (input == '*')
                        return InMultipleLineCommentWaitingToFinishState;
                    else
                        return InMultipleLineCommentState;
                case InMultipleLineCommentWaitingToFinishState:
                    if (input == '/')
                        return MultipleLineCommentState;
                    else if (input == '*')
                        return InMultipleLineCommentWaitingToFinishState;
                    else
                        return InMultipleLineCommentState;
                default:
                    throw new InvalidOperationException("Wrong state and input.");
            }
        }

        private int FinalStateToColor(int state)
        {
            switch (state)
            {
                case StringState: return StringColorId;
                case IdState: return KeywordColorId;
                case SingleLineCommentState: return CommentColorId;
                case MultipleLineCommentState: return CommentColorId;
                default: return NormalColorId;
            }
        }

        private bool IsFinalState(int state)
        {
            switch (state)
            {
                case StringState: return true;
                case IdState: return true;
                case SingleLineCommentState: return true;
                case MultipleLineCommentState: return true;
                case NormalState: return true;
                default: return false;
            }
        }

        private int FinalColorState(int state)
        {
            switch (state)
            {
                case InStringState: return StringState;
                case InStringEscapingState: return StringState;
                case InCharState: return StringState;
                case InCharEscapingState: return StringState;
                case InMultipleLineCommentState: return MultipleLineCommentState;
                case InMultipleLineCommentWaitingToFinishState: return MultipleLineCommentState;

                case StringState: return state;
                case IdState: return state;
                case SingleLineCommentState: return state;
                case MultipleLineCommentState: return state;
                case NormalState: return state;

                default: return NormalState;
            }
        }

        private int FinalState(int state)
        {
            switch (state)
            {
                case InMultipleLineCommentState: return InMultipleLineCommentState;
                case InMultipleLineCommentWaitingToFinishState: return InMultipleLineCommentState;
                default: return StartState;
            }
        }

        private bool IsKeyword(string id)
        {
            return id[0] == '#' || Array.BinarySearch(keywords, id) >= 0;
        }

        private static string[] keywords ={
            "abstract",
            "as",
            "base",
            "bool",
            "break",
            "byte",
            "case",
            "catch",
            "char",
            "checked",
            "class",
            "const",
            "continue",
            "decimal",
            "default",
            "delegate",
            "do",
            "double",
            "else",
            "enum",
            "event",
            "explicit",
            "extern",
            "false",
            "finally",
            "fixed",
            "float",
            "for",
            "foreach",
            "goto",
            "if",
            "implicit",
            "in",
            "int",
            "interface",
            "internal",
            "is",
            "lock",
            "long",
            "namespace",
            "new",
            "null",
            "object",
            "operator",
            "out",
            "override",
            "params",
            "private",
            "protected",
            "public",
            "readonly",
            "ref",
            "return",
            "sbyte",
            "sealed",
            "short",
            "sizeof",
            "stackalloc",
            "static",
            "string",
            "struct",
            "switch",
            "this",
            "throw",
            "true",
            "try",
            "typeof",
            "unit",
            "ulong",
            "unchecked",
            "unsafe",
            "ushort",
            "using",
            "virtual",
            "void",
            "volatile",
            "while"
        };
    }
}
