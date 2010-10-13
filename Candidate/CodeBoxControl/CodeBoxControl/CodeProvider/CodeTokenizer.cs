﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.Core;

namespace CodeBoxControl.CodeProvider
{
    public class CodeTokenizer
    {
        protected int[] charset = null;
        protected int[,] transitions = null;
        protected int[] finalStateTokenIds = null;

        public List<CodeToken> Tokenize(char[] input)
        {
            List<CodeToken> tokens = new List<CodeToken>();
            TextPosition startPosition = new TextPosition(0, 0);
            TextPosition endPosition = new TextPosition(0, 0);
            TextPosition currentPosition = new TextPosition(0, 0);
            int startIndex = 0;
            int endIndex = 0;
            int currentState = 0;
            int endId = -1;

            int length = input.Length;
            for (int i = 0; i < length; i++)
            {
                TextPosition oldCurrentPosition = currentPosition;
                int oldCurrentState = currentState;
                char c = input[i];
                if (c == '\n')
                {
                    currentPosition.row++;
                    currentPosition.col = 0;
                }
                else
                {
                    currentPosition.col++;
                }
                currentState = this.transitions[currentState, this.charset[c]];
                if (currentState == -1)
                {
                    if (endIndex == startIndex)
                    {
                        endPosition = currentPosition;
                        endIndex = i + 1;
                    }

                    CodeToken token = new CodeToken();
                    token.Id = endId;
                    token.Start = startPosition;
                    token.End = endPosition;
                    token.Value = new string(input, startIndex, endIndex - startIndex);
                    tokens.Add(token);

                    startPosition = endPosition;
                    currentPosition = endPosition;
                    startIndex = endIndex;
                    currentState = 0;
                    endId = -1;
                    i = startIndex - 1;
                }
                else
                {
                    int stateTokenId = this.finalStateTokenIds[currentState];
                    if (stateTokenId != -1)
                    {
                        endPosition = currentPosition;
                        endIndex = i + 1;
                        endId = stateTokenId;
                    }
                }
            }
            {
                CodeToken token = new CodeToken();
                token.Id = endId;
                token.Start = startPosition;
                token.End = currentPosition;
                token.Value = new string(input, startIndex, length - startIndex);
                tokens.Add(token);
            }

            return tokens;
        }
    }

    public struct CodeToken
    {
        public int Id { get; set; }
        public TextPosition Start { get; set; }
        public TextPosition End { get; set; }
        public string Value { get; set; }
    }
}
