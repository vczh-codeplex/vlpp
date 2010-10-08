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
        private const int IdStateId = 1;
        private const int InStringStateId = 2;
        private const int InStringEscapingStateId = 3;
        private const int InCharStateId = 4;
        private const int InCharEscapingStateId = 5;
        private const int StringStateId = 6;
        private const int CommentStartStateId = 7;
        private const int SingleLineCommentStateId = 8;
        private const int InMultipleLineCommentStateId = 9;
        private const int InMultipleLineCommentWaitingToFinishStateId = 10;
        private const int MultipleLineCommentStateId = 11;

        private TextEditorColorItem[] colorItems = new TextEditorColorItem[NormalColorId + 4];
        private int[] charset = new int[65536];
        private int[,] transitions = new int[12, 10];
        private bool[] finalStates = new bool[12];
        private int[] stateColors = new int[12];

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
                }

                if (i == length || lastFinalState != state && lastFinalState != StartState)
                {
                    int color = stateColors[lastFinalState];
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
            for (int i = 35; i < 39; i++)
                charset[i] = 0;
            for (int i = 39; i < 40; i++)
                charset[i] = 3;
            for (int i = 40; i < 42; i++)
                charset[i] = 0;
            for (int i = 42; i < 43; i++)
                charset[i] = 4;
            for (int i = 43; i < 47; i++)
                charset[i] = 0;
            for (int i = 47; i < 48; i++)
                charset[i] = 5;
            for (int i = 48; i < 58; i++)
                charset[i] = 6;
            for (int i = 58; i < 65; i++)
                charset[i] = 0;
            for (int i = 65; i < 91; i++)
                charset[i] = 7;
            for (int i = 91; i < 92; i++)
                charset[i] = 0;
            for (int i = 92; i < 93; i++)
                charset[i] = 8;
            for (int i = 93; i < 95; i++)
                charset[i] = 0;
            for (int i = 95; i < 96; i++)
                charset[i] = 7;
            for (int i = 96; i < 97; i++)
                charset[i] = 0;
            for (int i = 97; i < 123; i++)
                charset[i] = 7;
            for (int i = 123; i < 65536; i++)
                charset[i] = 0;

            finalStates[0] = false;
            finalStates[1] = true;
            finalStates[2] = false;
            finalStates[3] = false;
            finalStates[4] = false;
            finalStates[5] = false;
            finalStates[6] = true;
            finalStates[7] = false;
            finalStates[8] = true;
            finalStates[9] = false;
            finalStates[10] = false;
            finalStates[11] = true;

            stateColors[0] = 0;
            stateColors[1] = 1;
            stateColors[2] = 0;
            stateColors[3] = 0;
            stateColors[4] = 0;
            stateColors[5] = 0;
            stateColors[6] = 2;
            stateColors[7] = 0;
            stateColors[8] = 3;
            stateColors[9] = 0;
            stateColors[10] = 0;
            stateColors[11] = 3;

            transitions[0, 0] = 0;
            transitions[0, 1] = 0;
            transitions[0, 2] = 2;
            transitions[0, 3] = 4;
            transitions[0, 4] = 0;
            transitions[0, 5] = 7;
            transitions[0, 6] = 0;
            transitions[0, 7] = 1;
            transitions[0, 8] = 0;
            transitions[0, 9] = 0;
            transitions[1, 0] = 0;
            transitions[1, 1] = 0;
            transitions[1, 2] = 0;
            transitions[1, 3] = 0;
            transitions[1, 4] = 0;
            transitions[1, 5] = 0;
            transitions[1, 6] = 1;
            transitions[1, 7] = 1;
            transitions[1, 8] = 0;
            transitions[1, 9] = 0;
            transitions[2, 0] = 2;
            transitions[2, 1] = 0;
            transitions[2, 2] = 2;
            transitions[2, 3] = 2;
            transitions[2, 4] = 2;
            transitions[2, 5] = 2;
            transitions[2, 6] = 2;
            transitions[2, 7] = 2;
            transitions[2, 8] = 3;
            transitions[2, 9] = 0;
            transitions[3, 0] = 2;
            transitions[3, 1] = 0;
            transitions[3, 2] = 2;
            transitions[3, 3] = 2;
            transitions[3, 4] = 2;
            transitions[3, 5] = 2;
            transitions[3, 6] = 2;
            transitions[3, 7] = 2;
            transitions[3, 8] = 2;
            transitions[3, 9] = 0;
            transitions[4, 0] = 4;
            transitions[4, 1] = 0;
            transitions[4, 2] = 4;
            transitions[4, 3] = 4;
            transitions[4, 4] = 4;
            transitions[4, 5] = 4;
            transitions[4, 6] = 4;
            transitions[4, 7] = 4;
            transitions[4, 8] = 5;
            transitions[4, 9] = 0;
            transitions[5, 0] = 4;
            transitions[5, 1] = 0;
            transitions[5, 2] = 4;
            transitions[5, 3] = 4;
            transitions[5, 4] = 4;
            transitions[5, 5] = 4;
            transitions[5, 6] = 4;
            transitions[5, 7] = 4;
            transitions[5, 8] = 4;
            transitions[5, 9] = 0;
            transitions[6, 0] = 0;
            transitions[6, 1] = 0;
            transitions[6, 2] = 0;
            transitions[6, 3] = 0;
            transitions[6, 4] = 0;
            transitions[6, 5] = 0;
            transitions[6, 6] = 0;
            transitions[6, 7] = 0;
            transitions[6, 8] = 0;
            transitions[6, 9] = 0;
            transitions[7, 0] = 0;
            transitions[7, 1] = 0;
            transitions[7, 2] = 0;
            transitions[7, 3] = 0;
            transitions[7, 4] = 9;
            transitions[7, 5] = 8;
            transitions[7, 6] = 0;
            transitions[7, 7] = 0;
            transitions[7, 8] = 0;
            transitions[7, 9] = 0;
            transitions[8, 0] = 8;
            transitions[8, 1] = 0;
            transitions[8, 2] = 8;
            transitions[8, 3] = 8;
            transitions[8, 4] = 8;
            transitions[8, 5] = 8;
            transitions[8, 6] = 8;
            transitions[8, 7] = 8;
            transitions[8, 8] = 8;
            transitions[8, 9] = 0;
            transitions[9, 0] = 9;
            transitions[9, 1] = 9;
            transitions[9, 2] = 9;
            transitions[9, 3] = 9;
            transitions[9, 4] = 10;
            transitions[9, 5] = 0;
            transitions[9, 6] = 9;
            transitions[9, 7] = 9;
            transitions[9, 8] = 9;
            transitions[9, 9] = 0;
            transitions[10, 0] = 9;
            transitions[10, 1] = 9;
            transitions[10, 2] = 9;
            transitions[10, 3] = 9;
            transitions[10, 4] = 9;
            transitions[10, 5] = 11;
            transitions[10, 6] = 9;
            transitions[10, 7] = 9;
            transitions[10, 8] = 9;
            transitions[10, 9] = 0;
            transitions[11, 0] = 0;
            transitions[11, 1] = 0;
            transitions[11, 2] = 0;
            transitions[11, 3] = 0;
            transitions[11, 4] = 0;
            transitions[11, 5] = 0;
            transitions[11, 6] = 0;
            transitions[11, 7] = 0;
            transitions[11, 8] = 0;
            transitions[11, 9] = 0;
        }
    }
}
