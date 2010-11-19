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
    public partial class TextEditorTooltip : Form
    {
        private Bitmap bitmap = null;
        private Form ownerForm = null;

        public TextEditorTooltip()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.Opaque, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        public void Show(Form owner, Point locationTop, Point locationBottom, string text)
        {
            if (owner == null) return;
            this.ownerForm = owner;
            text = text.TrimEnd(' ', '\t', '\r', '\n');

            Point location;
            Size textSize;
            using (Graphics g = Graphics.FromHwnd(this.Handle))
            {
                textSize = TextRenderer.MeasureText(g, text, this.Font, new Size(0, 0), TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
            }
            textSize.Width += 10;
            textSize.Height += 10;

            if (this.bitmap != null)
            {
                this.bitmap.Dispose();
            }
            this.bitmap = new Bitmap(textSize.Width, textSize.Height);
            using (Graphics g = Graphics.FromImage(this.bitmap))
            {
                g.FillRectangle(SystemBrushes.Info, 0, 0, textSize.Width, textSize.Height);
                TextRenderer.DrawText(g, text, this.Font, new Point(5, 5), SystemColors.InfoText, TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
                g.DrawRectangle(SystemPens.InfoText, 0, 0, textSize.Width - 1, textSize.Height - 1);
            }

            Screen screen = Screen.FromControl(owner);
            if (locationBottom.Y + textSize.Height < screen.WorkingArea.Bottom)
            {
                location = locationBottom;
            }
            else if (locationTop.Y - textSize.Height >= screen.WorkingArea.Top)
            {
                location = new Point(locationTop.X, locationTop.Y - textSize.Height);
            }
            else
            {
                location = new Point(locationTop.X, 0);
            }
            if (location.X + textSize.Width > screen.WorkingArea.Right)
            {
                if (location.X - textSize.Width >= screen.WorkingArea.Left)
                {
                    location.X -= textSize.Width;
                }
                else
                {
                    location.X = 0;
                }
            }
            this.SetBounds(location.X, location.Y, textSize.Width, textSize.Height);
            if (this.Visible)
            {
                Hide();
            }
            else
            {
                ShowWindow(this.Handle, 4);//SW_SHOWNOACTIVATE
            }
            SetWindowPos(this.Handle, 0, 0, 0, 0, 0, 19);//HWND_TOP, SWP_NOMOVE | SWP_MOSIZE | SWP_NOACTIVATE
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            this.bitmap.Dispose();
            this.bitmap = null;
            Hide();
            e.Cancel = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.bitmap, 0, 0);
            base.OnPaint(e);
        }

        [DllImport("User32.dll")]
        private static extern int ShowWindow(IntPtr hwnd, int flag);

        [DllImport("User32.dll")]
        private static extern int SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
    }
}
