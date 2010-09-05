using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace CodeBoxControl
{
    public class TextEditorPlanTextColorizer : ITextEditorColorizer
    {
        private TextEditorBox textEditorBox = null;

        public TextEditorPlanTextColorizer(TextEditorBox textEditorBox)
        {
            this.textEditorBox = textEditorBox;
        }

        public TextEditorColorItem[] ColorItems
        {
            get
            {
                TextEditorColorItem item = new TextEditorColorItem()
                {
                    Text = this.textEditorBox.ForeColor,
                    HighlightText = SystemColors.HighlightText,
                    Highlight = SystemColors.Highlight
                };
                return new TextEditorColorItem[] { item };
            }
        }

        public int ColorizeLine(char[] items, int initialState, int[] colors)
        {
            int count = items.Length;
            for (int i = 0; i < count; i++)
            {
                colors[i] = 0;
            }
            return initialState;
        }
    }
}
