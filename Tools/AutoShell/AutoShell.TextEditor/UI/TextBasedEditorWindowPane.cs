using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using AutoShell.Services;
using AutoShell.PublicShell;

namespace AutoShell.UI
{
    public abstract class TextBasedEditorWindowPane : EditorWindowPane, IGotoableWindow, IFindTarget
    {
        private class TextAction
        {
            public int Start { get; set; }
            public string OldText { get; set; }
            public string NewText { get; set; }

            public TextAction(int start, string oldText, string newText)
            {
                this.Start = start;
                this.OldText = oldText;
                this.NewText = newText;
            }
        }

        #region Undo Redo Recording

        private int observedStart = 0;
        private int observedLength = 0;
        private string observedString = null;
        private bool enableObserving = true;
        private bool enableObservingPrivate = true;
        private List<TextAction> textActions = new List<TextAction>();
        private int previousActionCount = 0;

        private void RecordAction(int start, string oldText, string newText)
        {
            if (oldText != newText)
            {
                if (this.previousActionCount != this.textActions.Count)
                {
                    this.textActions.RemoveRange(this.previousActionCount, this.textActions.Count - this.previousActionCount);
                }
                this.textActions.Add(new TextAction(start, oldText, newText));
                this.previousActionCount = this.textActions.Count;
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                uiShell.QueryUIStatus();
                UpdateFindResults(start, oldText.Length, newText.Length);
            }
        }

        private void RecognizeAction()
        {
            string newText = this.EditorControl.Text;
            int lengthDelta = newText.Length - this.observedString.Length;
            if (this.observedLength == 0)
            {
                if (newText.Length >= this.observedStart && newText.Substring(0, this.observedStart) == this.observedString.Substring(0, this.observedStart))
                {
                    if (newText.Length > this.observedString.Length)
                    {
                        //在光标右边插入
                        RecordAction(this.observedStart, "", newText.Substring(this.observedStart, lengthDelta));
                    }
                    else
                    {
                        //在光标右边删除
                        RecordAction(this.observedStart, this.observedString.Substring(this.observedStart, -lengthDelta), "");
                    }
                }
                else
                {
                    if (newText.Length > this.observedString.Length)
                    {
                        //在光标左边插入
                        RecordAction(this.observedStart, "", newText.Substring(this.observedStart, lengthDelta));
                    }
                    else
                    {
                        //在光标左边删除
                        int start = this.observedStart + lengthDelta;
                        RecordAction(start, this.observedString.Substring(start, -lengthDelta), "");
                    }
                }
            }
            else
            {
                //替换
                RecordAction(
                    this.observedStart,
                    this.observedString.Substring(this.observedStart, this.observedLength),
                    newText.Substring(this.observedStart, this.observedLength + lengthDelta)
                    );
            }
        }

        private void Observe()
        {
            if (this.enableObserving && this.enableObservingPrivate)
            {
                this.observedStart = this.EditorControl.SelectionStart;
                this.observedLength = this.EditorControl.SelectionLength;
                this.observedString = this.EditorControl.Text;
            }
        }

        private void ClearUndo()
        {
            this.textActions.Clear();
            this.previousActionCount = 0;
        }

        protected bool EnableObserving
        {
            get
            {
                return this.enableObserving;
            }
            set
            {
                this.enableObserving = value;
            }
        }

        private void EditorControl_KeyPress(object sender, KeyPressEventArgs e)
        {
            Observe();
        }

        private void EditorControl_KeyDown(object sender, KeyEventArgs e)
        {
            Observe();
        }

        private void EditorControl_MouseUp(object sender, MouseEventArgs e)
        {
            Observe();
        }

        private void EditorControl_MouseMove(object sender, MouseEventArgs e)
        {
            Observe();
        }

        private void EditorControl_MouseDown(object sender, MouseEventArgs e)
        {
            Observe();
        }

        private void EditorControl_TextChanged(object sender, EventArgs e)
        {
            Modified();
            if (this.enableObserving && this.enableObservingPrivate)
            {
                if (this.observedString == null)
                {
                    throw new Exception();
                }
                else
                {
                    RecognizeAction();
                    this.observedString = null;
                    Observe();
                }
            }
        }

