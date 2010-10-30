using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonButtonTab : RibbonTabBase
    {
        public const int ButtonDropDownPadding = 3;
        public const int ButtonDropDownWidth = 5;
        public const int ButtonDropDownHeight = 3;

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
            RibbonColorItem i1 = null;
            RibbonColorItem i2 = null;
            RibbonColorItem i3 = null;
            RibbonColorItem i4 = null;
            double ratio = 0;
            switch (this.State)
            {
                case RibbonElementState.Hot:
                    {
                        i1 = settings.ButtonReleasedBackground1;
                        i2 = settings.ButtonReleasedBackground2;
                        i3 = settings.ButtonReleasedBackground3;
                        i4 = settings.ButtonReleasedBackground4;
                        ratio = (double)2 / 5;
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        i1 = settings.ButtonPressedBackground1;
                        i2 = settings.ButtonPressedBackground2;
                        i3 = settings.ButtonPressedBackground3;
                        i4 = settings.ButtonPressedBackground4;
                        ratio = (double)2.25 / 5;
                    }
                    break;
                default:
                    return;
            }
            settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(itemBounds, 0, -1), ratio);
        }

        private void RenderBorder(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            RibbonColorItem outTop = null;
            RibbonColorItem outBottom = null;
            RibbonColorItem inTop = null;
            RibbonColorItem inBottom = null;
            switch (this.State)
            {
                case RibbonElementState.Hot:
                    {
                        outTop = settings.ButtonReleasedOuterBorderTop;
                        outBottom = settings.ButtonReleasedOuterBorderBottom;
                        inTop = settings.ButtonReleasedInnerBorderTop;
                        inBottom = settings.ButtonReleasedInnerBorderBottom;
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        outTop = settings.ButtonPressedOuterBorderTop;
                        outBottom = settings.ButtonPressedOuterBorderBottom;
                        inTop = settings.ButtonPressedInnerBorderTop;
                        inBottom = settings.ButtonPressedInnerBorderBottom;
                    }
                    break;
                default:
                    return;
            }
            settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
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
        }
    }
}
