﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Drawing;

namespace CodeForm
{
    partial class NativeXColorizer : ITextEditorColorizer
    {
       // public const int NormalColorId = 0;
        public const int IdColorId = NormalColorId + 1;
        public const int StringColorId = NormalColorId + 2;
        public const int NumberColorId = NormalColorId + 3;
        public const int CommentColorId = NormalColorId + 4;

        //private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        //private readonly Color NormalColor = Color.FromArgb(0, 0, 0);
        //private readonly Color IdColor = Color.FromArgb(0, 0, 0);
        //private readonly Color StringColor = Color.FromArgb(0, 0, 0);
        //private readonly Color NumberColor = Color.FromArgb(0, 0, 0);
        //private readonly Color CommentColor = Color.FromArgb(0, 0, 0);

        private const int StartState = 0;
        private const int NormalState = 1;
        private const int IdStateId = 2;
        private const int NativeIdStateId = 3;
        private const int NativeIdStartStateId = 4;
        private const int NativeIdInStateId = 5;
        private const int NativeIdEscapingStateId = 6;
        private const int CharInStateId = 7;
        private const int CharEscapingStateId = 8;
        private const int StringStateId = 9;
        private const int StringInStateId = 10;
        private const int StringEscapingStateId = 11;
        private const int WStartStateId = 12;
        private const int IntegerStateId = 13;
        private const int FloatStateId = 14;
        private const int DoubleStateId = 15;
        private const int TypedIntegerStateId = 16;
        private const int SizedIntegerStateId = 17;
        private const int Sized16IntegerStateId = 18;
        private const int Sized32IntegerStateId = 19;
        private const int Sized64IntegerStateId = 20;
        private const int MultipleLineCommentInStateId = 21;
        private const int CommentStartStateId = 22;
        private const int SingleLineCommentStateId = 23;
        private const int MultipleLineCommentWaitingStateId = 24;
        private const int MultipleLineCommentStateId = 25;

        private TextEditorColorItem[] colorItems = new TextEditorColorItem[NormalColorId + 5];
        private int[] charset = new int[65536];
        private int[,] transitions = new int[26, 23];
        private bool[] finalStates = new bool[26];
        private int[] stateColors = new int[26];

        public TextEditorColorItem[] ColorItems
        {
            get
            {
                return this.colorItems;
            }
        }

