﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.WinFormControls.Core;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.WinFormControls
{
    abstract class LanguageControlPanel<TResult, TEditingNode, TEditingNodeContainer> : ITextEditorControlPanel, IDisposable
        where TResult : class
        where TEditingNode : CodeNode
        where TEditingNodeContainer : CodeNode
    {
        protected ITextEditorControlPanelCallBack Callback { get; private set; }
        protected TResult AnalyzingResult { get; private set; }
        protected CalculationNotifier<string, TResult> Analyzer { get; private set; }

        protected TEditingNode EditingNode { get; private set; }
        protected TextProvider<object> EditingNodeCode { get; private set; }
        protected TextPosition EditingNodeStart { get; private set; }
        protected TextPosition EditingNodeEnd { get; private set; }
        protected int AnalyzingId { get; private set; }
        protected int ReceivedId { get; private set; }

        public LanguageControlPanel()
        {
            this.EditingNodeCode = new TextProvider<object>();
        }

        public virtual void Dispose()
        {
            this.Analyzer.Dispose();
            this.EditingNodeCode.Dispose();
            this.EditingNodeCode = null;
        }

        #region Language Callbacks

        protected abstract CalculationNotifier<string, TResult> CreateAnalyzer();
        protected abstract TEditingNodeContainer ParseEditingNodeContainer(string code);
        protected abstract TEditingNode GetEditingNode(TResult result, TextPosition pos);
        protected abstract void ResultUpdated();
        protected abstract void TemporaryResultUpdated();
        protected abstract void AnalyzerResultReceived(TResult result);

        #endregion

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
            this.Analyzer = CreateAnalyzer();
            this.Analyzer.Received += new CalculationNotifierReceivedHandler<TResult>(AnalyzerReceived);
            this.Callback = callback;
            this.Callback.TextEditorBox.SelectionChanged += new EventHandler(TextEditorBox_SelectionChanged);
        }

        public virtual void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public virtual void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            this.AnalyzingId = this.Analyzer.Analyze(this.Callback.TextEditorBox.Text);
            if (this.EditingNode != null)
            {
                TextPosition editingStart = ConvertToEditingPosition(start);
                TextPosition editingOldEnd = ConvertToEditingPosition(oldEnd);
                TextPosition editingNewEnd = ConvertToEditingPosition(newEnd);
                if (this.EditingNodeCode.Contains(editingStart) && this.EditingNodeCode.Contains(editingOldEnd))
                {
                    string editingText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                    this.EditingNodeCode.Edit(editingStart, editingOldEnd, editingText);

                    TEditingNodeContainer editingNodeContainer = ParseEditingNodeContainer(this.EditingNodeCode.Text);
                    if (editingNodeContainer != null)
                    {
                        editingNodeContainer.BuildScope(null);
                        this.EditingNode = editingNodeContainer.FindDeepest<TEditingNode>(editingNewEnd);
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
                TemporaryResultUpdated();
            }
        }

        public virtual bool NeedColorLineForDisplay(int lineIndex)
        {
            return false;
        }

        public virtual void ColorLineForDisplay(int lineIndex, int[] colors)
        {
        }

        public virtual void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
        }

        public virtual void DrawLineForeground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
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

        protected void AnalyzerReceived(TResult result, int id)
        {
            AnalyzerResultReceived(result);
            TEditingNode editingNode = GetEditingNode(result, this.Callback.TextEditorBox.SelectionCaret);

            this.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.ReceivedId = id;
                this.AnalyzingResult = result;
                UpdateBlock(editingNode);
            }));
        }

        protected TextPosition ConvertToEditingPosition(TextPosition pos)
        {
            if (pos.row == this.EditingNodeStart.row)
            {
                pos.col -= this.EditingNodeStart.col;
            }
            pos.row -= this.EditingNodeStart.row;
            return pos;
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
                    this.EditingNodeCode.Text = "";
                }
                else
                {
                    this.EditingNodeStart = editingNode.Start;
                    this.EditingNodeEnd = editingNode.End;
                    this.EditingNodeCode.Text = this.Callback.TextEditorBox.TextProvider.GetString(this.EditingNodeStart, this.EditingNodeEnd);
                }
                ResultUpdated();
            }
        }

        private void TextEditorBox_SelectionChanged(object sender, EventArgs e)
        {
            UpdateBlock(GetEditingNode(this.AnalyzingResult, this.Callback.TextEditorBox.SelectionCaret));
        }

        #endregion
    }
}
