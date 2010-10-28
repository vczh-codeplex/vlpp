﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonButton : RibbonButtonGroupItem
    {
        public const int ButtonSmallIconSize = 16;
        public const int ButtonBigIconSize = 32;
        public const int ButtonBorder = 3;
        public const int ButtonTextPadding = 3;

        public enum VisualState
        {
            Normal,
            Hot,
            Pressed
        }

        public VisualState BorderVisualState { get; protected set; }
        public VisualState PanelVisualState { get; protected set; }

        public RibbonButtonStyle ButtonStyle { get; set; }
        public int ToggleGroup { get; set; }
        public bool Toggled { get; set; }

        public bool EnabledToggling
        {
            get
            {
                return this.ButtonStyle == RibbonButtonStyle.GroupedToggleButton || this.ButtonStyle == RibbonButtonStyle.ToggleButton;
            }
        }

        public RibbonButton()
        {
            this.ButtonStyle = RibbonButtonStyle.PushButton;
            this.ToggleGroup = -1;
            this.Toggled = false;
        }

        public override int GetBigSize(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Group.Tab.Container.Font);
            return Math.Max((int)size.Width + 2 * ButtonTextPadding, ButtonBigIconSize + 2 * ButtonBorder);
        }

        public override int GetSmallWidth(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Group.Tab.Container.Font);
            return ButtonSmallIconSize + 2 * ButtonBorder + 2 * ButtonTextPadding + (int)size.Width;
        }

        public override int GetSmallCompactWidth(Graphics g)
        {
            return ButtonSmallIconSize + 2 * ButtonBorder;
        }

        public override void Render(Graphics g, Rectangle itemBounds)
        {
            var settings = this.Group.Tab.Container.Settings;

            switch (this.PanelVisualState)
            {
                case VisualState.Hot:
                    {
                        RibbonColorItem i1 = settings.ButtonReleasedBackground1;
                        RibbonColorItem i2 = settings.ButtonReleasedBackground2;
                        RibbonColorItem i3 = settings.ButtonReleasedBackground3;
                        RibbonColorItem i4 = settings.ButtonReleasedBackground4;
                        settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(itemBounds, 0, -1), (double)2 / 5);
                    }
                    break;
                case VisualState.Pressed:
                    {
                        RibbonColorItem i1 = settings.ButtonPressedBackground1;
                        RibbonColorItem i2 = settings.ButtonPressedBackground2;
                        RibbonColorItem i3 = settings.ButtonPressedBackground3;
                        RibbonColorItem i4 = settings.ButtonPressedBackground4;
                        settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(itemBounds, 0, -1), (double)2.25 / 5);
                    }
                    break;
            }
            {
                int iconSize = this.ItemSize == RibbonItemSize.Big ? ButtonBigIconSize : ButtonSmallIconSize;
                int ix = itemBounds.Left + ButtonBorder;
                int iy = itemBounds.Top + ButtonBorder;
                if (this.ItemSize == RibbonItemSize.Big)
                {
                    ix = itemBounds.Left + (itemBounds.Width - iconSize) / 2;
                }
                Rectangle iconBounds = new Rectangle(ix, iy, iconSize, iconSize);
                g.DrawImage(this.Image, iconBounds);

                Font font = this.Group.Tab.Container.Font;
                SizeF size = g.MeasureString(this.Name, font);
                Brush textBrush = settings.TabText.Brush;
                switch (this.ItemSize)
                {
                    case RibbonItemSize.Big:
                        {
                            int tx = itemBounds.Left + (int)(itemBounds.Width - size.Width) / 2;
                            int ty = iconBounds.Bottom + (int)(itemBounds.Bottom - iconBounds.Bottom - size.Height) / 2;
                            g.DrawString(this.Name, font, textBrush, tx, ty);
                        }
                        break;
                    case RibbonItemSize.Small:
                        {
                            int tx = iconBounds.Right + ButtonTextPadding;
                            int ty = iconBounds.Top + (int)(iconBounds.Height - size.Height) / 2;
                            g.DrawString(this.Name, font, textBrush, tx, ty);
                        }
                        break;
                }
            }
            switch (this.BorderVisualState)
            {
                case VisualState.Hot:
                    {
                        RibbonColorItem outTop = settings.ButtonReleasedOuterBorderTop;
                        RibbonColorItem outBottom = settings.ButtonReleasedOuterBorderBottom;
                        RibbonColorItem inTop = settings.ButtonReleasedInnerBorderTop;
                        RibbonColorItem inBottom = settings.ButtonReleasedInnerBorderBottom;
                        settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
                    }
                    break;
                case VisualState.Pressed:
                    {
                        RibbonColorItem outTop = settings.ButtonPressedOuterBorderTop;
                        RibbonColorItem outBottom = settings.ButtonPressedOuterBorderBottom;
                        RibbonColorItem inTop = settings.ButtonPressedInnerBorderTop;
                        RibbonColorItem inBottom = settings.ButtonPressedInnerBorderBottom;
                        settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
                    }
                    break;
            }
        }

        public override void Update(Graphics g)
        {
            if (this.Toggled && this.EnabledToggling)
            {
                UpdateVisualStyle(VisualState.Pressed);
            }
            else
            {
                UpdateVisualStyle(VisualState.Normal);
            }
        }

        public override bool OnMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            this.Group.Tab.Container.Capture(this);
            return UpdateVisualStyleToPressed();
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            if (this.Group.Tab.Container.CapturedItem == this)
            {
                if (this.Group.GetItemFromPoint(e.Location) == this)
                {
                    return UpdateVisualStyleToPressed();
                }
                else
                {
                    return UpdateVisualStyleToNormal();
                }
            }
            else
            {
                return UpdateVisualStyleToHot();
            }
        }

        public override bool OnMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            this.Group.Tab.Container.Capture(null);
            if (this.Group.GetItemFromPoint(e.Location) == this)
            {
                Executed();
                return UpdateVisualStyleToHot();
            }
            else
            {
                return UpdateVisualStyleToNormal();
            }
        }

        public override bool OnMouseLeave(EventArgs e)
        {
            return UpdateVisualStyleToNormal();
        }

        protected virtual void Executed()
        {
            switch (this.ButtonStyle)
            {
                case RibbonButtonStyle.GroupedToggleButton:
                    {
                        if (!this.Toggled)
                        {
                            foreach (var item in this.Group.Items)
                            {
                                RibbonButton button = item as RibbonButton;
                                if (button.ButtonStyle == RibbonButtonStyle.GroupedToggleButton)
                                {
                                    button.Toggled = false;
                                }
                            }
                            this.Toggled = true;
                        }
                    }
                    break;
                case RibbonButtonStyle.ToggleButton:
                    {
                        this.Toggled = !this.Toggled;
                    }
                    break;
            }
        }

        protected bool UpdateVisualStyleToNormal()
        {
            if (this.EnabledToggling && this.Toggled)
            {
                return UpdateVisualStyle(VisualState.Pressed, VisualState.Pressed);
            }
            else
            {
                return UpdateVisualStyle(VisualState.Normal);
            }
        }

        protected bool UpdateVisualStyleToHot()
        {
            if (this.EnabledToggling && this.Toggled)
            {
                return UpdateVisualStyle(VisualState.Hot, VisualState.Pressed);
            }
            else
            {
                return UpdateVisualStyle(VisualState.Hot);
            }
        }

        protected bool UpdateVisualStyleToPressed()
        {
            if (this.EnabledToggling && this.Toggled)
            {
                return UpdateVisualStyle(VisualState.Pressed, VisualState.Pressed);
            }
            else
            {
                return UpdateVisualStyle(VisualState.Pressed);
            }
        }

        private bool UpdateVisualStyle(VisualState state)
        {
            return UpdateVisualStyle(state, state);
        }

        private bool UpdateVisualStyle(VisualState border, VisualState panel)
        {
            bool result = this.BorderVisualState != border || this.PanelVisualState != panel;
            this.BorderVisualState = border;
            this.PanelVisualState = panel;
            return result;
        }
    }

    public enum RibbonButtonStyle
    {
        PushButton,
        ToggleButton,
        GroupedToggleButton,
    }
}
