using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;
using System.Drawing;
using System.Windows.Forms;
using System.Xml.Linq;

namespace Developer.WinFormControls.Extension
{
    public class TextEditorBoxExtensibleControlPanel : ITextEditorControlPanel
    {
        private int width = 0;
        private List<ITextEditorBoxEditingObserverExtension> editingObserverExtensions = new List<ITextEditorBoxEditingObserverExtension>();
        private List<ITextEditorBoxHeaderExtension> headerExtensions = new List<ITextEditorBoxHeaderExtension>();
        private List<ITextEditorBoxLineExtension> lineExtensions = new List<ITextEditorBoxLineExtension>();
        private List<ITextEditorBoxColorizerExtension> colorizerExtensions = new List<ITextEditorBoxColorizerExtension>();
        private List<ITextEditorBoxPopupListExtension> popupListExtensions = new List<ITextEditorBoxPopupListExtension>();
        private List<ITextEditorBoxTooltipExtension> tooltipExtensions = new List<ITextEditorBoxTooltipExtension>();
        private List<ITextEditorBoxWordingExtension> wordingExtensions = new List<ITextEditorBoxWordingExtension>();
        private List<ITextEditorBoxExtension> extensions = new List<ITextEditorBoxExtension>();

        public TextEditorBoxExtensibleControlPanel()
        {
        }

        public void ExtendBeforeInstall(ITextEditorBoxExtension extension)
        {
            ITextEditorBoxEditingObserverExtension editingObserver = extension as ITextEditorBoxEditingObserverExtension;
            ITextEditorBoxHeaderExtension header = extension as ITextEditorBoxHeaderExtension;
            ITextEditorBoxLineExtension line = extension as ITextEditorBoxLineExtension;
            ITextEditorBoxColorizerExtension colorizer = extension as ITextEditorBoxColorizerExtension;
            ITextEditorBoxPopupListExtension popupList = extension as ITextEditorBoxPopupListExtension;
            ITextEditorBoxTooltipExtension tooltip = extension as ITextEditorBoxTooltipExtension;
            ITextEditorBoxWordingExtension wording = extension as ITextEditorBoxWordingExtension;

            if (editingObserver != null) this.editingObserverExtensions.Add(editingObserver);
            if (header != null) this.headerExtensions.Add(header);
            if (line != null) this.lineExtensions.Add(line);
            if (colorizer != null) this.colorizerExtensions.Add(colorizer);
            if (popupList != null) this.popupListExtensions.Add(popupList);
            if (tooltip != null) this.tooltipExtensions.Add(tooltip);
            if (wording != null) this.wordingExtensions.Add(wording);

            this.extensions.Add(extension);
            if (header != null)
            {
                this.width += header.Width;
            }
        }

        public int Width
        {
            get
            {
                return width;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            foreach (var extension in this.extensions)
            {
                extension.Attach(callback);
            }
        }

        public void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
            foreach (var ex in this.editingObserverExtensions)
            {
                ex.OnBeforeEdit(start, end, ref lines);
            }
        }

        public void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            foreach (var ex in this.editingObserverExtensions)
            {
                ex.OnAfterEdit(start, oldEnd, newEnd);
            }
        }

        public string OnGetSimpleTooltip(TextPosition pos)
        {
            foreach (var ex in this.tooltipExtensions)
            {
                string result = ex.OnGetSimpleTooltip(pos);
                if (!string.IsNullOrEmpty(result))
                {
                    return result;
                }
            }
            return null;
        }

        public Tuple<XDocument, TextPosition> OnGetQuickInfoTooltip()
        {
            foreach (var ex in this.tooltipExtensions)
            {
                var result = ex.OnGetQuickInfoTooltip();
                if (result != null)
                {
                    return result;
                }
            }
            return null;
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            foreach (var ex in this.colorizerExtensions)
            {
                if (ex.NeedColorLineForDisplay(lineIndex))
                {
                    return true;
                }
            }
            return false;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            foreach (var ex in this.colorizerExtensions)
            {
                if (ex.NeedColorLineForDisplay(lineIndex))
                {
                    ex.ColorLineForDisplay(lineIndex, colors);
                }
            }
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            foreach (var ex in this.lineExtensions)
            {
                ex.DrawLineBackground(g, lineIndex, backgroundArea);
            }
        }

