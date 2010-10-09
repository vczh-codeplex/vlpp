using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Drawing;

namespace CodeForm
{
    partial class CSharpColorizer
    {
        public const int BreakPointColorId = 0;
        public const int BlockPointColorId = 1;
        public const int NormalColorId = 2;

        private readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        private readonly Color NormalColor = Color.FromArgb(0, 0, 0);
        private readonly Color IdColor = Color.FromArgb(0, 0, 255);
        private readonly Color StringColor = Color.FromArgb(163, 21, 21);
        private readonly Color CommentColor = Color.FromArgb(0, 128, 0);

        private readonly Color BreakPointColor = Color.FromArgb(255, 255, 255);
        private readonly Color BreakPointHighlightColor = Color.FromArgb(123, 119, 166);
        private readonly Color BlockPointColor = Color.Gray;

        private void CreateAdditionalColors()
        {
            this.colorItems[BreakPointColorId] = new TextEditorColorItem()
            {
                Text = BreakPointColor,
                HighlightText = BreakPointColor,
                Highlight = BreakPointHighlightColor
            };
            this.colorItems[BlockPointColorId] = new TextEditorColorItem()
            {
                Text = BlockPointColor,
                HighlightText = BlockPointColor,
                Highlight = HighlightColor
            };
        }

        private bool IsValidId(string token)
        {
            return token[0] == '#' || Array.BinarySearch(keywords, token) >= 0;
        }

        private static string[] keywords ={
            "abstract",
            "as",
            "base",
            "bool",
            "break",
            "byte",
            "case",
            "catch",
            "char",
            "checked",
            "class",
            "const",
            "continue",
            "decimal",
            "default",
            "delegate",
            "do",
            "double",
            "else",
            "enum",
            "event",
            "explicit",
            "extern",
            "false",
            "finally",
            "fixed",
            "float",
            "for",
            "foreach",
            "goto",
            "if",
            "implicit",
            "in",
            "int",
            "interface",
            "internal",
            "is",
            "lock",
            "long",
            "namespace",
            "new",
            "null",
            "object",
            "operator",
            "out",
            "override",
            "params",
            "private",
            "protected",
            "public",
            "readonly",
            "ref",
            "return",
            "sbyte",
            "sealed",
            "short",
            "sizeof",
            "stackalloc",
            "static",
            "string",
            "struct",
            "switch",
            "this",
            "throw",
            "true",
            "try",
            "typeof",
            "unit",
            "ulong",
            "unchecked",
            "unsafe",
            "ushort",
            "using",
            "virtual",
            "void",
            "volatile",
            "while"
        };
    }
}
