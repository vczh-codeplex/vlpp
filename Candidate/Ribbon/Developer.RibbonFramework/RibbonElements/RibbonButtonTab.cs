using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonButtonTab : RibbonTabBase
    {
        public const int ButtonDropDownPadding = 3;
        public const int ButtonDropDownWidth = 5;
        public const int ButtonDropDownHeight = 3;

        private bool dropDownOpening = false;

        public RibbonDropDownBase DropDown { get; set; }

        protected override RibbonItem CreateMenuItem()
        {
            RibbonMenuButton button = new RibbonMenuButton();
            button.Name = this.Name;
            button.DropDown = this.DropDown;
            return button;
        }

        public override bool Selectable
        {
            get
            {
                return false;
            }
        }

        #region Helper Rendering Functions

        private void RenderPanel(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            switch (this.dropDownOpening ? RibbonElementState.Pressed : this.State)
            {
                case RibbonElementState.Hot:
                    {
                        settings.DrawHotButtonPanel(g, itemBounds, (double)2 / 5);
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        settings.DrawPressedButtonPanel(g, itemBounds, (double)2.25 / 5);
                    }
                    break;
                default:
                    return;
            }
        }

        private void RenderBorder(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            switch (this.dropDownOpening ? RibbonElementState.Pressed : this.State)
            {
                case RibbonElementState.Hot:
                    {
                        settings.DrawHotButtonBorder(g, itemBounds);
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        settings.DrawPressedButtonBorder(g, itemBounds);
                    }
                    break;
            }
        }

        private void RenderDropDown(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            if (this.DropDown != null)
            {
                int dx = itemBounds.Right - ButtonDropDownWidth - ButtonDropDownPadding;
                int dy = itemBounds.Top + (int)(itemBounds.Height - ButtonDropDownHeight) / 2;
                Rectangle dr = new Rectangle(dx, dy, ButtonDropDownWidth, ButtonDropDownHeight);
                settings.DrawDownDropDownTriangle(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
            }
        }

        #endregion

        public override void RenderTab(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, System.Drawing.Rectangle tabBounds)
        {
            RenderPanel(g, settings, tabBounds);
            RenderBorder(g, settings, tabBounds);
            RenderDropDown(g, settings, tabBounds);
            SizeF size = g.MeasureString(this.Name, settings.Font);
            int tx = tabBounds.Left + (tabBounds.Width - (int)size.Width) / 2;
            int ty = tabBounds.Bottom - TabHeightOffset / 2 - (int)size.Height;
            g.DrawString(this.Name, settings.Font, settings.TabText.Brush, tx, ty);
        }

        public override void Update(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            this.TabWidth = (int)size.Width + TabWidthOffset;
            this.TabHeight = (int)size.Height + TabHeightOffset;
            this.State = RibbonElementState.Normal;
        }

        public override void Executed()
        {
            if (this.DropDown != null)
            {
                Rectangle bounds = this.Container.GetTabBounds(this);
                int x = bounds.Left;
                int y = bounds.Bottom;
                this.DropDown.Open(this.Container, new Point(x, y));
                this.dropDownOpening = true;
                this.DropDown.DropDownHost.Closed += new ToolStripDropDownClosedEventHandler(DropDownHost_Closed);
            }
        }

        private void DropDownHost_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            RibbonDropDownHost host = (RibbonDropDownHost)sender;
            host.Closed -= new ToolStripDropDownClosedEventHandler(DropDownHost_Closed);
            this.dropDownOpening = false;
            (this.Container as IRibbonItemContainerServices).RefreshItemContainer();
        }
    }
}
