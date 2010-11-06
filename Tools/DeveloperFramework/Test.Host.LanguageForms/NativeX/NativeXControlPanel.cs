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
        private NativeXForm form = null;

        private NativeXStatement editingStatement = null;
        private TextProvider<object> editingStatementCode = null;
        private TextPosition grayStart = new TextPosition(0, 0);
        private TextPosition grayEnd = new TextPosition(0, 0);
        private int counter = 0;
        private int analyzingId = 0;
        private int receivedId = 0;

        public NativeXControlPanel(NativeXForm form)
        {
            this.form = form;
            this.editingStatementCode = new TextProvider<object>();
        }

        public void Dispose()
        {
            this.analyzer.Dispose();
            this.editingStatementCode.Dispose();
            this.editingStatementCode = null;
        }

        #region ITextEditorControlPanel Members

        public int Width
        {
            get
            {
                return 0;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.analyzer = new NativeXCodeAnalyzer();
            this.analyzer.Received += new CalculationNotifierReceivedHandler<NativeXAnalyzingResult>(Analyzer_Received);
            this.callback = callback;
            this.callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            this.analyzingId = this.analyzer.Analyze(this.callback.TextEditorBox.Text);
            if (this.editingStatement != null)
            {
                TextPosition editingStart = ConvertToEditingPosition(start);
                TextPosition editingOldEnd = ConvertToEditingPosition(oldEnd);
                TextPosition editingNewEnd = ConvertToEditingPosition(newEnd);
                if (this.editingStatementCode.Contains(editingStart) && this.editingStatementCode.Contains(editingOldEnd))
                {
                    string editingText = this.callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                    this.editingStatementCode.Edit(editingStart, editingOldEnd, editingText);

                    List<CodeToken> tokens = NativeXCodeParser.Tokenize(this.editingStatementCode.Text.ToCharArray());
                    int currentToken = 0;
                    bool parseSuccess = false;
                    NativeXEditingStatement editingCode = NativeXCodeParser.ParseEditingStatement(tokens, ref currentToken, ref parseSuccess);
                    if (editingCode != null)
                    {
                        this.editingStatement = editingCode.FindDeepest<NativeXStatement>(editingNewEnd);
                    }
                    else
                    {
                        this.editingStatement = null;
                    }
                }
                else
                {
                    this.editingStatement = null;
                    this.editingStatementCode.Text = "";
                }
                UpdateContextText();
            }
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

        #endregion

        #region Reaction Functions

        private void Analyzer_Received(NativeXAnalyzingResult result, int id)
        {
            NativeXStatement statement = null;
            FindBlock(result, out statement);

            string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
            string statusText = (++this.counter).ToString();

            this.callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.receivedId = id;
                this.analyzingResult = result;

                this.form.TreeText = treeText;
                this.form.StatusText = statusText;
                UpdateBlock(statement);

                this.callback.TextEditorBox.Refresh();
            }));
        }

        private TextPosition ConvertToEditingPosition(TextPosition pos)
        {
            if (pos.row == this.grayStart.row)
            {
                pos.col -= this.grayStart.col;
            }
            pos.row -= this.grayStart.row;
            return pos;
        }

        private void FindBlock(NativeXAnalyzingResult result, out NativeXStatement statement)
        {
            TextPosition pos = this.callback.TextEditorBox.SelectionCaret;
            statement = null;

            if (result != null && result.Unit != null)
            {
                statement = result.Unit.FindDeepest<NativeXStatement>(pos);
            }
        }

        private void UpdateContextText()
        {
            this.form.ContextText = this.editingStatementCode.Text
                + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                + (this.editingStatement == null ? "<NULL>" : this.editingStatement.ToString());
        }

        private void UpdateBlock(NativeXStatement statement)
        {
            if (this.analyzingId == this.receivedId)
            {
                this.editingStatement = statement;
                if (statement == null)
                {
                    this.grayStart = new TextPosition(0, 0);
                    this.grayEnd = new TextPosition(0, 0);
                    this.editingStatementCode.Text = "";
                }
                else
                {
                    this.grayStart = statement.Start;
                    this.grayEnd = statement.End;
                    this.editingStatementCode.Text = this.callback.TextEditorBox.TextProvider.GetString(this.grayStart, this.grayEnd);
                }
                UpdateContextText();
            }
        }

        private void TextEditorBox_SelectionChanged(object sender, EventArgs e)
        {
            NativeXStatement statement = null;
            FindBlock(this.analyzingResult, out statement);
            UpdateBlock(statement);
        }

        #endregion
    }
}
