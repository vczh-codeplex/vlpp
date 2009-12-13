using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using System.Drawing;

namespace AsHost
{
    public class CodeFileEditor : CodeEditorWindowPane
    {
        private ICodeFormatter codeFormatter = null;

        protected override ICodeFormatter CodeFormatter
        {
            get
            {
                return this.codeFormatter;
            }
        }

        public CodeFileEditor()
        {
            RegexCodeFormatter formatter = new RegexCodeFormatter();
            this.codeFormatter = formatter;

            formatter.DefaultFormat = new CodeFormat()
            {
            };

            formatter.AddFormat(
                @"using|namespace|public|protected|internal|private|class|struct|override|abstract|virtual|this|bool|char|int|float|double|string|true|false",
                new CodeFormat()
                {
                    TextColor = Color.Blue
                });

            formatter.AddFormat(
                @"@(""[^""]*"")+|""([^""\\]|\\.)*""",
                new CodeFormat()
                {
                    TextColor = Color.Brown
                });

            formatter.AddFormat(
                @"[a-zA-Z_.][a-zA-Z0-9_.]*",
                new CodeFormat()
                {
                    TextColor = Color.Black
                });

            formatter.AddFormat(
                @"//[^\n]*(\n|$)|/\*([^\*]|\*+[^/])*\*+/",
                new CodeFormat()
                {
                    TextColor = Color.DarkGreen
                });
        }
    }
}
