using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Threading;
using System.Windows.Forms;
using Microsoft.VisualStudio.TestTools.UITesting.WinControls;

namespace CodeBoxControlTest
{
    static class UITestHelper
    {
        public static TextEditorBox OpenEditor(bool loadFile)
        {
            CodeForm.CodeForm form = null;
            new Thread(new ThreadStart(() =>
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                form = new CodeForm.CodeForm(loadFile);
                Application.Run(form);
            })).Start();
            while (form == null) Thread.Sleep(10);
            return form.textEditorBox;
        }

        public static TextEditorBox OpenEditor()
        {
            return OpenEditor(false);
        }

        public static void CloseEditor(TextEditorBox editor)
        {
            editor.Invoke(new MethodInvoker(() =>
            {
                editor.FindForm().Close();
            }));
        }
    }

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
    }
}
