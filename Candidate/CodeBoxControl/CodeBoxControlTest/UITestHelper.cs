using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Threading;
using System.Windows.Forms;
using Microsoft.VisualStudio.TestTools.UITesting.WinControls;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;
using CodeBoxControl.Core;
using CodeBoxControlTest.TextEditorServiceReference;
using System.Windows;

namespace CodeBoxControlTest
{
    class CodeFormWindow : WinWindow
    {
        public TextEditorServiceClient Service { get; private set; }
        public ApplicationUnderTest Application { get; private set; }

        public WinWindow Editor { get; private set; }
        public WinScrollBar HScroll { get; private set; }
        public WinScrollBar VScroll { get; private set; }
        public WinWindow Host { get; private set; }

        public CodeFormWindow(string colorizer = "normal")
        {
            this.Application = ApplicationUnderTest.Launch("CodeForm.exe", "", colorizer);
            this.Service = new TextEditorServiceClient();
            this.Service.Open();

            this.SearchProperties[WinControl.PropertyNames.Name] = "CodeForm";
            this.Find();
            {
                WinWindow window = new WinWindow(this);
                window.SearchProperties[WinControl.PropertyNames.ControlName] = "textEditorBox";
                window.Find();

                this.Editor = new WinWindow(window);
                this.Editor.Find();
            }
            {
                WinWindow window = new WinWindow(this.Editor);
                window.SearchProperties[WinControl.PropertyNames.ControlName] = "scrollVertical";
                window.Find();

                this.HScroll = new WinScrollBar(window);
                this.HScroll.Find();
            }
            {
                WinWindow window = new WinWindow(this.Editor);
                window.SearchProperties[WinControl.PropertyNames.ControlName] = "scrollHorizontal";
                window.Find();

                this.VScroll = new WinScrollBar(window);
                this.VScroll.Find();
            }
            {
                WinWindow window = new WinWindow(this.Editor);
                window.SearchProperties[WinControl.PropertyNames.ControlName] = "panelContent";
                window.Find();

                this.Host = new WinWindow(window);
                this.Host.Find();
            }
        }

        public void Close()
        {
            this.Service.Close();
            this.Application.Close();
        }

        #region Accessibility Functions

        public void Type(string text)
        {
            text = text.Select(c =>
                c == '{' ? "{{}" :
                c == '}' ? "{}}" :
                c == '.' ? "{.}" :
                c.ToString()
                ).Aggregate((a, b) => (a + b));
            Keyboard.SendKeys(this.Host, text);
        }

        public void TypeAndEnter(string text)
        {
            if (text != "")
            {
                Type(text);
            }
            Press("{ENTER}", ModifierKeys.None);
        }

        public void Press(string keys, ModifierKeys modifiers)
        {
            Keyboard.SendKeys(this.Host, keys, modifiers);
        }

        public void Press(string keys, bool control, bool shift)
        {
            ModifierKeys modifiers = ModifierKeys.None;
            if (control) modifiers |= ModifierKeys.Control;
            if (shift) modifiers |= ModifierKeys.Shift;
            Press(keys, modifiers);
        }

        #endregion

        #region Output

        private System.Drawing.Point ToPoint(TextPosition p)
        {
            return new System.Drawing.Point(p.col, p.row);
        }

        private TextPosition ToPosition(System.Drawing.Point p)
        {
            return new TextPosition(p.Y, p.X);
        }

        public TextPosition SelectionAnchor
        {
            get
            {
                return ToPosition(this.Service.GetSelectionAnchor());
            }
        }

        public TextPosition SelectionCaret
        {
            get
            {
                return ToPosition(this.Service.GetSelectionCaret());
            }
        }

        public TextPosition SelectionStart
        {
            get
            {
                return ToPosition(this.Service.GetSelectionStart());
            }
        }

        public TextPosition SelectionEnd
        {
            get
            {
                return ToPosition(this.Service.GetSelectionEnd());
            }
        }

        public int LineCount
        {
            get
            {
                return this.Service.GetLineCount();
            }
        }

        public int GetLineLength(int index)
        {
            return this.Service.GetLineLength(index);
        }

        public string GetString(TextPosition p1, TextPosition p2)
        {
            return this.Service.GetString(ToPoint(p1), ToPoint(p2));
        }

        #endregion

        #region Input

        public void Input(string text)
        {
            string[] lines = text.Split(new string[] { "\r\n" }, StringSplitOptions.None);
            for (int i = 0; i < lines.Length; i++)
            {
                if (i < lines.Length - 1)
                {
                    TypeAndEnter(lines[i]);
                }
                else
                {
                    Type(lines[i]);
                }
            }
        }

        public void Move(TextPosition caret, bool control, bool shift)
        {
            PressHome(true, shift);
            for (int i = 0; i < caret.row; i++)
            {
                PressDown(false, shift);
            }
            for (int i = 0; i < caret.col; i++)
            {
                PressRight(false, shift);
            }
        }

        public void PressUp(bool control, bool shift)
        {
            Press("{UP}", control, shift);
        }

        public void PressDown(bool control, bool shift)
        {
            Press("{DOWN}", control, shift);
        }

        public void PressLeft(bool control, bool shift)
        {
            Press("{LEFT}", control, shift);
        }

        public void PressRight(bool control, bool shift)
        {
            Press("{RIGHT}", control, shift);
        }

        public void PressHome(bool control, bool shift)
        {
            Press("{HOME}", control, shift);
        }

        public void PressEnd(bool control, bool shift)
        {
            Press("{END}", control, shift);
        }

        public void PressPageUp(bool control, bool shift)
        {
            Press("{PAGEUP}", control, shift);
        }

        public void PressPageDown(bool control, bool shift)
        {
            Press("{PAGEDOWN}", control, shift);
        }

        public void PressDelete(bool control, bool shift)
        {
            Press("{DELETE}", control, shift);
        }

        public void PressBackspace(bool control, bool shift)
        {
            Press("{BACK}", control, shift);
        }

        public void PressEnter(bool control, bool shift)
        {
            Press("{ENTER}", control, shift);
        }

        #endregion
    }
}