        #endregion

        public abstract TextBoxBase EditorControl { get; }

        public TextBasedEditorWindowPane()
        {
        }

        protected override void OnInitialize()
        {
            base.OnInitialize();
            this.EditorControl.KeyDown += new KeyEventHandler(EditorControl_KeyDown);
            this.EditorControl.KeyPress += new KeyPressEventHandler(EditorControl_KeyPress);
            this.EditorControl.MouseDown += new MouseEventHandler(EditorControl_MouseDown);
            this.EditorControl.MouseMove += new MouseEventHandler(EditorControl_MouseMove);
            this.EditorControl.MouseUp += new MouseEventHandler(EditorControl_MouseUp);
            this.EditorControl.TextChanged += new EventHandler(EditorControl_TextChanged);
        }

        private void SelectText(int start, int length)
        {
            /*this.EditorControl.SelectionStart = start;
            this.EditorControl.SelectionLength = length;*/
            this.EditorControl.Select(start, length);
        }

        #region Editor Window

        protected override bool CanUndo
        {
            get
            {
                return this.previousActionCount > 0;
            }
        }

        protected override bool CanRedo
        {
            get
            {
                return this.textActions.Count - this.previousActionCount > 0;
            }
        }

        protected override bool CanCut
        {
            get
            {
                return true;
            }
        }

        protected override bool CanCopy
        {
            get
            {
                return true;
            }
        }

        protected override bool CanPaste
        {
            get
            {
                return Clipboard.ContainsText();
            }
        }

        protected override bool CanClearSelection
        {
            get
            {
                return true;
            }
        }

        protected override bool CanSelectAll
        {
            get
            {
                return true;
            }
        }

        protected override void Undo()
        {
            if (this.CanUndo)
            {
                this.enableObservingPrivate = false;
                TextAction action = this.textActions[--this.previousActionCount];
                SelectText(action.Start, action.NewText.Length);
                this.EditorControl.SelectedText = action.OldText;
                SelectText(action.Start, action.OldText.Length);
                this.EditorControl.ScrollToCaret();
                this.enableObservingPrivate = true;
                Observe();
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                uiShell.QueryUIStatus();
                UpdateFindResults(action.Start, action.NewText.Length, action.OldText.Length);
            }
        }

        protected override void Redo()
        {
            if (this.CanRedo)
            {
                this.enableObservingPrivate = false;
                TextAction action = this.textActions[this.previousActionCount++];
                SelectText(action.Start, action.OldText.Length);
                this.EditorControl.SelectedText = action.NewText;
                SelectText(action.Start, action.NewText.Length);
                this.EditorControl.ScrollToCaret();
                this.enableObservingPrivate = true;
                Observe();
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                uiShell.QueryUIStatus();
                UpdateFindResults(action.Start, action.OldText.Length, action.NewText.Length);
            }
        }

        protected override void Cut()
        {
            Observe();
            this.EditorControl.Cut();
        }

        protected override void Copy()
        {
            Observe();
            this.EditorControl.Copy();
        }

        protected override void Paste()
        {
            Observe();
            this.EditorControl.Paste();
        }

        protected override void ClearSelection()
        {
            Observe();
            SendMessage(this.EditorControl.Handle, WM_KEYDOWN, (int)Keys.Delete, IntPtr.Zero);
            SendMessage(this.EditorControl.Handle, WM_KEYUP, (int)Keys.Delete, IntPtr.Zero);
        }

        protected override void SelectAll()
        {
            this.EditorControl.SelectAll();
        }

        protected override void LoadFromFile(string fileName)
        {
            this.enableObservingPrivate = false;
            using (StreamReader reader = new StreamReader(fileName, Encoding.Default, true))
            {
                this.EditorControl.Text = reader.ReadToEnd();
            }
            this.enableObservingPrivate = true;
            ClearUndo();
        }

        protected override void SaveToFile(string fileName)
        {
            using (StreamWriter writer = new StreamWriter(fileName, false, Encoding.Default))
            {
                writer.Write(this.EditorControl.Text);
            }
        }

        #endregion

        public override Control ToolWindowControl
        {
            get
            {
                return this.EditorControl;
            }
        }

