using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace CodeBoxControl
{
    public class TextEditorColorItem
    {
        private Color lastHighlight = Color.Empty;
        private SolidBrush lastHighlightBrush = null;

        public SolidBrush HighlightBrush
        {
            get
            {
                if (this.lastHighlight != this.Highlight)
                {
                    if (this.lastHighlightBrush != null)
                    {
                        this.lastHighlightBrush.Dispose();
                    }
                    this.lastHighlight = this.Highlight;
                    this.lastHighlightBrush = new SolidBrush(this.lastHighlight);
                }
                return this.lastHighlightBrush;
            }
        }

        public Color Text { get; set; }
        public Color HighlightText { get; set; }
        public Color Highlight { get; set; }
    }

    public interface ITextEditorColorizer
    {
        TextEditorColorItem[] ColorItems { get; }
        int ColorizeLine(char[] items, int length, int initialState, int[] colors);
    }

    public interface ITextEditorControlPanel
    {
        int Width { get; }
        void DrawLineBackground(Graphics g, char[] items, int lineIndex, Point textPosition, Rectangle backgroundArea);
        void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea);
        void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
    }
}
