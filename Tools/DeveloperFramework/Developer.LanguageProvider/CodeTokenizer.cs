using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider
{
    public class CodeTokenizer
    {
        protected int[] charset = null;
        protected int[,] transitions = null;
        protected int[] finalStateTokenIds = null;

        public static TextPosition GetStartPosition(List<CodeToken> tokens, int currentToken)
        {
            if (tokens.Count == 0)
            {
                return new TextPosition(-1, -1);
            }
            else if (currentToken < 0)
            {
                return tokens[0].Start;
            }
            else if (currentToken < tokens.Count)
            {
                return tokens[currentToken].Start;
            }
            else
            {
                return tokens[tokens.Count - 1].End;
            }
        }

        public static TextPosition GetEndPosition(List<CodeToken> tokens, int currentToken)
        {
            if (tokens.Count == 0)
            {
                return new TextPosition(-1, -1);
            }
            else if (currentToken < 1)
            {
                return tokens[0].Start;
            }
            else if (currentToken < tokens.Count - 1)
            {
                return tokens[currentToken].Start;
            }
            else if (currentToken == tokens.Count - 1)
            {
                return tokens[currentToken].Start;
            }
            else
            {
                return tokens[tokens.Count - 1].End;
            }
        }

        public static CodeToken ParseToken(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess, int id)
        {
            parseSuccess = false;
            if (currentToken < tokens.Count)
            {
                CodeToken result = tokens[currentToken];
                if (result.Id == id)
                {
                    currentToken++;
                    parseSuccess = true;
                    return result;
                }
            }
            return default(CodeToken);
        }

        public static CodeToken ParseToken(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess, params string[] values)
        {
            parseSuccess = false;
            if (TestToken(tokens, currentToken, values))
            {
                CodeToken result = tokens[currentToken];
                currentToken++;
                parseSuccess = true;
                return result;
            }
            else
            {
                return default(CodeToken);
            }
        }

        public static bool TestToken(List<CodeToken> tokens, int currentToken, params string[] values)
        {
            if (currentToken < tokens.Count)
            {
                CodeToken result = tokens[currentToken];
                foreach (string value in values)
                {
                    if (result.Value == value)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        protected virtual void Inject(ref CodeToken token, int finalState)
        {
        }

        public List<CodeToken> Tokenize(char[] input)
        {
            List<CodeToken> tokens = new List<CodeToken>();
            TextPosition startPosition = new TextPosition(0, 0);
            TextPosition endPosition = new TextPosition(0, 0);
            TextPosition currentPosition = new TextPosition(0, 0);
            int startIndex = 0;
            int endIndex = 0;
            int endId = -1;
            int endState = -1;
            int currentState = 0;

            int length = input.Length;
            for (int i = 0; i < length; i++)
            {
                TextPosition oldCurrentPosition = currentPosition;
                int oldCurrentState = currentState;
                char c = input[i];
                switch (c)
                {
                    case '\n':
                        currentPosition.row++;
                        currentPosition.col = 0;
                        break;
                    case '\r':
                        continue;
                    default:
                        currentPosition.col++;
                        break;
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
                    Inject(ref token, endState);
                    tokens.Add(token);

                    startPosition = endPosition;
                    currentPosition = endPosition;
                    startIndex = endIndex;
                    currentState = 0;
                    endId = -1;
                    endState = -1;
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
                        endState = currentState;
                    }
                }
            }
            {
                CodeToken token = new CodeToken();
                token.Id = endId;
                token.Start = startPosition;
                token.End = currentPosition;
                token.Value = new string(input, startIndex, length - startIndex);
                Inject(ref token, -1);
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
