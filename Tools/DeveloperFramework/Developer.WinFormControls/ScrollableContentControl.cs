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
    public partial class ScrollableContentControl : UserControl
    {
        private Size viewSize;
        private Point viewPosition;
        private Point caretPosition;
        private bool updating = false;
        private int scrollHorizontalHeight = 0;
        private int scrollVerticalWidth = 0;
        private IScrollableContent content = null;

        public Size ViewSize
        {
            get
            {
                return this.viewSize;
            }
            protected set
            {
                this.viewSize = value;
                UpdateScrollBar();
                panelContent.Refresh();
            }
        }

        public Size ViewVisibleSize
        {
            get
            {
                Size size = new Size(
                    Math.Min(this.viewSize.Width, panelContent.Width),
                    Math.Min(this.viewSize.Height, panelContent.Height)
                );
                return size;
            }
        }

        public Size ViewAreaSize
        {
            get
            {
                return panelContent.Size;
            }
        }

        public Point ViewPosition
        {
            get
            {
                return this.viewPosition;
            }
            protected set
            {
                this.viewPosition = value;
                UpdateScrollBar();
                panelContent.Refresh();
            }
        }

        public Point CaretPosition
        {
            get
            {
                return this.caretPosition;
            }
            protected set
            {
                this.caretPosition = value;
                panelContent.UpdateCompositionForContent();
                panelContent.Refresh();
            }
        }

        public int VerticalSmallChange
        {
            get
            {
                return scrollVertical.SmallChange;
            }
            set
            {
                scrollVertical.SmallChange = value;
            }
        }

        public int HorizontalSmallChange
        {
            get
            {
                return scrollHorizontal.SmallChange;
            }
            set
            {
                scrollHorizontal.SmallChange = value;
            }
        }

        public bool ImeEnabled
        {
            get
            {
                return panelContent.ImeEnabled;
            }
            protected set
            {
                panelContent.ImeEnabled = value;
            }
        }

        protected ScrollableContentControl()
        {
            this.updating = true;
            InitializeComponent();
            this.scrollHorizontalHeight = scrollHorizontal.Height;
            this.scrollVerticalWidth = scrollVertical.Width;
            this.updating = false;
            panelContent.ParentControl = this;
        }

        public ScrollableContentControl(IScrollableContent content)
            : this()
        {
            this.content = content;
            this.content.Initialize(panelContent, this);
        }

        protected override void OnBackColorChanged(EventArgs e)
        {
            tableLayoutContent.BackColor = this.BackColor;
            base.OnBackColorChanged(e);
        }

        private int SetScrollBar(ScrollBar scrollBar, int value, int max, int page)
        {
            if (page < max)
            {
                scrollBar.Enabled = true;
                page = Math.Min(max, page);
                value =
                    value < 0 ? 0 :
                    value > max - page ? max - page :
                    value;
                scrollBar.Maximum = Math.Max(1, max - 1);
                scrollBar.LargeChange = Math.Max(1, page);
                scrollBar.Value = value;
                return value;
            }
            else
            {
                scrollBar.Enabled = false;
                return 0;
            }
        }

        private void UpdateScrollBar()
        {
            bool enabledV = scrollVertical.Enabled;
            bool enabledH = scrollHorizontal.Enabled;

            this.updating = true;
            this.viewPosition.X = SetScrollBar(scrollHorizontal, this.viewPosition.X, this.viewSize.Width, panelContent.Width);
            this.viewPosition.Y = SetScrollBar(scrollVertical, this.viewPosition.Y, this.viewSize.Height, panelContent.Height);
            if (scrollVertical.Enabled)
            {
                tableLayoutContent.ColumnStyles[1] = new ColumnStyle(SizeType.AutoSize);
                scrollVertical.Width = this.scrollVerticalWidth;
            }
            else
            {
                tableLayoutContent.ColumnStyles[1] = new ColumnStyle(SizeType.Absolute, 0);
            }
            if (scrollHorizontal.Enabled)
            {
                tableLayoutContent.RowStyles[1] = new RowStyle(SizeType.AutoSize);
                scrollHorizontal.Height = this.scrollHorizontalHeight;
            }
            else
            {
                tableLayoutContent.RowStyles[1] = new RowStyle(SizeType.Absolute, 0);
            }
            this.updating = false;

            if (enabledV != scrollVertical.Enabled || enabledH != scrollHorizontal.Enabled)
            {
                UpdateScrollBar();
            }
        }

        private void scrollVertical_ValueChanged(object sender, EventArgs e)
        {
            if (!this.updating)
            {
                this.viewPosition.Y = scrollVertical.Value;
                panelContent.Refresh();
            }
        }

        private void scrollHorizontal_ValueChanged(object sender, EventArgs e)
        {
            if (!this.updating)
            {
                this.viewPosition.X = scrollHorizontal.Value;
                panelContent.Refresh();
            }
        }

        private void panelContent_Paint(object sender, PaintEventArgs e)
        {
            if (this.content != null)
            {
                Rectangle area = new Rectangle(this.viewPosition, this.ViewVisibleSize);
                this.content.RenderContent(e.Graphics, area, panelContent.Bounds);
            }
        }

        private void panelContent_Resize(object sender, EventArgs e)
        {
            if (!this.updating)
            {
                UpdateScrollBar();
                panelContent.Refresh();
            }
        }

        private class ImePanel : UserControl
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

            public bool ImeEnabled { get; set; }
            public ScrollableContentControl ParentControl { get; set; }

            public ImePanel()
            {
                this.ImeEnabled = false;
                this.DoubleBuffered = true;
            }

            protected override bool IsInputKey(Keys keyData)
            {
                return true;
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

            protected override bool CanEnableIme
            {
                get
                {
                    return this.ImeEnabled;
                }
            }

            protected override void WndProc(ref Message m)
            {
                if (m.Msg == WM_IME_STARTCOMPOSITION)
                {
                    UpdateCompositionForContent();
                }
                base.WndProc(ref m);
            }
        }
    }

    public interface IScrollableContent
    {
        void Initialize(Control host, ScrollableContentControl control);
        void RenderContent(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds);
    }
}
