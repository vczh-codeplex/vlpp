using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageProvider;

namespace VlTurtle
{
    public partial class TurtleIdeFormContent : UserControl
    {
        public TurtleIdeFormContent()
        {
            InitializeComponent();
        }

        private void codeEditorNativeX_SelectionChanged(object sender, EventArgs e)
        {
            InvokeButtonStateUpdated();
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
