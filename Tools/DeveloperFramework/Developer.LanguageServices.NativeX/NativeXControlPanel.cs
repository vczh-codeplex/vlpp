using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Drawing;

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

        public int Width
        {
            get
            {
                return 0;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.analyzer = new NativeXCodeAnalyzer(this);
            this.callback = callback;
            this.callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public void OnEdit(Developer.LanguageProvider.TextPosition start, Developer.LanguageProvider.TextPosition oldEnd, Developer.LanguageProvider.TextPosition newEnd)
        {
            this.analyzer.Analyze(this.callback.TextEditorBox.Text);
        }

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            return this.grayStart != this.grayEnd && this.grayStart.row <= lineIndex && lineIndex <= this.grayEnd.row;
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
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

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
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

        public void Receive(NativeXAnalyzingResult result)
        {
            this.callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.analyzingResult = result;
                UpdateBlock();
            }));
        }

        public void Dispose()
        {
            this.analyzer.Dispose();
        }

        private void UpdateBlock()
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
