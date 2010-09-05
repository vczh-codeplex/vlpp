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

namespace CodeBoxControlTest
{
    class CodeFormWindow : WinWindow
    {
        public WinWindow Editor { get; set; }
        public WinScrollBar HScroll { get; set; }
        public WinScrollBar VScroll { get; set; }
        public WinWindow Host { get; set; }

        public CodeFormWindow()
        {
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

        public void Wait(int ms)
        {
            Thread.Sleep(ms);
        }

        public void Type(string text)
        {
            text = text.Select(c => c == '{' ? "{{}" : c == '}' ? "{}}" : c.ToString()).Aggregate((a, b) => (a + b));
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
    }
}
