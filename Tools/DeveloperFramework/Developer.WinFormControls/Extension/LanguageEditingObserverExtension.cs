using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;
using Developer.WinFormControls.Core;
using System.Windows.Forms;

namespace Developer.WinFormControls.Extension
{
    public abstract class LanguageEditingObserverExtension<TResult, TEditingNode, TEditingNodeContainer> : ITextEditorBoxEditingObserverExtension, IDisposable
        where TResult : class
        where TEditingNode : CodeNode
        where TEditingNodeContainer : CodeNode
    {
        protected CalculationNotifier<string, TResult> Analyzer { get; private set; }
        protected ILanguageEditingObserverProvider<TResult, TEditingNode, TEditingNodeContainer> Provider { get; private set; }
        protected int AnalyzingId { get; private set; }
        protected int ReceivedId { get; private set; }

        public ITextEditorControlPanelCallBack Callback { get; private set; }
        public TResult AnalyzingResult { get; private set; }
        public TEditingNode EditingNode { get; private set; }
        public CodeScope EditingNodeScope { get; private set; }
        public TextProvider<object> EditingNodeCode { get; private set; }
        public TextPosition EditingNodeStart { get; private set; }
        public TextPosition EditingNodeEnd { get; private set; }
        public bool IsTemporaryEditingNode { get; private set; }

        public LanguageEditingObserverExtension(ILanguageEditingObserverProvider<TResult, TEditingNode, TEditingNodeContainer> provider)
        {
            this.EditingNodeCode = new TextProvider<object>();
            this.Provider = provider;
            this.Provider.Attach(this);
        }

        public virtual void OnBeforeEdit(LanguageProvider.TextPosition start, LanguageProvider.TextPosition end, ref string[] lines)
        {
        }

        public virtual void OnAfterEdit(LanguageProvider.TextPosition start, LanguageProvider.TextPosition oldEnd, LanguageProvider.TextPosition newEnd)
        {
            this.AnalyzingId = this.Analyzer.Analyze(this.Callback.TextEditorBox.Text);
            if (this.EditingNode != null)
            {
                this.IsTemporaryEditingNode = true;
                TextPosition editingStart = ConvertToEditingPosition(start);
                TextPosition editingOldEnd = ConvertToEditingPosition(oldEnd);
                TextPosition editingNewEnd = ConvertToEditingPosition(newEnd);
                if (this.EditingNodeCode.Contains(editingStart) && this.EditingNodeCode.Contains(editingOldEnd))
                {
                    string editingText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                    this.EditingNodeCode.Edit(editingStart, editingOldEnd, editingText);

                    TEditingNodeContainer editingNodeContainer = this.Provider.ParseEditingNodeContainer(this.EditingNodeCode.Text);
                    if (editingNodeContainer != null)
                    {
                        editingNodeContainer.BuildScope(this.EditingNodeScope);
                        this.EditingNode = this.Provider.GetEditingNode(editingNodeContainer, editingNewEnd);
                    }
                    else
                    {
                        this.EditingNode = null;
                    }
                }
                else
                {
                    this.EditingNode = null;
                    this.EditingNodeCode.Text = "";
                }
                this.Provider.TemporaryResultUpdated();
            }
        }

        public virtual void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Analyzer = this.Provider.CreateAnalyzer();
            this.Analyzer.Received += new CalculationNotifierReceivedHandler<TResult>(AnalyzerReceived);
            this.Callback = callback;
            this.Callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public virtual void Dispose()
        {
            this.Analyzer.Dispose();
            this.EditingNodeCode.Dispose();
            this.EditingNodeCode = null;
        }

        #region Reaction Functions

        public TextPosition ConvertToEditingPosition(TextPosition pos, bool editingNodeCodePosition = false)
        {
            if (this.IsTemporaryEditingNode || editingNodeCodePosition)
            {
                if (pos.row == this.EditingNodeStart.row)
                {
                    pos.col -= this.EditingNodeStart.col;
                }
                pos.row -= this.EditingNodeStart.row;
            }
            return pos;
        }

        public TextPosition ConvertToCodePosition(TextPosition pos)
        {
            if (pos.row == 0)
            {
                return new TextPosition(this.EditingNodeStart.row, this.EditingNodeStart.col + pos.col);
            }
            else
            {
                return new TextPosition(this.EditingNodeStart.row + pos.row, pos.col);
            }
        }

        protected void AnalyzerReceived(TResult result, int id)
        {
            this.Provider.AnalyzerResultReceived(result);
            TEditingNode editingNode = this.Provider.GetEditingNode(result, this.Callback.TextEditorBox.SelectionCaret);

            this.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.ReceivedId = id;
                this.AnalyzingResult = result;
                UpdateBlock(editingNode);
            }));
        }

        protected void UpdateBlock(TEditingNode editingNode)
        {
            if (this.AnalyzingId == this.ReceivedId)
            {
                this.EditingNode = editingNode;
                if (editingNode == null)
                {
                    this.EditingNodeStart = new TextPosition(0, 0);
                    this.EditingNodeEnd = new TextPosition(0, 0);
                    this.EditingNodeScope = null;
                    this.EditingNodeCode.Text = "";
                }
                else
                {
                    this.EditingNodeStart = editingNode.Start;
                    this.EditingNodeEnd = editingNode.End;
                    this.EditingNodeScope = editingNode.Scope;
                    if (editingNode.End == editingNode.Root.End)
                    {
                        int row = this.Callback.TextEditorBox.TextProvider.Count - 1;
                        int col = this.Callback.TextEditorBox.TextProvider[row].CharCount;
                        TextPosition end = new TextPosition(row, col);
                        this.EditingNodeCode.Text = this.Callback.TextEditorBox.TextProvider.GetString(this.EditingNodeStart, end);
                    }
                    else
                    {
                        this.EditingNodeCode.Text = this.Callback.TextEditorBox.TextProvider.GetString(this.EditingNodeStart, this.EditingNodeEnd);
                    }
                }
                this.Provider.ResultUpdated();
                this.IsTemporaryEditingNode = false;
            }
        }

        private void TextEditorBox_SelectionChanged(object sender, EventArgs e)
        {
            UpdateBlock(this.Provider.GetEditingNode(this.AnalyzingResult, this.Callback.TextEditorBox.SelectionCaret));
        }

        #endregion
    }

    public interface ILanguageEditingObserverProvider<TResult, TEditingNode, TEditingNodeContainer>
        where TResult : class
        where TEditingNode : CodeNode
        where TEditingNodeContainer : CodeNode
    {
        void Attach(LanguageEditingObserverExtension<TResult, TEditingNode, TEditingNodeContainer> extension);
        CalculationNotifier<string, TResult> CreateAnalyzer();
        TEditingNodeContainer ParseEditingNodeContainer(string code);
        TEditingNode GetEditingNode(TResult result, TextPosition pos);
        TEditingNode GetEditingNode(TEditingNodeContainer container, TextPosition pos);
        void ResultUpdated();
        void TemporaryResultUpdated();
        void AnalyzerResultReceived(TResult result);
    }
}
