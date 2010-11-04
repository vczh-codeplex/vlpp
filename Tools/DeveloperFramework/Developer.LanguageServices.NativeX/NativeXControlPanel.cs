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

namespace Developer.LanguageServices.NativeX
{
    public class NativeXControlPanel
        : ITextEditorControlPanel
        , INativeXAnalyzingResultReceiver
        , IDisposable
    {
        private ITextEditorControlPanelCallBack callback = null;
        private NativeXAnalyzingResult analyzingResult = null;
        private NativeXCodeAnalyzer analyzer = null;

        private TextPosition grayStart = new TextPosition(0, 0);
        private TextPosition grayEnd = new TextPosition(0, 0);

        public virtual int Width
        {
            get
            {
                return 0;
            }
        }

        public virtual void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.analyzer = new NativeXCodeAnalyzer(this);
            this.callback = callback;
            this.callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public virtual void OnEdit(Developer.LanguageProvider.TextPosition start, Developer.LanguageProvider.TextPosition oldEnd, Developer.LanguageProvider.TextPosition newEnd)
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

        public virtual void Receive(NativeXAnalyzingResult result)
        {
            this.callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.analyzingResult = result;
                UpdateUnit(this.analyzingResult.Unit);
                UpdateBlock();
            }));
        }

        public virtual void Dispose()
        {
            this.analyzer.Dispose();
        }

        protected virtual void UpdateUnit(NativeXUnit unit)
        {
        }

        protected virtual void UpdateBlock()
        {
            NativeXAnalyzingResult result = this.analyzingResult;
            TextPosition pos = this.callback.TextEditorBox.SelectionCaret;
            if (result != null)
            {
                foreach (CodeToken token in result.Tokens)
                {
                    if (token.Start < pos && pos < token.End)
                    {
                        this.grayStart = token.Start;
                        this.grayEnd = token.End;
                        return;
                    }
                }
                this.grayStart = new TextPosition(0, 0);
                this.grayEnd = new TextPosition(0, 0);
            }
        }

        private void TextEditorBox_SelectionChanged(object sender, EventArgs e)
        {
            UpdateBlock();
        }
    }
}
