using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Developer.RibbonFramework.RibbonElements;

namespace Developer.RibbonFramework
{
    public abstract class RibbonDropDownHost : ToolStripDropDown
    {
        class DoubleBufferedPanel : Panel
        {
            public DoubleBufferedPanel()
            {
                this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            }
        }

        private DoubleBufferedPanel ribbonPanel = null;
        private ToolStripControlHost host = null;
        private Bitmap ribbonBitmap = null;
        private Graphics ribbonGraphics = null;

        public RibbonItemContainer RibbonItems { get; set; }
        public RibbonThemaSettingsBase Settings { get; set; }
        public Size RibbonContainerSize { get; set; }

        public RibbonDropDownHost()
        {
            this.Settings = new RibbonThemaSettings();
            this.Settings.Font = SystemFonts.CaptionFont;
            this.ribbonPanel = new DoubleBufferedPanel();
            this.host = new ToolStripControlHost(this.ribbonPanel);
            this.host.AutoSize = false;
            this.host.Margin = new Padding(0);
            this.Items.Add(host);
            this.Padding = new Padding(0);

            this.ribbonPanel.Paint += new PaintEventHandler(ribbonPanel_Paint);
            this.ribbonPanel.MouseDown += new MouseEventHandler(ribbonPanel_MouseDown);
            this.ribbonPanel.MouseMove += new MouseEventHandler(ribbonPanel_MouseMove);
            this.ribbonPanel.MouseUp += new MouseEventHandler(ribbonPanel_MouseUp);
            this.ribbonPanel.MouseLeave += new EventHandler(ribbonPanel_MouseLeave);
        }

        protected void CaptureMouse(bool capture)
        {
            this.ribbonPanel.Capture = capture;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.Settings.Dispose();
            }
            base.Dispose(disposing);
        }

        protected Panel RibbonPanelControl
        {
            get
            {
                return this.ribbonPanel;
            }
        }

        protected ToolStripItem HostingItem
        {
            get
            {
                return this.host;
            }
        }

        protected abstract void PrepareToOpen();
        protected abstract Size GetSuggestedSize();

        protected virtual void PaintBackground(Graphics g, Rectangle bounds)
        {
            RibbonColorItem i1 = this.Settings.Panel1;
            RibbonColorItem i2 = this.Settings.Panel2;
            RibbonColorItem i3 = this.Settings.Panel3;
            RibbonColorItem i4 = this.Settings.Panel4;
            this.Settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, bounds, (double)1 / 6);
        }

        protected override void OnOpening(System.ComponentModel.CancelEventArgs e)
        {
            using (Bitmap tempBitmap = new Bitmap(1, 1))
            using (Graphics tempGraphics = Graphics.FromImage(tempBitmap))
            {
                this.RibbonItems.Update(tempGraphics, this.Settings);
                PrepareToOpen();
                this.RibbonItems.UpdateWithSizeDecided(tempGraphics, this.Settings);
                this.RibbonContainerSize = GetSuggestedSize();
                this.ribbonPanel.Size = this.RibbonContainerSize;

                this.ribbonBitmap = new Bitmap(this.ribbonPanel.Width, this.ribbonPanel.Height);
                this.ribbonGraphics = Graphics.FromImage(this.ribbonBitmap);
                Render();
            }
            base.OnOpening(e);
        }

        protected override void OnOpened(EventArgs e)
        {
            base.OnOpened(e);
        }

        protected override void OnClosing(ToolStripDropDownClosingEventArgs e)
        {
            base.OnClosing(e);
        }

        protected override void OnClosed(ToolStripDropDownClosedEventArgs e)
        {
            base.OnClosed(e);
            this.ribbonGraphics.Dispose();
            this.ribbonGraphics = null;
            this.ribbonBitmap.Dispose();
            this.ribbonBitmap = null;
        }

        protected void Render()
        {
            if (this.ribbonBitmap != null)
            {
                Rectangle bounds = new Rectangle(0, 0, this.ribbonBitmap.Width, this.ribbonBitmap.Height);
                PaintBackground(this.ribbonGraphics, bounds);
                this.RibbonItems.Render(this.ribbonGraphics, this.Settings, bounds);
            }
        }

        private void ribbonPanel_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.ribbonBitmap, 0, 0);
        }

        private MouseEventArgs CopyForRibbon(MouseEventArgs e)
        {
            return new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
        }

        private void ribbonPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.RibbonItems.OnMouseDown(CopyForRibbon(e)))
            {
                Render();
                this.ribbonPanel.Refresh();
            }
        }

        private void ribbonPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.RibbonItems.OnMouseMove(CopyForRibbon(e)))
            {
                Render();
                this.ribbonPanel.Refresh();
            }
        }

        private void ribbonPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.RibbonItems.OnMouseUp(CopyForRibbon(e)))
            {
                Render();
                this.ribbonPanel.Refresh();
            }
        }

        private void ribbonPanel_MouseLeave(object sender, EventArgs e)
        {
            if (this.RibbonItems.OnMouseLeave(e))
            {
                Render();
                this.ribbonPanel.Refresh();
            }
        }
    }
}
