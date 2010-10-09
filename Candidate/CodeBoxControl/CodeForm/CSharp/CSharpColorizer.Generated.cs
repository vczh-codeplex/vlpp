using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Drawing;

namespace CodeForm
{
    partial class CSharpColorizer : ITextEditorColorizer
    {
        //public const int NormalColorId = 0;
        public const int IdColorId = NormalColorId + 1;
        public const int StringColorId = NormalColorId + 2;
        public const int CommentColorId = NormalColorId + 3;

        //private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        //private readonly Color NormalColor = Color.FromArgb(0, 0, 0);
        //private readonly Color IdColor = Color.FromArgb(0, 0, 0);
        //private readonly Color StringColor = Color.FromArgb(0, 0, 0);
        //private readonly Color CommentColor = Color.FromArgb(0, 0, 0);

        private const int StartState = 0;
        private const int NormalState = 1;
        private const int IdStateId = 2;
        private const int InStringStateId = 3;
        private const int InStringEscapingStateId = 4;
        private const int InCharStateId = 5;
        private const int InCharEscapingStateId = 6;
        private const int StringStateId = 7;
        private const int CommentStartStateId = 8;
        private const int SingleLineCommentStateId = 9;
        private const int InMultipleLineCommentStateId = 10;
        private const int InMultipleLineCommentWaitingToFinishStateId = 11;
        private const int MultipleLineCommentStateId = 12;

        private TextEditorColorItem[] colorItems = new TextEditorColorItem[NormalColorId + 4];
        private int[] charset = new int[65536];
        private int[,] transitions = new int[13, 11];
        private bool[] finalStates = new bool[13];
        private int[] stateColors = new int[13];

        public TextEditorColorItem[] ColorItems
        {
            get
            {
                return this.colorItems;
            }
        }

