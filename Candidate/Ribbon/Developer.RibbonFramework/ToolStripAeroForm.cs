using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;

namespace Developer.RibbonFramework
{
    public partial class ToolStripAeroForm : AeroForm
    {
        private Image aeroIcon = null;
        private Size aeroIconSize = new Size(16, 16);
        private ToolStrip aeroToolStrip = null;
        private bool updated = false;

        private Bitmap aeroBitmap = null;
        private Graphics aeroGraphics = null;

        public ToolStripAeroForm()
        {
            InitializeComponent();
            this.updated = true;
            RecreateAeroBitmap(this.AeroClientBounds.Size);
        }

        protected void InitializeAero(ToolStrip toolStrip)
        {
            this.AeroToolStrip = toolStrip;
            base.InitializeAero();
        }

        public Image AeroIcon
        {
            get
            {
                return this.aeroIcon;
            }
            set
            {
                SetImage(ref this.aeroIcon, value);
                pictureIcon.Image = this.aeroIcon;
            }
        }

        public Size AeroIconSize
        {
            get
            {
                return this.aeroIconSize;
            }
            set
            {
                this.aeroIconSize = value;
                OnSizeChanged(new EventArgs());
            }
        }

        public ToolStrip AeroToolStrip
        {
            get
            {
                return this.aeroToolStrip;
            }
            set
            {
                this.aeroToolStrip = value;
                if (this.aeroToolStrip != null)
                {
                    this.aeroToolStrip.Renderer = new ToolStripAeroFormToolStripRenderer();
                }
                OnSizeChanged(new EventArgs());
            }
        }

        public Bitmap AeroBitmap
        {
            get
            {
                return this.aeroBitmap;
            }
        }

        public Graphics AeroGraphics
        {
            get
            {
                return this.aeroGraphics;
            }
        }

        public Panel AeroContainer
        {
            get
            {
                return panelContainer;
            }
        }

        public override Padding AeroAreaPadding
        {
            get
            {
                Padding padding = this.AeroBorderPadding;
                padding.Top += this.AeroBorderCaptionHeight;
                return padding;
            }
        }

        public override Padding AeroBorderPadding
        {
            get
            {
                return new Padding(8, 5, 8, 8);
            }
        }

        public override int AeroBorderCaptionHeight
        {
            get
            {
                if (this.aeroToolStrip == null)
                {
                    return base.AeroBorderCaptionHeight;
                }
                else
                {
                    return aeroToolStrip.Height;
                }
            }
        }

        public override bool AeroFullBorderControl
        {
            get
            {
                return true;
            }
        }

        public Rectangle CalculateAeroClientBounds(Size size)
        {
            if (this.IsAeroFullBorderControlEnabled)
            {
                int x = this.AeroBorderPadding.Left;
                int y = this.AeroBorderPadding.Top + this.AeroBorderCaptionHeight;
                int w = size.Width - this.AeroBorderPadding.Left - this.AeroBorderPadding.Right;
                int h = size.Height - this.AeroBorderPadding.Top - this.AeroBorderPadding.Bottom - this.AeroBorderCaptionHeight;
                return new Rectangle(x, y, w, h);
            }
            else
            {
                int x = 0;
                int y = this.AeroBorderCaptionHeight;
                int w = size.Width;
                int h = size.Height - this.AeroBorderCaptionHeight;
                return new Rectangle(x, y, w, h);
            }
        }

        public Rectangle CalculateAeroIconBounds(Size size)
        {
            if (this.IsAeroFullBorderControlEnabled)
            {
                int x = this.AeroBorderPadding.Left;
                int y = this.AeroBorderPadding.Top;
                int w = this.AeroIconSize.Width;
                int h = this.AeroIconSize.Height;
                return new Rectangle(x, y, w, h);
            }
            else
            {
                int x = 0;
                int y = 0;
                int w = this.AeroIconSize.Width;
                int h = this.AeroIconSize.Height;
                return new Rectangle(x, y, w, h);
            }
        }

        public Rectangle AeroClientBounds
        {
            get
            {
                return CalculateAeroClientBounds(this.ClientSize);
            }
        }

        public Rectangle AeroIconBounds
        {
            get
            {
                return CalculateAeroIconBounds(this.ClientSize);
            }
        }

        private void SetImage(ref Image image, Image value)
        {
            if (image != value)
            {
                if (image != null)
                {
                    image.Dispose();
                }
                image = value == null ? null : (Image)value.Clone();
            }
        }

        private void RecreateAeroBitmap(Size size)
        {
            if (this.aeroBitmap != null)
            {
                if (size == this.aeroBitmap.Size)
                {
                    return;
                }
                this.aeroGraphics.Dispose();
                this.aeroBitmap.Dispose();
            }
            this.aeroBitmap = new Bitmap(Math.Max(1, size.Width), Math.Max(1, size.Height));
            this.aeroGraphics = Graphics.FromImage(this.AeroBitmap);
            this.aeroGraphics.CompositingQuality = CompositingQuality.HighQuality;
        }

