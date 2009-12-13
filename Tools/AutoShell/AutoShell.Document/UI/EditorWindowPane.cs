using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using AutoShell.Services;
using System.Windows.Forms;

namespace AutoShell.UI
{
    public abstract class EditorWindowPane : ToolWindowPane, IEditorWindow
    {
        private string fileName = "";
        private Guid fileType = Guid.Empty;
        private bool modified = false;

        public EditorWindowPane()
        {
            this.EnableModifyDockType = false;
        }

        public override Size DefaultSize
        {
            get
            {
                return new Size(300, 200);
            }
        }

        protected override void OnInitialize()
        {
        }

        protected override void OnOpening()
        {
        }

        protected override void OnClosing(ref bool cancel)
        {
            cancel = false;
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            if (this.modified)
            {
                string caption = null;
                if (string.IsNullOrEmpty(this.fileName))
                {
                    caption = editorService.GetFileType(this.fileType).FileTypeName;
                }
                else if (this.modified)
                {
                    caption = Path.GetFileName(this.fileName);
                }
                switch (uiShell.ShowMessage(string.Format(Strings.MessageBoxSaveModificationTo, caption), MessageBoxButtons.YesNoCancel))
                {
                    case DialogResult.Yes:
                        cancel = !editorService.SaveEditingContent(this, false);
                        break;
                    case DialogResult.No:
                        break;
                    case DialogResult.Cancel:
                        cancel = true;
                        break;
                }
            }
        }

        protected override void OnClosed()
        {
        }

        protected override void OnQueryStatus()
        {
        }

        protected void UpdateCaption()
        {
            if (string.IsNullOrEmpty(this.fileName))
            {
                IEditorService editorService = this.Services.QueryService<IEditorService>();
                FileType fileType = editorService.GetFileType(this.fileType);
                this.Caption = string.Format(Strings.ToolWindowUnsavedDocument, fileType.FileTypeName);
            }
            else if (this.modified)
            {
                this.Caption = Path.GetFileName(this.fileName) + "*";
            }
            else
            {
                this.Caption = Path.GetFileName(this.fileName);
            }
        }

        protected void Modified()
        {
            this.modified = true;
            UpdateCaption();
        }

        protected abstract bool CanUndo { get; }
        protected abstract bool CanRedo { get; }
        protected abstract bool CanCut { get; }
        protected abstract bool CanCopy { get; }
        protected abstract bool CanPaste { get; }
        protected abstract bool CanClearSelection { get; }
        protected abstract bool CanSelectAll { get; }
        protected abstract void Undo();
        protected abstract void Redo();
        protected abstract void Cut();
        protected abstract void Copy();
        protected abstract void Paste();
        protected abstract void ClearSelection();
        protected abstract void SelectAll();
        protected abstract void LoadFromFile(string fileName);
        protected abstract void SaveToFile(string fileName);

        #region IEditorWindow Members

        Guid IEditorWindow.FileType
        {
            get
            {
                return this.fileType;
            }
        }

        string IEditorWindow.FileName
        {
            get
            {
                return this.fileName;
            }
        }

        bool IEditorWindow.IsFileBased
        {
            get
            {
                return true;
            }
        }

        bool IEditorWindow.IsModified
        {
            get
            {
                return this.modified;
            }
        }

        bool IEditorWindow.CanUndo
        {
            get
            {
                return this.CanUndo;
            }
        }

        bool IEditorWindow.CanRedo
        {
            get
            {
                return this.CanRedo;
            }
        }

        bool IEditorWindow.CanCut
        {
            get
            {
                return this.CanCut;
            }
        }

        bool IEditorWindow.CanCopy
        {
            get
            {
                return this.CanCopy;
            }
        }

        bool IEditorWindow.CanPaste
        {
            get
            {
                return this.CanPaste;
            }
        }

        bool IEditorWindow.CanClearSelection
        {
            get
            {
                return this.CanClearSelection;
            }
        }

        bool IEditorWindow.CanSelectAll
        {
            get
            {
                return this.CanSelectAll;
            }
        }

        void IEditorWindow.Initialize(Guid fileType)
        {
            this.fileType = fileType;
            UpdateCaption();
        }

        void IEditorWindow.LoadFromFile(string fileName)
        {
            LoadFromFile(fileName);
            this.modified = false;
            UpdateCaption();
        }

        void IEditorWindow.SaveToFile(string fileName)
        {
            SaveToFile(fileName);
            this.modified = false;
            UpdateCaption();
        }

        void IEditorWindow.AssociateToFile(string fileName)
        {
            this.fileName = fileName;
            UpdateCaption();
        }

        void IEditorWindow.Undo()
        {
            Undo();
        }

        void IEditorWindow.Redo()
        {
            Redo();
        }

        void IEditorWindow.Cut()
        {
            Cut();
        }

        void IEditorWindow.Copy()
        {
            Copy();
        }

        void IEditorWindow.Paste()
        {
            Paste();
        }

        void IEditorWindow.ClearSelection()
        {
            ClearSelection();
        }

        void IEditorWindow.SelectAll()
        {
            SelectAll();
        }

        #endregion
    }
}
