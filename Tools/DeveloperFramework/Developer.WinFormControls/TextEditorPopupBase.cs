using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Developer.WinFormControls
{
    public partial class TextEditorPopupBase : Form
    {
        public TextEditorPopupBase()
        {
            InitializeComponent();
        }

        public void Show(Control control, Point locationTop, Point locationBottom)
        {
            Point location;
            Size size = this.Size;
            Screen screen = Screen.FromControl(control);
            if (locationBottom.Y + size.Height < screen.WorkingArea.Bottom)
            {
                location = locationBottom;
            }
            else if (locationTop.Y - size.Height >= screen.WorkingArea.Top)
            {
                location = new Point(locationTop.X, locationTop.Y - size.Height);
            }
            else
            {
                location = new Point(locationTop.X, 0);
            }
            if (location.X + size.Width > screen.WorkingArea.Right)
            {
                if (location.X - size.Width >= screen.WorkingArea.Left)
                {
                    location.X -= size.Width;
                }
                else
                {
                    location.X = 0;
                }
            }
            this.Location = location;
            if (this.Visible)
            {
                Refresh();
            }
            else
            {
                ShowWindow(this.Handle, 4);//SW_SHOWNOACTIVATE
            }
            SetWindowPos(this.Handle, 0, 0, 0, 0, 0, 19);//HWND_TOP, SWP_NOMOVE | SWP_MOSIZE | SWP_NOACTIVATE
            this.Location = location;
        }

        [DllImport("User32.dll")]
        private static extern int ShowWindow(IntPtr hwnd, int flag);

        [DllImport("User32.dll")]
        private static extern int SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
    }
}
