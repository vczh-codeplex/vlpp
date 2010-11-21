using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Windows.Forms;
using System.Xml.Linq;

namespace Developer.WinFormControls
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

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
        }

        public void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
        }

        public string OnGetSimpleTooltip(TextPosition pos)
        {
            return null;
        }

        public XDocument OnGetQuickInfoTooltip()
        {
            return null;
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            return false;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
        }

        public void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea)
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

        public bool IsPopupListKeyAcceptable(KeyEventArgs e)
        {
            return false;
        }

        public bool IsPopupListCharAcceptable(char c)
        {
            return false;
        }

        public void PopupListItemSelected(string searchingKey, string text)
        {
        }

        public TextPosition GetLeftWord(TextPosition caret)
        {
            return caret;
        }

        public TextPosition GetRightWord(TextPosition caret)
        {
            return caret;
        }
    }
}
