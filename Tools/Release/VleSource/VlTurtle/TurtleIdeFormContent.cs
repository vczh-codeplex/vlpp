using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageProvider;
using VlTurtle.EditorControls;

namespace VlTurtle
{
    public partial class TurtleIdeFormContent : UserControl
    {
        private ClipboardMonitor clipboardMonitor = null;

        public TurtleIdeFormContent()
        {
            InitializeComponent();
            if (this.components == null)
            {
                this.components = new Container();
            }
            this.clipboardMonitor = new ClipboardMonitor(this);
            this.clipboardMonitor.ClipboardChanged += new EventHandler(clipboardMonitor_ClipboardChanged);
            this.components.Add(this.clipboardMonitor);
        }

        private void clipboardMonitor_ClipboardChanged(object sender, EventArgs e)
        {
            InvokeButtonStateUpdated();
        }

        private void codeEditorNativeX_SelectionChanged(object sender, EventArgs e)
        {
            InvokeButtonStateUpdated();
        }

        protected override void WndProc(ref Message m)
        {
            if (this.clipboardMonitor != null)
            {
                this.clipboardMonitor.WndProc(ref m);
            }
            base.WndProc(ref m);
        }

        private void InvokeButtonStateUpdated()
        {
            if (this.ButtonStateUpdated != null)
            {
                this.ButtonStateUpdated(this, new EventArgs());
            }
        }

        public void LoadDefaultCode()
        {
            this.codeEditorNativeX.Text =
                "unit MyTurtleProgram;\r\n\r\n" +
                "uses syscrnat, Turtle;\r\n\r\n" +
                "function void main()\r\n" +
                "{\r\n" +
                "\t\r\n" +
                "}\r\n";
            this.codeEditorNativeX.ClearUndoRedoHistory();
            this.codeEditorNativeX.Controller.Select(new TextPosition(6, 1), new TextPosition(6, 1));
        }

        public override string Text
        {
            get
            {
                return this.codeEditorNativeX.Text;
            }
            set
            {
                this.codeEditorNativeX.Text = value;
            }
        }

        #region Operations

        public event EventHandler ButtonStateUpdated;

        public void OperationNew()
        {
        }

        public void OperationOpen()
        {
        }

        public void OperationSave()
        {
        }

        public void OperationSaveAs()
        {
        }

        public void OperationCut()
        {
            this.codeEditorNativeX.Cut();
            InvokeButtonStateUpdated();
        }

        public void OperationCopy()
        {
            this.codeEditorNativeX.Copy();
            InvokeButtonStateUpdated();
        }

        public void OperationPaste()
        {
            this.codeEditorNativeX.Paste();
            InvokeButtonStateUpdated();
        }

        public void OperationRedo()
        {
            this.codeEditorNativeX.Redo();
            InvokeButtonStateUpdated();
        }

        public void OperationUndo()
        {
            this.codeEditorNativeX.Undo();
            InvokeButtonStateUpdated();
        }

        public void OperationRun()
        {
        }

        public void OperationStop()
        {
        }

        #endregion

        #region Operation States

        public bool AvailableCut()
        {
            return codeEditorNativeX.CanCut;
        }

        public bool AvailableCopy()
        {
            return codeEditorNativeX.CanCopy;
        }

        public bool AvailablePaste()
        {
            return codeEditorNativeX.CanPaste;
        }

        public bool AvailableRedo()
        {
            return codeEditorNativeX.CanRedo;
        }

        public bool AvailableUndo()
        {
            return codeEditorNativeX.CanUndo;
        }

        #endregion
    }
}