        public CSharpColorizer()
        {
            this.colorItems[NormalColorId] = new TextEditorColorItem()
            {
                Text = NormalColor,
                HighlightText = NormalColor,
                Highlight = HighlightColor
            };
            this.colorItems[IdColorId] = new TextEditorColorItem()
            {
                Text = IdColor,
                HighlightText = IdColor,
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
            // You should write your own CreateAdditionalColors() implementation to add additional colors.
            // You can modify the NormalColorId and put all additional colors ids before NormalColorId.
            // It is recommended to use another partial class to store all customized code.
            CreateAdditionalColors();
            CreateStateMachine();
        }
        public int ColorizeLine(char[] items, int length, int initialState, int[] colors)
        {
            int state = initialState;
            int itemStart = 0;
            int lastFinalState = StartState;

            for (int i = 0; i <= length; i++)
            {
                if (i != length)
                {
                    state = transitions[state, charset[items[i]]];
                    if (state == StartState)
                    {
                        state = transitions[state, charset[items[i]]];
                    }
                }
                else
                {
                    lastFinalState = state;
                }

                if (i == length || lastFinalState != state && lastFinalState != StartState)
                {
                    int color = stateColors[lastFinalState];
                    switch (lastFinalState)
                    {
                        case IdStateId:
                            // You should write your own IsValidId implementation.
                            color = IsValidId(new string(items, itemStart, Math.Min(i, length) - itemStart)) ? stateColors[lastFinalState] : NormalColorId;
                            break;
                    }
                    for (int j = itemStart; j < i; j++)
                    {
                        colors[j] = color;
                    }
                    itemStart = i;
                }
                lastFinalState = finalStates[state] ? state : StartState;
            }

            return transitions[state, charset['\n']];
        }

        private void CreateStateMachine()
        {
            for (int i = 0; i < 10; i++)
                charset[i] = 0;
            for (int i = 10; i < 11; i++)
                charset[i] = 1;
            for (int i = 11; i < 34; i++)
                charset[i] = 0;
            for (int i = 34; i < 35; i++)
                charset[i] = 2;
            for (int i = 35; i < 36; i++)
                charset[i] = 3;
            for (int i = 36; i < 39; i++)
                charset[i] = 0;
            for (int i = 39; i < 40; i++)
                charset[i] = 4;
            for (int i = 40; i < 42; i++)
                charset[i] = 0;
            for (int i = 42; i < 43; i++)
                charset[i] = 5;
            for (int i = 43; i < 47; i++)
                charset[i] = 0;
            for (int i = 47; i < 48; i++)
                charset[i] = 6;
            for (int i = 48; i < 58; i++)
                charset[i] = 7;
            for (int i = 58; i < 65; i++)
                charset[i] = 0;
            for (int i = 65; i < 91; i++)
                charset[i] = 8;
            for (int i = 91; i < 92; i++)
                charset[i] = 0;
            for (int i = 92; i < 93; i++)
                charset[i] = 9;
            for (int i = 93; i < 95; i++)
                charset[i] = 0;
            for (int i = 95; i < 96; i++)
                charset[i] = 8;
            for (int i = 96; i < 97; i++)
                charset[i] = 0;
            for (int i = 97; i < 123; i++)
                charset[i] = 8;
            for (int i = 123; i < 65536; i++)
                charset[i] = 0;

            finalStates[0] = false;
            finalStates[1] = true;
            finalStates[2] = true;
            finalStates[3] = false;
            finalStates[4] = false;
            finalStates[5] = false;
            finalStates[6] = false;
            finalStates[7] = true;
            finalStates[8] = false;
            finalStates[9] = true;
            finalStates[10] = false;
            finalStates[11] = false;
            finalStates[12] = true;

            stateColors[0] = NormalColorId + 0;
            stateColors[1] = NormalColorId + 0;
            stateColors[2] = NormalColorId + 1;
            stateColors[3] = NormalColorId + 2;
            stateColors[4] = NormalColorId + 2;
            stateColors[5] = NormalColorId + 2;
            stateColors[6] = NormalColorId + 2;
            stateColors[7] = NormalColorId + 2;
            stateColors[8] = NormalColorId + 0;
            stateColors[9] = NormalColorId + 3;
            stateColors[10] = NormalColorId + 3;
            stateColors[11] = NormalColorId + 3;
            stateColors[12] = NormalColorId + 3;

            transitions[0, 0] = 1;
            transitions[0, 1] = 1;
            transitions[0, 2] = 3;
            transitions[0, 3] = 2;
            transitions[0, 4] = 5;
            transitions[0, 5] = 1;
            transitions[0, 6] = 8;
            transitions[0, 7] = 1;
            transitions[0, 8] = 2;
            transitions[0, 9] = 1;
            transitions[0, 10] = 1;
            transitions[1, 0] = 0;
            transitions[1, 1] = 0;
            transitions[1, 2] = 0;
            transitions[1, 3] = 0;
            transitions[1, 4] = 0;
            transitions[1, 5] = 0;
            transitions[1, 6] = 0;
            transitions[1, 7] = 0;
            transitions[1, 8] = 0;
            transitions[1, 9] = 0;
            transitions[1, 10] = 0;
            transitions[2, 0] = 0;
            transitions[2, 1] = 0;
            transitions[2, 2] = 0;
            transitions[2, 3] = 0;
            transitions[2, 4] = 0;
            transitions[2, 5] = 0;
            transitions[2, 6] = 0;
            transitions[2, 7] = 2;
            transitions[2, 8] = 2;
            transitions[2, 9] = 0;
            transitions[2, 10] = 0;
            transitions[3, 0] = 3;
            transitions[3, 1] = 0;
            transitions[3, 2] = 7;
            transitions[3, 3] = 3;
            transitions[3, 4] = 3;
            transitions[3, 5] = 3;
            transitions[3, 6] = 3;
            transitions[3, 7] = 3;
            transitions[3, 8] = 3;
            transitions[3, 9] = 4;
            transitions[3, 10] = 0;
            transitions[4, 0] = 3;
            transitions[4, 1] = 0;
            transitions[4, 2] = 3;
            transitions[4, 3] = 3;
            transitions[4, 4] = 3;
            transitions[4, 5] = 3;
            transitions[4, 6] = 3;
            transitions[4, 7] = 3;
            transitions[4, 8] = 3;
            transitions[4, 9] = 3;
            transitions[4, 10] = 0;
            transitions[5, 0] = 5;
            transitions[5, 1] = 0;
            transitions[5, 2] = 5;
            transitions[5, 3] = 5;
            transitions[5, 4] = 7;
            transitions[5, 5] = 5;
            transitions[5, 6] = 5;
            transitions[5, 7] = 5;
            transitions[5, 8] = 5;
            transitions[5, 9] = 6;
            transitions[5, 10] = 0;
            transitions[6, 0] = 5;
            transitions[6, 1] = 0;
            transitions[6, 2] = 5;
            transitions[6, 3] = 5;
            transitions[6, 4] = 5;
            transitions[6, 5] = 5;
            transitions[6, 6] = 5;
            transitions[6, 7] = 5;
            transitions[6, 8] = 5;
            transitions[6, 9] = 5;
            transitions[6, 10] = 0;
            transitions[7, 0] = 0;
            transitions[7, 1] = 0;
            transitions[7, 2] = 0;
            transitions[7, 3] = 0;
            transitions[7, 4] = 0;
            transitions[7, 5] = 0;
            transitions[7, 6] = 0;
            transitions[7, 7] = 0;
            transitions[7, 8] = 0;
            transitions[7, 9] = 0;
            transitions[7, 10] = 0;
            transitions[8, 0] = 1;
            transitions[8, 1] = 1;
            transitions[8, 2] = 1;
            transitions[8, 3] = 1;
            transitions[8, 4] = 1;
            transitions[8, 5] = 10;
            transitions[8, 6] = 9;
            transitions[8, 7] = 1;
            transitions[8, 8] = 1;
            transitions[8, 9] = 1;
            transitions[8, 10] = 1;
            transitions[9, 0] = 9;
            transitions[9, 1] = 0;
            transitions[9, 2] = 9;
            transitions[9, 3] = 9;
            transitions[9, 4] = 9;
            transitions[9, 5] = 9;
            transitions[9, 6] = 9;
            transitions[9, 7] = 9;
            transitions[9, 8] = 9;
            transitions[9, 9] = 9;
            transitions[9, 10] = 0;
            transitions[10, 0] = 10;
            transitions[10, 1] = 10;
            transitions[10, 2] = 10;
            transitions[10, 3] = 10;
            transitions[10, 4] = 10;
            transitions[10, 5] = 11;
            transitions[10, 6] = 0;
            transitions[10, 7] = 10;
            transitions[10, 8] = 10;
            transitions[10, 9] = 10;
            transitions[10, 10] = 0;
            transitions[11, 0] = 10;
            transitions[11, 1] = 10;
            transitions[11, 2] = 10;
            transitions[11, 3] = 10;
            transitions[11, 4] = 10;
            transitions[11, 5] = 10;
            transitions[11, 6] = 12;
            transitions[11, 7] = 10;
            transitions[11, 8] = 10;
            transitions[11, 9] = 10;
            transitions[11, 10] = 0;
            transitions[12, 0] = 0;
            transitions[12, 1] = 0;
            transitions[12, 2] = 0;
            transitions[12, 3] = 0;
            transitions[12, 4] = 0;
            transitions[12, 5] = 0;
            transitions[12, 6] = 0;
            transitions[12, 7] = 0;
            transitions[12, 8] = 0;
            transitions[12, 9] = 0;
            transitions[12, 10] = 0;
        }
    }
}