        public NativeXColorizer()
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
            this.colorItems[NumberColorId] = new TextEditorColorItem()
            {
                Text = NumberColor,
                HighlightText = NumberColor,
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
                        case NativeIdStateId:
                            // You should write your own IsValidNativeId implementation.
                            color = IsValidNativeId(new string(items, itemStart, Math.Min(i, length) - itemStart)) ? stateColors[lastFinalState] : NormalColorId;
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
            for (int i = 35; i < 39; i++)
                charset[i] = 0;
            for (int i = 39; i < 40; i++)
                charset[i] = 3;
            for (int i = 40; i < 42; i++)
                charset[i] = 0;
            for (int i = 42; i < 43; i++)
                charset[i] = 4;
            for (int i = 43; i < 46; i++)
                charset[i] = 0;
            for (int i = 46; i < 47; i++)
                charset[i] = 5;
            for (int i = 47; i < 48; i++)
                charset[i] = 6;
            for (int i = 48; i < 49; i++)
                charset[i] = 7;
            for (int i = 49; i < 50; i++)
                charset[i] = 8;
            for (int i = 50; i < 51; i++)
                charset[i] = 9;
            for (int i = 51; i < 52; i++)
                charset[i] = 10;
            for (int i = 52; i < 53; i++)
                charset[i] = 11;
            for (int i = 53; i < 54; i++)
                charset[i] = 7;
            for (int i = 54; i < 55; i++)
                charset[i] = 12;
            for (int i = 55; i < 56; i++)
                charset[i] = 7;
            for (int i = 56; i < 57; i++)
                charset[i] = 13;
            for (int i = 57; i < 58; i++)
                charset[i] = 7;
            for (int i = 58; i < 64; i++)
                charset[i] = 0;
            for (int i = 64; i < 65; i++)
                charset[i] = 14;
            for (int i = 65; i < 70; i++)
                charset[i] = 15;
            for (int i = 70; i < 71; i++)
                charset[i] = 16;
            for (int i = 71; i < 76; i++)
                charset[i] = 15;
            for (int i = 76; i < 77; i++)
                charset[i] = 17;
            for (int i = 77; i < 91; i++)
                charset[i] = 15;
            for (int i = 91; i < 92; i++)
                charset[i] = 0;
            for (int i = 92; i < 93; i++)
                charset[i] = 18;
            for (int i = 93; i < 95; i++)
                charset[i] = 0;
            for (int i = 95; i < 96; i++)
                charset[i] = 15;
            for (int i = 96; i < 97; i++)
                charset[i] = 0;
            for (int i = 97; i < 102; i++)
                charset[i] = 15;
            for (int i = 102; i < 103; i++)
                charset[i] = 19;
            for (int i = 103; i < 115; i++)
                charset[i] = 15;
            for (int i = 115; i < 116; i++)
                charset[i] = 20;
            for (int i = 116; i < 117; i++)
                charset[i] = 15;
            for (int i = 117; i < 118; i++)
                charset[i] = 21;
            for (int i = 118; i < 123; i++)
                charset[i] = 15;
            for (int i = 123; i < 65536; i++)
                charset[i] = 0;

            finalStates[0] = false;
            finalStates[1] = true;
            finalStates[2] = true;
            finalStates[3] = true;
            finalStates[4] = false;
            finalStates[5] = false;
            finalStates[6] = false;
            finalStates[7] = false;
            finalStates[8] = false;
            finalStates[9] = true;
            finalStates[10] = false;
            finalStates[11] = false;
            finalStates[12] = false;
            finalStates[13] = true;
            finalStates[14] = true;
            finalStates[15] = true;
            finalStates[16] = true;
            finalStates[17] = true;
            finalStates[18] = false;
            finalStates[19] = false;
            finalStates[20] = false;
            finalStates[21] = false;
            finalStates[22] = false;
            finalStates[23] = true;
            finalStates[24] = false;
            finalStates[25] = true;

            stateColors[0] = NormalColorId + 0;
            stateColors[1] = NormalColorId + 0;
            stateColors[2] = NormalColorId + 1;
            stateColors[3] = NormalColorId + 1;
            stateColors[4] = NormalColorId + 1;
            stateColors[5] = NormalColorId + 1;
            stateColors[6] = NormalColorId + 1;
            stateColors[7] = NormalColorId + 2;
            stateColors[8] = NormalColorId + 2;
            stateColors[9] = NormalColorId + 2;
            stateColors[10] = NormalColorId + 2;
            stateColors[11] = NormalColorId + 2;
            stateColors[12] = NormalColorId + 0;
            stateColors[13] = NormalColorId + 3;
            stateColors[14] = NormalColorId + 3;
            stateColors[15] = NormalColorId + 3;
            stateColors[16] = NormalColorId + 3;
            stateColors[17] = NormalColorId + 3;
            stateColors[18] = NormalColorId + 3;
            stateColors[19] = NormalColorId + 3;
            stateColors[20] = NormalColorId + 3;
            stateColors[21] = NormalColorId + 4;
            stateColors[22] = NormalColorId + 0;
            stateColors[23] = NormalColorId + 4;
            stateColors[24] = NormalColorId + 4;
            stateColors[25] = NormalColorId + 4;

            transitions[0, 0] = 1;
            transitions[0, 1] = 1;
            transitions[0, 2] = 10;
            transitions[0, 3] = 7;
            transitions[0, 4] = 1;
            transitions[0, 5] = 1;
            transitions[0, 6] = 22;
            transitions[0, 7] = 13;
            transitions[0, 8] = 13;
            transitions[0, 9] = 13;
            transitions[0, 10] = 13;
            transitions[0, 11] = 13;
            transitions[0, 12] = 13;
            transitions[0, 13] = 13;
            transitions[0, 14] = 4;
            transitions[0, 15] = 2;
            transitions[0, 16] = 2;
            transitions[0, 17] = 12;
            transitions[0, 18] = 1;
            transitions[0, 19] = 2;
            transitions[0, 20] = 2;
            transitions[0, 21] = 2;
            transitions[0, 22] = 1;
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
            transitions[1, 11] = 0;
            transitions[1, 12] = 0;
            transitions[1, 13] = 0;
            transitions[1, 14] = 0;
            transitions[1, 15] = 0;
            transitions[1, 16] = 0;
            transitions[1, 17] = 0;
            transitions[1, 18] = 0;
            transitions[1, 19] = 0;
            transitions[1, 20] = 0;
            transitions[1, 21] = 0;
            transitions[1, 22] = 0;
            transitions[2, 0] = 0;
            transitions[2, 1] = 0;
            transitions[2, 2] = 0;
            transitions[2, 3] = 0;
            transitions[2, 4] = 0;
            transitions[2, 5] = 0;
            transitions[2, 6] = 0;
            transitions[2, 7] = 2;
            transitions[2, 8] = 2;
            transitions[2, 9] = 2;
            transitions[2, 10] = 2;
            transitions[2, 11] = 2;
            transitions[2, 12] = 2;
            transitions[2, 13] = 2;
            transitions[2, 14] = 0;
            transitions[2, 15] = 2;
            transitions[2, 16] = 2;
            transitions[2, 17] = 2;
            transitions[2, 18] = 0;
            transitions[2, 19] = 2;
            transitions[2, 20] = 2;
            transitions[2, 21] = 2;
            transitions[2, 22] = 0;
            transitions[3, 0] = 0;
            transitions[3, 1] = 0;
            transitions[3, 2] = 0;
            transitions[3, 3] = 0;
            transitions[3, 4] = 0;
            transitions[3, 5] = 0;
            transitions[3, 6] = 0;
            transitions[3, 7] = 0;
            transitions[3, 8] = 0;
            transitions[3, 9] = 0;
            transitions[3, 10] = 0;
            transitions[3, 11] = 0;
            transitions[3, 12] = 0;
            transitions[3, 13] = 0;
            transitions[3, 14] = 0;
            transitions[3, 15] = 0;
            transitions[3, 16] = 0;
            transitions[3, 17] = 0;
            transitions[3, 18] = 0;
            transitions[3, 19] = 0;
            transitions[3, 20] = 0;
            transitions[3, 21] = 0;
            transitions[3, 22] = 0;
            transitions[4, 0] = 0;
            transitions[4, 1] = 0;
            transitions[4, 2] = 5;
            transitions[4, 3] = 0;
            transitions[4, 4] = 0;
            transitions[4, 5] = 0;
            transitions[4, 6] = 0;
            transitions[4, 7] = 0;
            transitions[4, 8] = 0;
            transitions[4, 9] = 0;
            transitions[4, 10] = 0;
            transitions[4, 11] = 0;
            transitions[4, 12] = 0;
            transitions[4, 13] = 0;
            transitions[4, 14] = 0;
            transitions[4, 15] = 2;
            transitions[4, 16] = 2;
            transitions[4, 17] = 2;
            transitions[4, 18] = 0;
            transitions[4, 19] = 2;
            transitions[4, 20] = 2;
            transitions[4, 21] = 2;
            transitions[4, 22] = 0;
            transitions[5, 0] = 5;
            transitions[5, 1] = 5;
            transitions[5, 2] = 3;
            transitions[5, 3] = 5;
            transitions[5, 4] = 5;
            transitions[5, 5] = 5;
            transitions[5, 6] = 5;
            transitions[5, 7] = 5;
            transitions[5, 8] = 5;
            transitions[5, 9] = 5;
            transitions[5, 10] = 5;
            transitions[5, 11] = 5;
            transitions[5, 12] = 5;
            transitions[5, 13] = 5;
            transitions[5, 14] = 5;
            transitions[5, 15] = 5;
            transitions[5, 16] = 5;
            transitions[5, 17] = 5;
            transitions[5, 18] = 6;
            transitions[5, 19] = 5;
            transitions[5, 20] = 5;
            transitions[5, 21] = 5;
            transitions[5, 22] = 0;
            transitions[6, 0] = 5;
            transitions[6, 1] = 5;
            transitions[6, 2] = 5;
            transitions[6, 3] = 5;
            transitions[6, 4] = 5;
            transitions[6, 5] = 5;
            transitions[6, 6] = 5;
            transitions[6, 7] = 5;
            transitions[6, 8] = 5;
            transitions[6, 9] = 5;
            transitions[6, 10] = 5;
            transitions[6, 11] = 5;
            transitions[6, 12] = 5;
            transitions[6, 13] = 5;
            transitions[6, 14] = 5;
            transitions[6, 15] = 5;
            transitions[6, 16] = 5;
            transitions[6, 17] = 5;
            transitions[6, 18] = 5;
            transitions[6, 19] = 5;
            transitions[6, 20] = 5;
            transitions[6, 21] = 5;
            transitions[6, 22] = 0;
            transitions[7, 0] = 7;
            transitions[7, 1] = 7;
            transitions[7, 2] = 7;
            transitions[7, 3] = 9;
            transitions[7, 4] = 7;
            transitions[7, 5] = 7;
            transitions[7, 6] = 7;
            transitions[7, 7] = 7;
            transitions[7, 8] = 7;
            transitions[7, 9] = 7;
            transitions[7, 10] = 7;
            transitions[7, 11] = 7;
            transitions[7, 12] = 7;
            transitions[7, 13] = 7;
            transitions[7, 14] = 7;
            transitions[7, 15] = 7;
            transitions[7, 16] = 7;
            transitions[7, 17] = 7;
            transitions[7, 18] = 8;
            transitions[7, 19] = 7;
            transitions[7, 20] = 7;
            transitions[7, 21] = 7;
            transitions[7, 22] = 0;
            transitions[8, 0] = 7;
            transitions[8, 1] = 7;
            transitions[8, 2] = 7;
            transitions[8, 3] = 7;
            transitions[8, 4] = 7;
            transitions[8, 5] = 7;
            transitions[8, 6] = 7;
            transitions[8, 7] = 7;
            transitions[8, 8] = 7;
            transitions[8, 9] = 7;
            transitions[8, 10] = 7;
            transitions[8, 11] = 7;
            transitions[8, 12] = 7;
            transitions[8, 13] = 7;
            transitions[8, 14] = 7;
            transitions[8, 15] = 7;
            transitions[8, 16] = 7;
            transitions[8, 17] = 7;
            transitions[8, 18] = 7;
            transitions[8, 19] = 7;
            transitions[8, 20] = 7;
            transitions[8, 21] = 7;
            transitions[8, 22] = 0;
            transitions[9, 0] = 0;
            transitions[9, 1] = 0;
            transitions[9, 2] = 0;
            transitions[9, 3] = 0;
            transitions[9, 4] = 0;
            transitions[9, 5] = 0;
            transitions[9, 6] = 0;
            transitions[9, 7] = 0;
            transitions[9, 8] = 0;
            transitions[9, 9] = 0;
            transitions[9, 10] = 0;
            transitions[9, 11] = 0;
            transitions[9, 12] = 0;
            transitions[9, 13] = 0;
            transitions[9, 14] = 0;
            transitions[9, 15] = 0;
            transitions[9, 16] = 0;
            transitions[9, 17] = 0;
            transitions[9, 18] = 0;
            transitions[9, 19] = 0;
            transitions[9, 20] = 0;
            transitions[9, 21] = 0;
            transitions[9, 22] = 0;
            transitions[10, 0] = 10;
            transitions[10, 1] = 10;
            transitions[10, 2] = 9;
            transitions[10, 3] = 10;
            transitions[10, 4] = 10;
            transitions[10, 5] = 10;
            transitions[10, 6] = 10;
            transitions[10, 7] = 10;
            transitions[10, 8] = 10;
            transitions[10, 9] = 10;
            transitions[10, 10] = 10;
            transitions[10, 11] = 10;
            transitions[10, 12] = 10;
            transitions[10, 13] = 10;
            transitions[10, 14] = 10;
            transitions[10, 15] = 10;
            transitions[10, 16] = 10;
            transitions[10, 17] = 10;
            transitions[10, 18] = 11;
            transitions[10, 19] = 10;
            transitions[10, 20] = 10;
            transitions[10, 21] = 10;
            transitions[10, 22] = 0;
            transitions[11, 0] = 10;
            transitions[11, 1] = 10;
            transitions[11, 2] = 10;
            transitions[11, 3] = 10;
            transitions[11, 4] = 10;
            transitions[11, 5] = 10;
            transitions[11, 6] = 10;
            transitions[11, 7] = 10;
            transitions[11, 8] = 10;
            transitions[11, 9] = 10;
            transitions[11, 10] = 10;
            transitions[11, 11] = 10;
            transitions[11, 12] = 10;
            transitions[11, 13] = 10;
            transitions[11, 14] = 10;
            transitions[11, 15] = 10;
            transitions[11, 16] = 10;
            transitions[11, 17] = 10;
            transitions[11, 18] = 10;
            transitions[11, 19] = 10;
            transitions[11, 20] = 10;
            transitions[11, 21] = 10;
            transitions[11, 22] = 0;
            transitions[12, 0] = 1;
            transitions[12, 1] = 1;
            transitions[12, 2] = 10;
            transitions[12, 3] = 7;
            transitions[12, 4] = 1;
            transitions[12, 5] = 1;
            transitions[12, 6] = 1;
            transitions[12, 7] = 4;
            transitions[12, 8] = 4;
            transitions[12, 9] = 4;
            transitions[12, 10] = 4;
            transitions[12, 11] = 4;
            transitions[12, 12] = 4;
            transitions[12, 13] = 4;
            transitions[12, 14] = 1;
            transitions[12, 15] = 4;
            transitions[12, 16] = 4;
            transitions[12, 17] = 4;
            transitions[12, 18] = 1;
            transitions[12, 19] = 4;
            transitions[12, 20] = 4;
            transitions[12, 21] = 4;
            transitions[12, 22] = 1;
            transitions[13, 0] = 0;
            transitions[13, 1] = 0;
            transitions[13, 2] = 0;
            transitions[13, 3] = 0;
            transitions[13, 4] = 0;
            transitions[13, 5] = 14;
            transitions[13, 6] = 0;
            transitions[13, 7] = 13;
            transitions[13, 8] = 13;
            transitions[13, 9] = 13;
            transitions[13, 10] = 13;
            transitions[13, 11] = 13;
            transitions[13, 12] = 13;
            transitions[13, 13] = 13;
            transitions[13, 14] = 0;
            transitions[13, 15] = 0;
            transitions[13, 16] = 0;
            transitions[13, 17] = 0;
            transitions[13, 18] = 0;
            transitions[13, 19] = 0;
            transitions[13, 20] = 16;
            transitions[13, 21] = 16;
            transitions[13, 22] = 0;
            transitions[14, 0] = 0;
            transitions[14, 1] = 0;
            transitions[14, 2] = 0;
            transitions[14, 3] = 0;
            transitions[14, 4] = 0;
            transitions[14, 5] = 0;
            transitions[14, 6] = 0;
            transitions[14, 7] = 14;
            transitions[14, 8] = 14;
            transitions[14, 9] = 14;
            transitions[14, 10] = 14;
            transitions[14, 11] = 14;
            transitions[14, 12] = 14;
            transitions[14, 13] = 14;
            transitions[14, 14] = 0;
            transitions[14, 15] = 0;
            transitions[14, 16] = 15;
            transitions[14, 17] = 0;
            transitions[14, 18] = 0;
            transitions[14, 19] = 15;
            transitions[14, 20] = 0;
            transitions[14, 21] = 0;
            transitions[14, 22] = 0;
            transitions[15, 0] = 0;
            transitions[15, 1] = 0;
            transitions[15, 2] = 0;
            transitions[15, 3] = 0;
            transitions[15, 4] = 0;
            transitions[15, 5] = 0;
            transitions[15, 6] = 0;
            transitions[15, 7] = 0;
            transitions[15, 8] = 0;
            transitions[15, 9] = 0;
            transitions[15, 10] = 0;
            transitions[15, 11] = 0;
            transitions[15, 12] = 0;
            transitions[15, 13] = 0;
            transitions[15, 14] = 0;
            transitions[15, 15] = 0;
            transitions[15, 16] = 0;
            transitions[15, 17] = 0;
            transitions[15, 18] = 0;
            transitions[15, 19] = 0;
            transitions[15, 20] = 0;
            transitions[15, 21] = 0;
            transitions[15, 22] = 0;
            transitions[16, 0] = 0;
            transitions[16, 1] = 0;
            transitions[16, 2] = 0;
            transitions[16, 3] = 0;
            transitions[16, 4] = 0;
            transitions[16, 5] = 0;
            transitions[16, 6] = 0;
            transitions[16, 7] = 0;
            transitions[16, 8] = 18;
            transitions[16, 9] = 0;
            transitions[16, 10] = 19;
            transitions[16, 11] = 0;
            transitions[16, 12] = 20;
            transitions[16, 13] = 17;
            transitions[16, 14] = 0;
            transitions[16, 15] = 0;
            transitions[16, 16] = 0;
            transitions[16, 17] = 0;
            transitions[16, 18] = 0;
            transitions[16, 19] = 0;
            transitions[16, 20] = 0;
            transitions[16, 21] = 0;
            transitions[16, 22] = 0;
            transitions[17, 0] = 0;
            transitions[17, 1] = 0;
            transitions[17, 2] = 0;
            transitions[17, 3] = 0;
            transitions[17, 4] = 0;
            transitions[17, 5] = 0;
            transitions[17, 6] = 0;
            transitions[17, 7] = 0;
            transitions[17, 8] = 0;
            transitions[17, 9] = 0;
            transitions[17, 10] = 0;
            transitions[17, 11] = 0;
            transitions[17, 12] = 0;
            transitions[17, 13] = 0;
            transitions[17, 14] = 0;
            transitions[17, 15] = 0;
            transitions[17, 16] = 0;
            transitions[17, 17] = 0;
            transitions[17, 18] = 0;
            transitions[17, 19] = 0;
            transitions[17, 20] = 0;
            transitions[17, 21] = 0;
            transitions[17, 22] = 0;
            transitions[18, 0] = 0;
            transitions[18, 1] = 0;
            transitions[18, 2] = 0;
            transitions[18, 3] = 0;
            transitions[18, 4] = 0;
            transitions[18, 5] = 0;
            transitions[18, 6] = 0;
            transitions[18, 7] = 0;
            transitions[18, 8] = 0;
            transitions[18, 9] = 0;
            transitions[18, 10] = 0;
            transitions[18, 11] = 0;
            transitions[18, 12] = 17;
            transitions[18, 13] = 0;
            transitions[18, 14] = 0;
            transitions[18, 15] = 0;
            transitions[18, 16] = 0;
            transitions[18, 17] = 0;
            transitions[18, 18] = 0;
            transitions[18, 19] = 0;
            transitions[18, 20] = 0;
            transitions[18, 21] = 0;
            transitions[18, 22] = 0;
            transitions[19, 0] = 0;
            transitions[19, 1] = 0;
            transitions[19, 2] = 0;
            transitions[19, 3] = 0;
            transitions[19, 4] = 0;
            transitions[19, 5] = 0;
            transitions[19, 6] = 0;
            transitions[19, 7] = 0;
            transitions[19, 8] = 0;
            transitions[19, 9] = 17;
            transitions[19, 10] = 0;
            transitions[19, 11] = 0;
            transitions[19, 12] = 0;
            transitions[19, 13] = 0;
            transitions[19, 14] = 0;
            transitions[19, 15] = 0;
            transitions[19, 16] = 0;
            transitions[19, 17] = 0;
            transitions[19, 18] = 0;
            transitions[19, 19] = 0;
            transitions[19, 20] = 0;
            transitions[19, 21] = 0;
            transitions[19, 22] = 0;
            transitions[20, 0] = 0;
            transitions[20, 1] = 0;
            transitions[20, 2] = 0;
            transitions[20, 3] = 0;
            transitions[20, 4] = 0;
            transitions[20, 5] = 0;
            transitions[20, 6] = 0;
            transitions[20, 7] = 0;
            transitions[20, 8] = 0;
            transitions[20, 9] = 0;
            transitions[20, 10] = 0;
            transitions[20, 11] = 17;
            transitions[20, 12] = 0;
            transitions[20, 13] = 0;
            transitions[20, 14] = 0;
            transitions[20, 15] = 0;
            transitions[20, 16] = 0;
            transitions[20, 17] = 0;
            transitions[20, 18] = 0;
            transitions[20, 19] = 0;
            transitions[20, 20] = 0;
            transitions[20, 21] = 0;
            transitions[20, 22] = 0;
            transitions[21, 0] = 21;
            transitions[21, 1] = 21;
            transitions[21, 2] = 21;
            transitions[21, 3] = 21;
            transitions[21, 4] = 24;
            transitions[21, 5] = 21;
            transitions[21, 6] = 0;
            transitions[21, 7] = 21;
            transitions[21, 8] = 21;
            transitions[21, 9] = 21;
            transitions[21, 10] = 21;
            transitions[21, 11] = 21;
            transitions[21, 12] = 21;
            transitions[21, 13] = 21;
            transitions[21, 14] = 21;
            transitions[21, 15] = 21;
            transitions[21, 16] = 21;
            transitions[21, 17] = 21;
            transitions[21, 18] = 21;
            transitions[21, 19] = 21;
            transitions[21, 20] = 21;
            transitions[21, 21] = 21;
            transitions[21, 22] = 0;
            transitions[22, 0] = 1;
            transitions[22, 1] = 1;
            transitions[22, 2] = 1;
            transitions[22, 3] = 1;
            transitions[22, 4] = 21;
            transitions[22, 5] = 1;
            transitions[22, 6] = 23;
            transitions[22, 7] = 1;
            transitions[22, 8] = 1;
            transitions[22, 9] = 1;
            transitions[22, 10] = 1;
            transitions[22, 11] = 1;
            transitions[22, 12] = 1;
            transitions[22, 13] = 1;
            transitions[22, 14] = 1;
            transitions[22, 15] = 1;
            transitions[22, 16] = 1;
            transitions[22, 17] = 1;
            transitions[22, 18] = 1;
            transitions[22, 19] = 1;
            transitions[22, 20] = 1;
            transitions[22, 21] = 1;
            transitions[22, 22] = 1;
            transitions[23, 0] = 23;
            transitions[23, 1] = 0;
            transitions[23, 2] = 23;
            transitions[23, 3] = 23;
            transitions[23, 4] = 23;
            transitions[23, 5] = 23;
            transitions[23, 6] = 23;
            transitions[23, 7] = 23;
            transitions[23, 8] = 23;
            transitions[23, 9] = 23;
            transitions[23, 10] = 23;
            transitions[23, 11] = 23;
            transitions[23, 12] = 23;
            transitions[23, 13] = 23;
            transitions[23, 14] = 23;
            transitions[23, 15] = 23;
            transitions[23, 16] = 23;
            transitions[23, 17] = 23;
            transitions[23, 18] = 23;
            transitions[23, 19] = 23;
            transitions[23, 20] = 23;
            transitions[23, 21] = 23;
            transitions[23, 22] = 0;
            transitions[24, 0] = 21;
            transitions[24, 1] = 21;
            transitions[24, 2] = 21;
            transitions[24, 3] = 21;
            transitions[24, 4] = 21;
            transitions[24, 5] = 21;
            transitions[24, 6] = 25;
            transitions[24, 7] = 21;
            transitions[24, 8] = 21;
            transitions[24, 9] = 21;
            transitions[24, 10] = 21;
            transitions[24, 11] = 21;
            transitions[24, 12] = 21;
            transitions[24, 13] = 21;
            transitions[24, 14] = 21;
            transitions[24, 15] = 21;
            transitions[24, 16] = 21;
            transitions[24, 17] = 21;
            transitions[24, 18] = 21;
            transitions[24, 19] = 21;
            transitions[24, 20] = 21;
            transitions[24, 21] = 21;
            transitions[24, 22] = 0;
            transitions[25, 0] = 0;
            transitions[25, 1] = 0;
            transitions[25, 2] = 0;
            transitions[25, 3] = 0;
            transitions[25, 4] = 0;
            transitions[25, 5] = 0;
            transitions[25, 6] = 0;
            transitions[25, 7] = 0;
            transitions[25, 8] = 0;
            transitions[25, 9] = 0;
            transitions[25, 10] = 0;
            transitions[25, 11] = 0;
            transitions[25, 12] = 0;
            transitions[25, 13] = 0;
            transitions[25, 14] = 0;
            transitions[25, 15] = 0;
            transitions[25, 16] = 0;
            transitions[25, 17] = 0;
            transitions[25, 18] = 0;
            transitions[25, 19] = 0;
            transitions[25, 20] = 0;
            transitions[25, 21] = 0;
            transitions[25, 22] = 0;
        }
    }
}
