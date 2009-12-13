using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.UI
{
    public interface ICodeFormatter
    {
        CodeFormatManager Manager { get; }
        void BeginFormat(string text);
        void EndFormat();
        bool GetToken(out int format, out int length);
    }
}
