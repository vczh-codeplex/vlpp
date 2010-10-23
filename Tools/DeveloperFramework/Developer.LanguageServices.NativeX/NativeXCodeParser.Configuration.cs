using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    static partial class NativeXCodeParser
    {
        public static List<CodeToken> Tokenize(char[] input)
        {
            List<CodeToken> tokens = new NativeXTokenizer().Tokenize(input)
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
