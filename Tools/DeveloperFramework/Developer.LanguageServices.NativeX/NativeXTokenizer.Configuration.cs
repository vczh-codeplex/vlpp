using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    partial class NativeXTokenizer
    {
        public int KeywordToken = ColorizerCustomTokenStart + 0;

        private void InjectNativeId(ref CodeToken token)
        {
            if (Array.BinarySearch(NativeXColorizer.Keywords, token) >= 0)
            {
                token.Id = KeywordToken;
            }
        }

        private void InjectId(ref CodeToken token)
        {
            if (Array.BinarySearch(NativeXColorizer.Keywords, token) >= 0)
            {
                token.Id = KeywordToken;
            }
        }
    }
}
