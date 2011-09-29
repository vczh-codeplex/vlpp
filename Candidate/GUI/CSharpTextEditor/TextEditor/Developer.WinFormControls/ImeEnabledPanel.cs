using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Developer.WinFormControls
{
    public partial class ImeEnabledPanel : UserControl
    {
        private const int CFS_POINT = 0x0002;
        private const int WM_IME_STARTCOMPOSITION = 0x010D;

        [StructLayout(LayoutKind.Sequential)]
        private struct Point
        {
            public int x;
            public int y;
        }

        [StructLayout(LayoutKind.Explicit)]
        private struct Rect
        {
            [FieldOffset(0)]
            public int left;
            [FieldOffset(4)]
            public int top;
            [FieldOffset(8)]
            public int right;
            [FieldOffset(12)]
            public int bottom;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct COMPOSITIONFORM
        {
            public int dwStyle;
            public Point ptCurrentPos;
            public Rect rcArea;
        }

        [DllImport("Imm32.dll")]
        private static extern int ImmGetContext(IntPtr hWnd);

        [DllImport("imm32.dll")]
        private static extern int ImmSetCompositionWindow(int hIMC, ref COMPOSITIONFORM lpCompositionForm);

        [DllImport("Imm32.dll")]
        private static extern bool ImmReleaseContext(IntPtr hWnd, int hIMC);

        public ScrollableContentControl ParentControl { get; set; }

        public ImeEnabledPanel()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.DoubleBuffered = true;
        }

        public void UpdateCompositionForContent()
        {
            int imc = ImmGetContext(this.Handle);
            COMPOSITIONFORM cf = new COMPOSITIONFORM();
            cf.dwStyle = CFS_POINT;
            cf.ptCurrentPos = new Point();
            cf.ptCurrentPos.x = this.ParentControl.CaretPosition.X - this.ParentControl.ViewPosition.X;
            cf.ptCurrentPos.y = this.ParentControl.CaretPosition.Y - this.ParentControl.ViewPosition.Y;
            int res = ImmSetCompositionWindow(imc, ref cf);
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_IME_STARTCOMPOSITION)
            {
                UpdateCompositionForContent();
            }
            base.WndProc(ref m);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (this.ParentControl != null && this.ParentControl.ScrollableContent != null)
            {
                this.ParentControl.ScrollableContent.RenderContent(e.Graphics);
            }
        }

        private void ImeEnabledPanel_Paint(object sender, PaintEventArgs e)
        {
        }
    }
}
