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

        public int ColorizeLine(char[] items, int length, int initialState, int[] colors)
        {
            int count = items.Length;
            for (int i = 0; i < length; i++)
            {
                colors[i] = 0;
            }
            return initialState;
        }
    }

    public class TextEditorControlPanel : ITextEditorControlPanel
    {
        public int Width
        {
            get
            {
                return 0;
            }
        }

        public void DrawLineBackground(Graphics g, char[] items, int lineIndex, Point textPosition, Rectangle backgroundArea)
        {
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
        }

        public void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }
    }
}