        public void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            foreach (var ex in this.lineExtensions)
            {
                ex.DrawLineForeground(g, lineIndex, backgroundArea);
            }
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
            foreach (var ex in this.headerExtensions)
            {
                ex.DrawControlPanel(g, lineIndex, controlPanelArea);
            }
        }

        public void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea)
        {
            foreach (var ex in this.headerExtensions)
            {
                ex.DrawControlPanelBackground(g, backgroundArea);
            }
        }

        public void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
            foreach (var ex in this.headerExtensions)
            {
                ex.OnMouseDown(lineIndex, controlPanelArea, relativePosition, buttons);
            }
        }

        public void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
            foreach (var ex in this.headerExtensions)
            {
                ex.OnMouseMove(lineIndex, controlPanelArea, relativePosition, buttons);
            }
        }

        public void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
            foreach (var ex in this.headerExtensions)
            {
                ex.OnMouseUp(lineIndex, controlPanelArea, relativePosition, buttons);
            }
        }

        public bool IsPopupListKeyAcceptable(KeyEventArgs e)
        {
            foreach (var ex in this.popupListExtensions)
            {
                if (ex.IsPopupListKeyAcceptable(e))
                {
                    return true;
                }
            }
            return false;
        }

        public bool IsPopupListCharAcceptable(char c)
        {
            foreach (var ex in this.popupListExtensions)
            {
                if (ex.IsPopupListCharAcceptable(c))
                {
                    return true;
                }
            }
            return false;
        }

        public void PopupListItemSelected(string searchingKey, string text)
        {
            foreach (var ex in this.popupListExtensions)
            {
                ex.PopupListItemSelected(searchingKey, text);
            }
        }

        public TextPosition GetLeftWord(TextPosition caret)
        {
            foreach (var ex in this.wordingExtensions)
            {
                TextPosition result = ex.GetLeftWord(caret);
                if (result != caret)
                {
                    return result;
                }
            }
            return caret;
        }

        public TextPosition GetRightWord(TextPosition caret)
        {
            foreach (var ex in this.wordingExtensions)
            {
                TextPosition result = ex.GetRightWord(caret);
                if (result != caret)
                {
                    return result;
                }
            }
            return caret;
        }
    }

    public interface ITextEditorBoxExtension
    {
        void Attach(ITextEditorControlPanelCallBack callback);
    }

    public interface ITextEditorBoxEditingObserverExtension : ITextEditorBoxExtension
    {
        void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines);
        void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd);
    }

    public interface ITextEditorBoxHeaderExtension : ITextEditorBoxExtension
    {
        int Width { get; }
        void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea);
        void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea);
        void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
        void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons);
    }

    public interface ITextEditorBoxLineExtension : ITextEditorBoxExtension
    {
        void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea);
        void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea);
    }

    public interface ITextEditorBoxColorizerExtension : ITextEditorBoxExtension
    {
        bool NeedColorLineForDisplay(int lineIndex);
        void ColorLineForDisplay(int lineIndex, int[] colors);
    }

    public interface ITextEditorBoxPopupListExtension : ITextEditorBoxExtension
    {
        bool IsPopupListKeyAcceptable(KeyEventArgs e);
        bool IsPopupListCharAcceptable(char c);
        void PopupListItemSelected(string searchingKey, string text);
    }

    public interface ITextEditorBoxTooltipExtension : ITextEditorBoxExtension
    {
        string OnGetSimpleTooltip(TextPosition pos);
        Tuple<XDocument, TextPosition> OnGetQuickInfoTooltip();
    }

    public interface ITextEditorBoxWordingExtension : ITextEditorBoxExtension
    {
        TextPosition GetLeftWord(TextPosition caret);
        TextPosition GetRightWord(TextPosition caret);
    }
}
