using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public class TextEditorColorItem
    {
        private Color lastHighlight = Color.Empty;
        private Color lastHighlightText = Color.Empty;
        private Color lastText = Color.Empty;
        private SolidBrush lastHighlightBrush = null;
        private SolidBrush lastHighlightTextBrush = null;
        private SolidBrush lastTextBrush = null;

        private SolidBrush GetBrush(ref Color lastColor, Color color, ref SolidBrush brush)
        {
            if (lastColor != color)
            {
                if (brush != null)
                {
                    brush.Dispose();
                }
                lastColor = color;
                brush = new SolidBrush(lastColor);
            }
            return brush;
        }

        public SolidBrush HighlightBrush
        {
            get
            {
                return GetBrush(ref this.lastHighlight, this.Highlight, ref this.lastHighlightBrush);
            }
        }

        public SolidBrush HighlightTextBrush
        {
            get
            {
                return GetBrush(ref this.lastHighlightText, this.HighlightText, ref this.lastHighlightTextBrush);
            }
        }

        public SolidBrush TextBrush
        {
            get
            {
                return GetBrush(ref this.lastText, this.Text, ref this.lastTextBrush);
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
        void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines);
        void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd);
        string OnGetSimpleTooltip(TextPosition pos);
        Tuple<XDocument,TextPosition> OnGetQuickInfoTooltip();
        bool NeedColorLineForDisplay(int lineIndex);
        void ColorLineForDisplay(int lineIndex, int[] colors);

        void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea);
        void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea);
        void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea);
        void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea);
        void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);

        bool IsPopupListKeyAcceptable(KeyEventArgs e);
        bool IsPopupListCharAcceptable(char c);
        void PopupListItemSelected(string searchingKey, string text);

        TextPosition GetLeftWord(TextPosition caret);
        TextPosition GetRightWord(TextPosition caret);
    }
}
