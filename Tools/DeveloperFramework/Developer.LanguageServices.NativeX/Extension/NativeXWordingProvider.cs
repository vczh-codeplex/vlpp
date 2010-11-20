using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXWordingProvider : LanguageWordingExtension
    {
        public class NativeXProvider : ILanguageWordingProvider
        {
            protected char[] chars = @"[](){}.,;:!~+-*/%^|&=<>".OrderBy(c => c).ToArray();

            public virtual int GetNonNegativeCharClass(char c)
            {
                if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_')
                {
                    return 0;
                }
                else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
                {
                    return 1;
                }
                else if (Array.BinarySearch(this.chars, c) != -1)
                {
                    return 2;
                }
                else
                {
                    return 3;
                }
            }
        }

        public NativeXWordingProvider()
            : base(new NativeXProvider())
        {
        }

        public NativeXWordingProvider(NativeXProvider provider)
            : base(provider)
        {
        }
    }
}
