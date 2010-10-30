using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonTab : RibbonTabBase
    {
        public RibbonTabPanel TabPanel { get; private set; }

        public RibbonTab()
        {
            this.TabPanel = new RibbonTabPanel(this);
        }

        protected override RibbonItem CreateMenuItem()
        {
            RibbonMenuButton button = new RibbonMenuButton();
            button.Name = this.Name;
            button.Executed += new EventHandler(menuItem_Executed);
            return button;
        }

        private void menuItem_Executed(object sender, EventArgs e)
        {
            Executed();
        }

        public override bool Selectable
        {
            get
            {
                return true;
            }
        }

        public override void RenderTab(Graphics g, RibbonThemaSettingsBase settings, Rectangle tabBounds)
        {
            switch (this.State)
            {
                case RibbonElementState.Normal:
                    {
                    }
                    break;
                case RibbonElementState.Hot:
                    {
                        using (GraphicsPath path = new GraphicsPath())
                        {
                            path.AddLine(tabBounds.Left, tabBounds.Bottom, tabBounds.Left, tabBounds.Top + TabRoundRadius);
                            path.AddArc(tabBounds.Left, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 180, 90);
                            path.AddLine(tabBounds.Left + TabRoundRadius, tabBounds.Top, tabBounds.Right - TabRoundRadius, tabBounds.Top);
                            path.AddArc(tabBounds.Right - 2 * TabRoundRadius, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 270, 90);
                            path.AddLine(tabBounds.Right, tabBounds.Top + TabRoundRadius, tabBounds.Right, tabBounds.Bottom);
                            g.DrawPath(settings.Border.Pen, path);
                        }
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        using (GraphicsPath path = new GraphicsPath())
                        {
                            path.AddArc(tabBounds.Left - 2 * TabRoundRadius, tabBounds.Bottom - 2 * TabRoundRadius, TabRoundRadius * 2, TabRoundRadius * 2, 90, -90);
                            path.AddLine(tabBounds.Left, tabBounds.Bottom - TabRoundRadius, tabBounds.Left, tabBounds.Top + TabRoundRadius);
                            path.AddArc(tabBounds.Left, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 180, 90);
                            path.AddLine(tabBounds.Left + TabRoundRadius, tabBounds.Top, tabBounds.Right - TabRoundRadius, tabBounds.Top);
                            path.AddArc(tabBounds.Right - 2 * TabRoundRadius, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 270, 90);
                            path.AddLine(tabBounds.Right, tabBounds.Top + TabRoundRadius, tabBounds.Right, tabBounds.Bottom - TabRoundRadius);
                            path.AddArc(tabBounds.Right, tabBounds.Bottom - 2 * TabRoundRadius, TabRoundRadius * 2, TabRoundRadius * 2, 180, -90);
                            g.DrawLine(settings.Panel1.Pen, tabBounds.Left - TabRoundRadius, tabBounds.Bottom, tabBounds.Right + TabRoundRadius, tabBounds.Bottom);
                            g.FillPath(settings.Panel1.Brush, path);
                            g.DrawPath(settings.Border.Pen, path);
                        }
                    }
                    break;
            }
            SizeF size = g.MeasureString(this.Name, settings.Font);
            int tx = tabBounds.Left + (tabBounds.Width - (int)size.Width) / 2;
            int ty = tabBounds.Bottom - TabHeightOffset / 2 - (int)size.Height;
            g.DrawString(this.Name, settings.Font, settings.TabText.Brush, tx, ty);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            this.TabWidth = (int)size.Width + TabWidthOffset;
            this.TabHeight = (int)size.Height + TabHeightOffset;
            this.State = RibbonElementState.Normal;
            this.TabPanel.Update(g, settings);
        }

        public override void Executed()
        {
            this.Container.SelectTabPanel(this.TabPanel);
        }
    }
}
