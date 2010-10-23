using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    partial class NativeXTokenizer
    {
        public const int NormalKeywordToken = ColorizerCustomTokenStart + 0;
        public const int TypeKeywordToken = ColorizerCustomTokenStart + 1;

        private void InjectNativeId(ref CodeToken token)
        {
            if (Array.BinarySearch(normalKeywords, token.Value) >= 0)
            {
                token.Id = NormalKeywordToken;
            }
            else if (Array.BinarySearch(typedKeywords, token.Value) >= 0)
            {
                token.Id = TypeKeywordToken;
            }
        }

        private void InjectId(ref CodeToken token)
        {
            if (Array.BinarySearch(normalKeywords, token.Value) >= 0)
            {
                token.Id = NormalKeywordToken;
            }
            else if (Array.BinarySearch(typedKeywords, token.Value) >= 0)
            {
                token.Id = TypeKeywordToken;
            }
        }

        private static string[] typedKeywords = new string[]{
			"int"
			, "int8"
			, "int16"
			, "int32"
			, "int64"
			, "uint"
			, "uint8"
			, "uint16"
			, "uint32"
			, "uint64"
			, "f32"
			, "f64"
			, "bool"
			, "char"
			, "wchar"
			, "void"
        }.OrderBy(s => s).ToArray();

        private static string[] normalKeywords = new string[]{
			"true"
			, "false"
			, "null"
			, "exception"
			, "result"
			, "function"
			, "cast"
			, "variable"
			, "if"
			, "else"
			, "break"
			, "continue"
			, "exit"
			, "while"
			, "do"
			, "loop"
			, "when"
			, "for"
			, "with"
			, "try"
			, "catch"
			, "throw"
			, "type"
			, "structure"
			, "unit"
			, "uses"
			, "alias"
			, "generic"
			, "concept"
			, "instance"
			, "where"
			, "foreign"
        }.OrderBy(s => s).ToArray();
    }
}