        private void SetControlBounds(Control c, Rectangle r)
        {
            c.SetBounds(r.Left, r.Top, r.Width, r.Height);
        }

        protected void RenderIcon(Graphics g, Rectangle bounds)
        {
            if (this.aeroIcon != null)
            {
                g.DrawImage(this.aeroIcon, bounds);
            }
        }

        protected void RenderAeroBitmap()
        {
            Rectangle r = new Rectangle(0, 0, this.aeroBitmap.Width, this.aeroBitmap.Height);
            OnDrawBackground(this.aeroGraphics, r);
            r = this.AeroIconBounds;
            r.Offset(-this.AeroClientBounds.Left, -this.AeroClientBounds.Top);
            RenderIcon(this.aeroGraphics, r);
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WindowsAPI.WM_ERASEBKGND:
                    break;
                case WindowsAPI.WM_WINDOWPOSCHANGED:
                    if (this.aeroToolStrip != null)
                    {
                        int x = this.AeroIconBounds.Left;
                        int y = this.AeroIconBounds.Top;
                        this.aeroToolStrip.Location = new Point(x + this.aeroIconSize.Width + 3, y);
                    }
                    if (this.updated)
                    {
                        int l = (int)m.LParam;
                        WindowsAPI.WINDOWPOS pos = (WindowsAPI.WINDOWPOS)Marshal.PtrToStructure(m.LParam, typeof(WindowsAPI.WINDOWPOS));
                        Size size = new Size(pos.cx, pos.cy);
                        if (!this.IsAeroFullBorderControlEnabled)
                        {
                            size.Width -= this.Size.Width - this.ClientSize.Width;
                            size.Height -= this.Size.Height - this.ClientSize.Height;
                        }
                        Rectangle r = CalculateAeroClientBounds(size);
                        SetControlBounds(panelContainer, r);
                        SetControlBounds(pictureIcon, CalculateAeroIconBounds(size));
                        RecreateAeroBitmap(r.Size);
                        RenderAeroBitmap();
                    }
                    base.WndProc(ref m);
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }

        protected virtual void OnDrawBackground(Graphics g, Rectangle bounds)
        {
            g.FillRectangle(SystemBrushes.Control, bounds);
        }

        protected override void OnFormClosed(FormClosedEventArgs e)
        {
            base.OnFormClosed(e);
            if (this.aeroIcon != null)
            {
                this.AeroIcon = null;
            }
            this.AeroGraphics.Dispose();
            this.AeroBitmap.Dispose();
            this.updated = false;
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            if (this.updated)
            {
                RenderAeroBitmap();
                if (!this.IsAeroFullBorderControlEnabled)
                {
                    panelContainer.Refresh();
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (this.IsAeroFullBorderControlEnabled)
            {
                SizeF size = this.aeroGraphics.MeasureString(this.Text, SystemFonts.CaptionFont);
                int x = 0;
                int y = 0;
                if (this.aeroToolStrip != null)
                {
                    x = this.aeroToolStrip.Left + this.aeroToolStrip.Width + WindowsAPI.DrawAeroTextPadding;
                    y = this.aeroToolStrip.Top + Math.Max(0, (int)(this.aeroToolStrip.Height - size.Height) / 2);
                }
                else
                {
                    x = this.AeroAreaPadding.Left + this.aeroIconSize.Width + 3 + WindowsAPI.DrawAeroTextPadding;
                    y = this.AeroBorderPadding.Top;
                }
                int w = (int)size.Width;
                int h = (int)size.Height;
                WindowsAPI.DrawAeroText(e.Graphics, this.Text, SystemFonts.CaptionFont, new Rectangle(x, y, w, h), SystemColors.ActiveCaptionText);
            }
        }

        private void panelContainer_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.aeroBitmap, 0, 0);
        }
    }

    class ToolStripAeroFormToolStripRenderer : ToolStripSystemRenderer
    {
        protected override void OnRenderToolStripBackground(ToolStripRenderEventArgs e)
        {
            if (e.ToolStrip.IsDropDown)
            {
                base.OnRenderToolStripBackground(e);
            }
            else
            {
                e.Graphics.Clear(Color.Transparent);
            }
        }

        protected override void OnRenderToolStripBorder(ToolStripRenderEventArgs e)
        {
            if (e.ToolStrip.IsDropDown)
            {
                base.OnRenderToolStripBorder(e);
            }
        }

        protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
        {
            if (e.Item is ToolStripLabel)
            {
                e.Graphics.Clear(Color.Transparent);
                Form form = e.ToolStrip.FindForm();
                WindowsAPI.DrawAeroText(e.Graphics, e.Text, e.TextFont, e.TextRectangle, SystemColors.ActiveCaptionText, e.TextFormat, WindowsAPI.TextStyle.Glowing);
            }
            else
            {
                base.OnRenderItemText(e);
            }
        }
    }
}
