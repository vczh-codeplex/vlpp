using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;

namespace Developer.WinFormControls
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

    public interface ITextEditorControlPanelCallBack
    {
        object GetLineData(int index);
        void BindLineData(int index, object data);
        TextEditorBox TextEditorBox { get; }
    }

    public interface ITextEditorControlPanel
    {
        int Width { get; }
        void InstallCallBack(ITextEditorControlPanelCallBack callback);
        void OnEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd);
        bool NeedColorLineForDisplay(int lineIndex);
        void ColorLineForDisplay(int lineIndex, int[] colors);

        void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea);
        void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea);
        void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea);
        void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea);
        void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
    }
}
