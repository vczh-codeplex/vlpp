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
    }
}
