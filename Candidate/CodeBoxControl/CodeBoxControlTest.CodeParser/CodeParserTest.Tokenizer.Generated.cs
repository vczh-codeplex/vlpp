using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest.CodeParser
{
    public partial class CodeParserTokenizer : CodeTokenizer
    {
        public const int NumberToken = 0;
        public const int IdToken = 1;
        public const int OperatorToken = 2;

        private const int StartStateId = 0;
        private const int IntegerStateId = 1;
        private const int DoubleWaitingStateId = 2;
        private const int DoubleStateId = 3;
        private const int IdStateId = 4;
        private const int OperatorStateId = 5;

        public CodeParserTokenizer()
        {
            charset = new int[65536];
            transitions = new int[6, 5];
            finalStateTokenIds = new int[6];

            for (int i = 0; i < 40; i++)
                charset[i] = 4;
            for (int i = 40; i < 45; i++)
                charset[i] = 0;
            for (int i = 45; i < 46; i++)
                charset[i] = 4;
            for (int i = 46; i < 47; i++)
                charset[i] = 1;
            for (int i = 47; i < 48; i++)
                charset[i] = 0;
            for (int i = 48; i < 58; i++)
                charset[i] = 2;
            for (int i = 58; i < 61; i++)
                charset[i] = 4;
            for (int i = 61; i < 62; i++)
                charset[i] = 0;
            for (int i = 62; i < 65; i++)
                charset[i] = 4;
            for (int i = 65; i < 91; i++)
                charset[i] = 3;
            for (int i = 91; i < 95; i++)
                charset[i] = 4;
            for (int i = 95; i < 96; i++)
                charset[i] = 3;
            for (int i = 96; i < 97; i++)
                charset[i] = 4;
            for (int i = 97; i < 123; i++)
                charset[i] = 3;
            for (int i = 123; i < 65536; i++)
                charset[i] = 4;

            finalStateTokenIds[0] = -1;
            finalStateTokenIds[1] = 0;
            finalStateTokenIds[2] = -1;
            finalStateTokenIds[3] = 0;
            finalStateTokenIds[4] = 1;
            finalStateTokenIds[5] = 2;

            transitions[0, 0] = 5;
            transitions[0, 1] = -1;
            transitions[0, 2] = 1;
            transitions[0, 3] = 4;
            transitions[0, 4] = -1;
            transitions[1, 0] = -1;
            transitions[1, 1] = 2;
            transitions[1, 2] = 1;
            transitions[1, 3] = -1;
            transitions[1, 4] = -1;
            transitions[2, 0] = -1;
            transitions[2, 1] = -1;
            transitions[2, 2] = 3;
            transitions[2, 3] = -1;
            transitions[2, 4] = -1;
            transitions[3, 0] = -1;
            transitions[3, 1] = -1;
            transitions[3, 2] = 3;
            transitions[3, 3] = -1;
            transitions[3, 4] = -1;
            transitions[4, 0] = -1;
            transitions[4, 1] = -1;
            transitions[4, 2] = 4;
            transitions[4, 3] = 4;
            transitions[4, 4] = -1;
            transitions[5, 0] = -1;
            transitions[5, 1] = -1;
            transitions[5, 2] = -1;
            transitions[5, 3] = -1;
            transitions[5, 4] = -1;
        }
    }
}
