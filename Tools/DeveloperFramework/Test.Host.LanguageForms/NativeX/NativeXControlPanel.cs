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
    class NativeXControlPanel : LanguageControlPanel<NativeXAnalyzingResult, NativeXStatement, NativeXEditingStatement>
    {
        private NativeXForm form = null;
        private int counter = 0;

        public NativeXControlPanel(NativeXForm form)
        {
            this.form = form;
        }

        public override bool NeedColorLineForDisplay(int lineIndex)
        {
            return this.EditingNodeStart != this.EditingNodeEnd && this.EditingNodeStart.row <= lineIndex && lineIndex <= this.EditingNodeEnd.row;
        }

        public override void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            base.ColorLineForDisplay(lineIndex, colors);
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            int length = line.CharCount;

            int start = EditingNodeStart.row == lineIndex ? Math.Min(EditingNodeStart.col, length) : 0;
            int end = EditingNodeEnd.row == lineIndex ? Math.Min(EditingNodeEnd.col, length) : line.CharCount;
            for (int i = start; i < end; i++)
            {
                colors[i] = NativeXColorizer.BlockPointColorId;
            }
        }

        public override void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            base.DrawLineForeground(g, lineIndex, backgroundArea);
            if (NeedColorLineForDisplay(lineIndex))
            {
                TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
                int length = line.CharCount;
                int start = EditingNodeStart.row == lineIndex ? Math.Min(EditingNodeStart.col, length) : 0;
                int end = EditingNodeEnd.row == lineIndex ? Math.Min(EditingNodeEnd.col, length) : line.CharCount;

                int x1 = this.Callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, start)).X;
                int x2 = this.Callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, end)).X;
                g.DrawRectangle(Pens.Gray, x1, backgroundArea.Top, x2 - x1, backgroundArea.Height);
            }
        }

        protected override CalculationNotifier<string, NativeXAnalyzingResult> CreateAnalyzer()
        {
            return new NativeXCodeAnalyzer();
        }

        protected override NativeXEditingStatement ParseEditingNodeContainer(string code)
        {
            List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            return NativeXCodeParser.ParseEditingStatement(tokens, ref currentToken, ref parseSuccess);
        }

        protected override NativeXStatement GetEditingNode(NativeXAnalyzingResult result, TextPosition pos)
        {
            if (result != null && result.Unit != null)
            {
                return result.Unit.FindDeepest<NativeXStatement>(pos);
            }
            else
            {
                return null;
            }
        }

        protected override void ResultUpdated()
        {
            TemporaryResultUpdated();
            this.Callback.TextEditorBox.Refresh();
        }

        protected override void TemporaryResultUpdated()
        {
            this.form.ContextText = this.EditingNodeCode.Text
                + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                + (this.EditingNode == null ? "<NULL>" : this.EditingNode.ToString());
        }

        protected override void AnalyzerResultReceived(NativeXAnalyzingResult result)
        {
            string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
            string statusText = (++this.counter).ToString();

            this.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.form.TreeText = treeText;
                this.form.StatusText = statusText;
            }));
        }
    }
}
