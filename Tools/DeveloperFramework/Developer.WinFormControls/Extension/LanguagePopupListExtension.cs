using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;
using Developer.WinFormControls.Core;
using System.Windows.Forms;

namespace Developer.WinFormControls.Extension
{
    public class LanguagePopupListExtension : ITextEditorBoxPopupListExtension
    {
        private bool forcePopupList = false;

        public ITextEditorControlPanelCallBack Callback { get; private set; }

        public bool PreventPopupList
        {
            get
            {
                return !this.forcePopupList && !this.Callback.TextEditorBox.PressingChar;
            }
        }

        public virtual bool IsPopupListKeyAcceptable(System.Windows.Forms.KeyEventArgs e)
        {
            if (e.Control || e.Alt)
            {
                return false;
            }
            else if (Keys.A <= e.KeyCode && e.KeyCode <= Keys.Z)
            {
                return true;
            }
            else if (Keys.NumPad0 <= e.KeyCode && e.KeyCode <= Keys.NumPad9)
            {
                return true;
            }
            else if (Keys.D0 <= e.KeyCode && e.KeyCode <= Keys.D9)
            {
                return !e.Shift;
            }
            else if (e.KeyCode == Keys.OemMinus && e.Shift)
            {
                return true;
            }
            else
            {
                switch (e.KeyCode)
                {
                    case Keys.Back: return true;
                    case Keys.Delete: return true;
                    case Keys.ShiftKey: return true;
                    case Keys.CapsLock: return true;
                    default: return false;
                }
            }
        }

        public virtual bool IsPopupListCharAcceptable(char c)
        {
            return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_';
        }

        public virtual bool IsPopupListCloseChar(char c)
        {
            return false;
        }

        public virtual void PopupListItemSelected(string searchingKey, string text)
        {
            TextPosition end = this.Callback.TextEditorBox.SelectionCaret;
            TextPosition start = new TextPosition(end.row, end.col - searchingKey.Length);
            TextEditorController controller = this.Callback.TextEditorBox.Controller;
            controller.StartTask();
            controller.Move(start, false, false);
            controller.Move(end, false, true);
            controller.Input(text, false);
            controller.FinishTask();
        }

        public virtual void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
