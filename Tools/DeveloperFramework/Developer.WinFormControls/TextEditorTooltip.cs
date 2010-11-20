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
    public partial class TextEditorTooltip : TextEditorPopupBase
    {
        private Bitmap bitmap = null;
        private Control ownerControl = null;

        public TextEditorTooltip()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.Opaque, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        public void Show(Control control, Point locationTop, Point locationBottom, string text)
        {
            if (control == null) return;
            this.ownerControl = control;
            text = text.TrimEnd(' ', '\t', '\r', '\n');

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
            this.ClientSize = textSize;
            Show(control, locationTop, locationBottom);
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

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);
            this.ownerControl.Focus();
        }

        [DllImport("User32.dll")]
        private static extern int ShowWindow(IntPtr hwnd, int flag);

        [DllImport("User32.dll")]
        private static extern int SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
    }
}
