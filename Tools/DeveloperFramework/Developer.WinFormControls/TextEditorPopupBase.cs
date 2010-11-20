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
        private MouseHook mouseHook = new MouseHook();

        public TextEditorPopupBase()
        {
            InitializeComponent();
            this.mouseHook.OnMouseActivity += new MouseEventHandler(mouseHook_OnMouseActivity);
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
            SetWindowPos(this.Handle, -1, 0, 0, 0, 0, 19);//HWND_TOPMOST, SWP_NOMOVE | SWP_MOSIZE | SWP_NOACTIVATE
            this.Location = location;
            this.Size = size;
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            if (this.Visible)
            {
                this.mouseHook.Start();
            }
            else
            {
                this.mouseHook.Stop();
            }
        }

        private void GlobalMouseDown()
        {
            if (!new Rectangle(PointToScreen(new Point(0, 0)), this.Size).Contains(Control.MousePosition))
            {
                Hide();
            }
        }

        private void mouseHook_OnMouseActivity(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.None)
            {
                if (this.InvokeRequired)
                {
                    this.Invoke(new MethodInvoker(() =>
                    {
                        GlobalMouseDown();
                    }));
                }
                else
                {
                    GlobalMouseDown();
                }
            }
        }

        [DllImport("User32.dll")]
        private static extern int ShowWindow(IntPtr hwnd, int flag);

        [DllImport("User32.dll")]
        private static extern int SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
    }
}
