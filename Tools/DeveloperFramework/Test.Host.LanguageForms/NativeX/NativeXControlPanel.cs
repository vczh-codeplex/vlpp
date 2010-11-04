using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Drawing;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms.NativeX
{
    class NativeXControlPanel
        : ITextEditorControlPanel
        , IDisposable
    {
        private ITextEditorControlPanelCallBack callback = null;
        private NativeXAnalyzingResult analyzingResult = null;
        private NativeXCodeAnalyzer analyzer = null;

        private TextPosition grayStart = new TextPosition(0, 0);
        private TextPosition grayEnd = new TextPosition(0, 0);

        private NativeXForm form = null;
        private int counter = 0;

        public NativeXControlPanel(NativeXForm form)
        {
            this.form = form;
        }

        public virtual void Dispose()
        {
            this.analyzer.Dispose();
        }

        #region ITextEditorControlPanel Members

        public virtual int Width
        {
            get
            {
                return 0;
            }
        }

        public virtual void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.analyzer = new NativeXCodeAnalyzer();
            this.analyzer.Received += new CalculationNotifierReceivedHandler<NativeXAnalyzingResult>(Analyzer_Received);
            this.callback = callback;
            this.callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public virtual void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public virtual void OnAfterEdit(Developer.LanguageProvider.TextPosition start, Developer.LanguageProvider.TextPosition oldEnd, Developer.LanguageProvider.TextPosition newEnd)
        {
            this.analyzer.Analyze(this.callback.TextEditorBox.Text);
        }

        public virtual bool NeedColorLineForDisplay(int lineIndex)
        {
            return this.grayStart != this.grayEnd && this.grayStart.row <= lineIndex && lineIndex <= this.grayEnd.row;
        }

        public virtual void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            TextLine<TextEditorBox.LineInfo> line = this.callback.TextEditorBox.TextProvider[lineIndex];
            int length = line.CharCount;

            int start = grayStart.row == lineIndex ? Math.Min(grayStart.col, length) : 0;
            int end = grayEnd.row == lineIndex ? Math.Min(grayEnd.col, length) : line.CharCount;
            for (int i = start; i < end; i++)
            {
                colors[i] = NativeXColorizer.BlockPointColorId;
            }
        }

        public virtual void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public virtual void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            if (NeedColorLineForDisplay(lineIndex))
            {
                TextLine<TextEditorBox.LineInfo> line = this.callback.TextEditorBox.TextProvider[lineIndex];
                int length = line.CharCount;
                int start = grayStart.row == lineIndex ? Math.Min(grayStart.col, length) : 0;
                int end = grayEnd.row == lineIndex ? Math.Min(grayEnd.col, length) : line.CharCount;

                int x1 = this.callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, start)).X;
                int x2 = this.callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, end)).X;
                g.DrawRectangle(Pens.Gray, x1, backgroundArea.Top, x2 - x1, backgroundArea.Height);
            }
        }

        public virtual void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
        }

        public virtual void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea)
        {
        }

        public virtual void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public virtual void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        public virtual void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, System.Windows.Forms.MouseButtons buttons)
        {
        }

        #endregion

        #region Reaction Functions

        public virtual void Analyzer_Received(NativeXAnalyzingResult result, int id)
        {
            TextPosition start, end;
            string treeText, statusText;
            UpdateBlock(result, out start, out end);
            UpdateUnit(result, out treeText, out statusText);

            this.callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.analyzingResult = result;
                this.grayStart = start;
                this.grayEnd = end;
                this.form.TreeText = treeText;
                this.form.StatusText = statusText;
                this.callback.TextEditorBox.Refresh();
            }));
        }

        protected virtual void UpdateUnit(NativeXAnalyzingResult result, out string treeText, out string statusText)
        {
            treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
            statusText = (++this.counter).ToString();
        }

        protected virtual void UpdateBlock(NativeXAnalyzingResult result, out TextPosition start, out TextPosition end)
        {
            TextPosition pos = this.callback.TextEditorBox.SelectionCaret;
            start = new TextPosition(0, 0);
            end = new TextPosition(0, 0);

            if (result != null && result.Unit != null)
            {
                NativeXStatement statement = result.Unit.FindDeepest<NativeXStatement>(pos);
                if (statement != null)
                {
                    start = statement.Start;
                    end = statement.End;
                }
            }
        }

        private void TextEditorBox_SelectionChanged(object sender, EventArgs e)
        {
            TextPosition start, end;
            UpdateBlock(this.analyzingResult, out start, out end);
            this.grayStart = start;
            this.grayEnd = end;
        }

        #endregion
    }
}
