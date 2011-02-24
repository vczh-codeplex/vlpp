using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using System.Drawing;

namespace Developer.LanguageServices.NativeX
{
    partial class NativeXColorizer
    {
        public const int BreakPointColorId = 0;
        public const int SnippetColorId = 1;
        public const int BlockColorId = 2;
        public const int TypeColorId = 3;
        public const int NormalColorId = 4;

        private static readonly Color HighlightColor = Color.FromArgb(173, 214, 255);
        private static readonly Color NormalColor = Color.FromArgb(0, 0, 0);
        private static readonly Color IdColor = Color.FromArgb(0, 0, 255);
        private static readonly Color StringColor = Color.FromArgb(163, 21, 21);
        private static readonly Color NumberColor = Color.FromArgb(128, 0, 255);
        private static readonly Color CommentColor = Color.FromArgb(0, 128, 0);
        private static readonly Color AttributeColor = Color.FromArgb(128, 0, 255);

        private static readonly Color BreakPointColor = Color.FromArgb(255, 255, 255);
        private static readonly Color BreakPointHighlightColor = Color.FromArgb(123, 119, 166);
        private static readonly Color SnippetColor = Color.FromArgb(0, 0, 0);
        private static readonly Color SnippetHighlightColor = Color.FromArgb(173, 200, 198);
        private static readonly Color TypeColor = Color.FromArgb(43, 145, 175);
        private static readonly Color BlockColor = Color.Gray;

        public static readonly Color BreakPointBackgroundColor = Color.FromArgb(171, 97, 107);
        public static readonly Color SnippetBackgroundColor = Color.FromArgb(255, 231, 160);

        private void CreateAdditionalColors()
        {
            this.colorItems[BreakPointColorId] = new TextEditorColorItem()
            {
                Text = BreakPointColor,
                HighlightText = BreakPointColor,
                Highlight = BreakPointHighlightColor
            };
            this.colorItems[SnippetColorId] = new TextEditorColorItem()
            {
                Text = SnippetColor,
                HighlightText = SnippetColor,
                Highlight = SnippetHighlightColor
            };
            this.colorItems[BlockColorId] = new TextEditorColorItem()
            {
                Text = BlockColor,
                HighlightText = BlockColor,
                Highlight = HighlightColor
            };
            this.colorItems[TypeColorId] = new TextEditorColorItem()
            {
                Text = TypeColor,
                HighlightText = TypeColor,
                Highlight = HighlightColor
            };
        }

        private bool IsValidId(string token)
        {
            return Array.BinarySearch(keywords, token) >= 0;
        }

        private static string[] keywords = new string[]{
			"true"
			, "false"
			, "null"
			, "exception"
			, "result"
            , "stackdata"
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
			, "int"
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
            , "sizeof"
            , "typeof"
            , "offsetof"
        }.OrderBy(s => s).ToArray();
    }
}
