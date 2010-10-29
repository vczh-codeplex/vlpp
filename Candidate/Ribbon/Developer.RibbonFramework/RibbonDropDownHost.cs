using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Developer.RibbonFramework.RibbonElements;

namespace Developer.RibbonFramework
{
    public class RibbonDropDownHost : ToolStripDropDown
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
            this.ribbonPanel = new DoubleBufferedPanel();
            this.ribbonPanel.BackColor = Color.Yellow;
            this.host = new ToolStripControlHost(this.ribbonPanel);
            this.host.AutoSize = false;
            this.host.Margin = new Padding(0);
            this.Items.Add(host);
            this.Padding = new Padding(0);
            this.BackColor = Color.Blue;

            this.ribbonPanel.Paint += new PaintEventHandler(ribbonPanel_Paint);
            this.ribbonPanel.MouseDown += new MouseEventHandler(ribbonPanel_MouseDown);
            this.ribbonPanel.MouseMove += new MouseEventHandler(ribbonPanel_MouseMove);
            this.ribbonPanel.MouseUp += new MouseEventHandler(ribbonPanel_MouseUp);
            this.ribbonPanel.MouseLeave += new EventHandler(ribbonPanel_MouseLeave);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.Settings.Dispose();
            }
            base.Dispose(disposing);
        }

        protected virtual void PrepareToOpen()
        {
        }

        protected virtual void PaintBackground(Graphics g, Rectangle bounds)
        {
        }

        protected override void OnOpening(System.ComponentModel.CancelEventArgs e)
        {
            using (Bitmap tempBitmap = new Bitmap(1, 1))
            using (Graphics tempGraphics = Graphics.FromImage(tempBitmap))
            {
                this.RibbonItems.Update(tempGraphics, this.Settings);
                PrepareToOpen();
                this.ribbonPanel.Size = this.RibbonContainerSize;
                this.ribbonBitmap = new Bitmap(this.ribbonPanel.Width, this.ribbonPanel.Height);
                this.ribbonGraphics = Graphics.FromImage(this.ribbonBitmap);
                this.RibbonItems.UpdateWithSizeDecided(this.ribbonGraphics, this.Settings);
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

        private MouseEventArgs CopyForRibbon(MouseEventArgs e)
        {
            return new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
        }

        private void Render()
        {
            this.RibbonItems.Render(this.ribbonGraphics, this.Settings, new Rectangle(0, 0, this.ribbonBitmap.Width, this.ribbonBitmap.Height));
        }

        private void ribbonPanel_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.ribbonBitmap, 0, 0);
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
