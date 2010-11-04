using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX
{
    static partial class NativeXCodeParser
    {
        private static NativeXTokenizer tokenizer = null;

        static NativeXCodeParser()
        {
            tokenizer = new NativeXTokenizer();
            CodeNode.Preload(typeof(NativeXUnit).Assembly);
        }

        public static List<CodeToken> Tokenize(char[] input)
        {
            List<CodeToken> tokens = tokenizer.Tokenize(input)
                .Where(t => t.Id == NativeXTokenizer.IdToken
                    || t.Id == NativeXTokenizer.NumberToken
                    || t.Id == NativeXTokenizer.OperatorToken
                    || t.Id == NativeXTokenizer.StringToken
                    || t.Id == NativeXTokenizer.NormalKeywordToken
                    || t.Id == NativeXTokenizer.TypeKeywordToken
                    )
                .ToList();
            return tokens;
        }
    }
}