        [DllImport("user32")]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, IntPtr lParam);

        private const int WM_KEYDOWN = 0x0100;
        private const int WM_KEYUP = 0x0101;

        #region IGotoableWindow Members

        bool IGotoableWindow.CanGoto
        {
            get
            {
                return this.EditorControl.Lines.Length > 0;
            }
        }

        int IGotoableWindow.MaxGotoLines
        {
            get
            {
                return this.EditorControl.Lines.Length;
            }
        }

        int IGotoableWindow.CurrentLineNumber
        {
            get
            {
                return this.EditorControl.GetLineFromCharIndex(this.EditorControl.SelectionStart);
            }
        }

        void IGotoableWindow.Goto(int lineNumber)
        {
            this.EditorControl.Select(this.EditorControl.GetFirstCharIndexFromLine(lineNumber), 0);
            this.EditorControl.Focus();
        }

        #endregion

        #region IFindTarget Members

        private FindResult[] findResults = new FindResult[] { };

        private void UpdateFindResults(int start, int oldLength, int newLength)
        {
            int offset = newLength - oldLength;
            if (offset != 0)
            {
                foreach (FindResult findResult in this.findResults)
                {
                    if (findResult.Start >= start)
                    {
                        findResult.Start += offset;
                    }
                }
            }

            IFindService findService = this.Services.QueryService<IFindService>();
            findService.UpdateFindResults(this.findResults);
        }

        string IFindTarget.UniqueName
        {
            get
            {
                return this.CreateOrder.ToString() + "#" + (this as IEditorWindow).FileName;
            }
        }

        string IFindTarget.Name
        {
            get
            {
                return this.Caption;
            }
        }

        string IFindTarget.Text
        {
            get
            {
                return this.EditorControl.Text;
            }
        }

        bool IFindTarget.CanReplace
        {
            get
            {
                return true;
            }
        }

        bool IFindTarget.IsOpened
        {
            get
            {
                return true;
            }
        }

        int IFindTarget.Position
        {
            get
            {
                return this.EditorControl.SelectionStart + this.EditorControl.SelectionLength;
            }
        }

        void IFindTarget.Focus(FindResult result)
        {
            this.Open();
            int start = result.Start;
            int end = result.Start + result.Length;
            if (start < 0) start = 0; else if (start > this.EditorControl.Text.Length) start = this.EditorControl.Text.Length;
            if (end < 0) end = 0; else if (end > this.EditorControl.Text.Length) end = this.EditorControl.Text.Length;
            SelectText(start, end - start);
            this.EditorControl.ScrollToCaret();
        }

        void IFindTarget.Replace(FindResult result, string text)
        {
            this.Open();
            SelectText(result.Start, result.Length);
            Observe();
            this.EditorControl.SelectedText = text;
            SelectText(result.Start, text.Length);
            this.EditorControl.ScrollToCaret();
        }

        void IFindTarget.SetResultAndTrack(FindResult[] results)
        {
            this.findResults = results;
            string[] lines = this.EditorControl.Lines;
            foreach (FindResult findResult in this.findResults)
            {
                int line = this.EditorControl.GetLineFromCharIndex(findResult.Start);
                findResult.Line = lines[line];
            }
        }

        void IFindTarget.ReplaceTrackedResult(string[] texts)
        {
            string oldText = this.EditorControl.Text;
            string newText = string.Empty;
            int start = 0;
            int offset = 0;
            for (int i = 0; i < this.findResults.Length; i++)
            {
                FindResult result = this.findResults[i];
                if (result.Start > start)
                {
                    newText += oldText.Substring(start, result.Start - start);
                }
                newText += texts[i];
                start = result.Start + result.Length;

                result.Start += offset;
                offset += texts[i].Length - result.Length;
                result.Length = texts[i].Length;
            }
            if (oldText.Length > start)
            {
                newText += oldText.Substring(start);
            }
            this.EditorControl.SelectAll();
            Observe();
            this.EditorControl.Text = newText;

            IFindService findService = this.Services.QueryService<IFindService>();
            findService.UpdateFindResults(this.findResults);
        }

        #endregion
    }
}
