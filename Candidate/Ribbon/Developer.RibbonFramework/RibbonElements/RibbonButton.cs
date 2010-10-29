using System;
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
        public const int ButtonToolWidth = 23;
        public const int ButtonDropDownWidth = 5;
        public const int ButtonDropDownHeight = 3;
        public const int ButtonDropDownSpaceAdditionalWidth = ButtonDropDownWidth + ButtonTextPadding;

        public enum VisualState
        {
            Normal,
            Hot,
            Pressed
        }

        public enum HotState
        {
            None,
            MainButton,
            DropDownButton,
        }

        public VisualState BorderVisualState { get; protected set; }
        public VisualState PanelVisualState { get; protected set; }
        public HotState SplitButtonHotState { get; protected set; }

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

        public bool EnabledDropDown
        {
            get
            {
                return this.ButtonStyle == RibbonButtonStyle.DropDownButton || this.ButtonStyle == RibbonButtonStyle.SplitButton;
            }
        }

        public RibbonButton()
        {
            this.ButtonStyle = RibbonButtonStyle.PushButton;
            this.ToggleGroup = -1;
            this.Toggled = false;
        }

        public override int GetBigWidth(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Group.Tab.Container.Font);
            return Math.Max((int)size.Width + 2 * ButtonTextPadding, ButtonBigIconSize + 2 * ButtonBorder);
        }

        public override int GetSmallWidth(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Group.Tab.Container.Font);
            int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
            return ButtonSmallIconSize + dropDownWidth + 2 * ButtonBorder + 2 * ButtonTextPadding + (int)size.Width;
        }

        public override int GetSmallCompactWidth(Graphics g)
        {
            int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
            return ButtonSmallIconSize + dropDownWidth + 2 * ButtonBorder;
        }

        public override int GetSuggestedWidth(Graphics g)
        {
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big: return GetBigWidth(g);
                case RibbonItemSize.Small: return GetSmallWidth(g);
                case RibbonItemSize.SmallCompact: return GetSmallCompactWidth(g);
                case RibbonItemSize.ToolStrip:
                    {
                        int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
                        return ButtonToolWidth + dropDownWidth;
                    }
                default: return 0;
            }
        }

        #region Helper Bounding Functions

        private Rectangle GetIconBounds(Rectangle itemBounds)
        {
            var settings = this.Group.Tab.Container.Settings;
            int iconSize = this.ItemSize == RibbonItemSize.Big ? ButtonBigIconSize : ButtonSmallIconSize;
            int ix = itemBounds.Left + ButtonBorder;
            int iy = itemBounds.Top + ButtonBorder;
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        ix = itemBounds.Left + (itemBounds.Width - iconSize) / 2;
                    }
                    break;
                case RibbonItemSize.ToolStrip:
                    {
                        int width = iconSize + (this.EnabledDropDown ? ButtonDropDownWidth + ButtonTextPadding : 0);
                        ix = itemBounds.Left + (itemBounds.Width - width) / 2;
                        iy = itemBounds.Top + (itemBounds.Height - iconSize) / 2;
                    }
                    break;
            }
            Rectangle iconBounds = new Rectangle(ix, iy, iconSize, iconSize);
            return iconBounds;
        }

        private Rectangle GetMainButtonBounds(Rectangle itemBounds)
        {
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        int h = ButtonBigIconSize + 2 * ButtonBorder;
                        itemBounds.Height = h;
                    }
                    break;
                case RibbonItemSize.Small:
                case RibbonItemSize.SmallCompact:
                case RibbonItemSize.ToolStrip:
                    {
                        int w = ButtonDropDownWidth + 2 * ButtonBorder;
                        itemBounds.Width -= w;
                    }
                    break;
            }
            return itemBounds;
        }

        private Rectangle GetDropDownButtonBounds(Rectangle itemBounds)
        {
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        int h = ButtonBigIconSize + 2 * ButtonBorder;
                        itemBounds.Y += h;
                        itemBounds.Height -= h;
                    }
                    break;
                case RibbonItemSize.Small:
                case RibbonItemSize.SmallCompact:
                case RibbonItemSize.ToolStrip:
                    {
                        int w = ButtonDropDownWidth + 2 * ButtonBorder;
                        itemBounds.X = itemBounds.Right - w;
                        itemBounds.Width = w;
                    }
                    break;
            }
            return itemBounds;
        }

        private Rectangle GetHotBoundsForRender(Rectangle itemBounds)
        {
            if (this.ButtonStyle == RibbonButtonStyle.SplitButton)
            {
                switch (this.SplitButtonHotState)
                {
                    case HotState.MainButton:
                        {
                            itemBounds = GetMainButtonBounds(itemBounds);
                            if (this.ItemSize == RibbonItemSize.Big)
                            {
                                itemBounds.Height += ButtonBorder / 2;
                            }
                            else
                            {
                                itemBounds.Width += ButtonBorder / 2;
                            }
                        }
                        break;
                    case HotState.DropDownButton:
                        {
                            itemBounds = GetDropDownButtonBounds(itemBounds);
                            if (this.ItemSize == RibbonItemSize.Big)
                            {
                                itemBounds.Y -= ButtonBorder / 2;
                                itemBounds.Height += ButtonBorder / 2;
                            }
                            else
                            {
                                itemBounds.X -= ButtonBorder / 2;
                                itemBounds.Width += ButtonBorder / 2;
                            }
                        }
                        break;
                }
            }
            return itemBounds;
        }

        #endregion

        #region Helper Rendering Functions

        private void RenderPanel(Graphics g, Rectangle itemBounds)
        {
            var settings = this.Group.Tab.Container.Settings;
            RibbonColorItem i1 = null;
            RibbonColorItem i2 = null;
            RibbonColorItem i3 = null;
            RibbonColorItem i4 = null;
            double ratio = 0;
            switch (this.PanelVisualState)
            {
                case VisualState.Hot:
                    {
                        i1 = settings.ButtonReleasedBackground1;
                        i2 = settings.ButtonReleasedBackground2;
                        i3 = settings.ButtonReleasedBackground3;
                        i4 = settings.ButtonReleasedBackground4;
                        ratio = (double)2 / 5;
                    }
                    break;
                case VisualState.Pressed:
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
            settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(GetHotBoundsForRender(itemBounds), 0, -1), ratio);
        }

        private void RenderBorder(Graphics g, Rectangle itemBounds)
        {
            var settings = this.Group.Tab.Container.Settings;
            RibbonColorItem outTop = settings.ButtonReleasedOuterBorderTop;
            RibbonColorItem outBottom = settings.ButtonReleasedOuterBorderBottom;
            RibbonColorItem inTop = settings.ButtonReleasedInnerBorderTop;
            RibbonColorItem inBottom = settings.ButtonReleasedInnerBorderBottom;
            if (this.ButtonStyle == RibbonButtonStyle.SplitButton && this.BorderVisualState != VisualState.Normal)
            {
                settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
            }
            switch (this.BorderVisualState)
            {
                case VisualState.Hot:
                    break;
                case VisualState.Pressed:
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
            settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, GetHotBoundsForRender(itemBounds));
        }

        private void RenderText(Graphics g, Rectangle itemBounds, Rectangle iconBounds)
        {
            var settings = this.Group.Tab.Container.Settings;
            Font font = this.Group.Tab.Container.Font;
            SizeF size = g.MeasureString(this.Name, font);
            Brush textBrush = settings.TabText.Brush;
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        int tx = itemBounds.Left + (int)(itemBounds.Width - size.Width) / 2;
                        int ty = iconBounds.Bottom + (int)(itemBounds.Bottom - iconBounds.Bottom - size.Height - ButtonDropDownHeight - ButtonTextPadding * 2) / 2;
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

        private void RenderDropDown(Graphics g, Rectangle itemBounds, Rectangle iconBounds)
        {
            var settings = this.Group.Tab.Container.Settings;
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        if (this.EnabledDropDown)
                        {
                            int dx = itemBounds.Left + (int)(itemBounds.Width - ButtonDropDownWidth) / 2;
                            int dy = itemBounds.Bottom - ButtonDropDownHeight - ButtonTextPadding;
                            Rectangle dr = new Rectangle(dx, dy, ButtonDropDownWidth, ButtonDropDownHeight);
                            settings.DrawDropDown(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
                        }
                    }
                    break;
                case RibbonItemSize.Small:
                case RibbonItemSize.SmallCompact:
                case RibbonItemSize.ToolStrip:
                    {
                        if (this.EnabledDropDown)
                        {
                            int dx = itemBounds.Right - ButtonDropDownWidth - ButtonTextPadding;
                            int dy = itemBounds.Top + (int)(itemBounds.Height - ButtonDropDownHeight) / 2;
                            Rectangle dr = new Rectangle(dx, dy, ButtonDropDownWidth, ButtonDropDownHeight);
                            settings.DrawDropDown(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
                        }
                    }
                    break;
            }
        }

        #endregion

        public override void Render(Graphics g, Rectangle itemBounds)
        {
            RenderPanel(g, itemBounds);
            Rectangle iconBounds = GetIconBounds(itemBounds);
            g.DrawImage(this.Image, iconBounds);
            RenderText(g, itemBounds, iconBounds);
            RenderDropDown(g, itemBounds, iconBounds);
            RenderBorder(g, itemBounds);
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

        public override void UpdateWithSizeDecided(Graphics g)
        {
            this.UpdatedWidth = GetSuggestedWidth(g);
        }

        public override bool OnMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            this.Group.Tab.Container.Capture(this);
            return UpdateVisualStyleToPressed();
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            HotState old = this.SplitButtonHotState;
            if (this.ButtonStyle == RibbonButtonStyle.SplitButton)
            {
                Rectangle bounds = this.Group.GetItemBounds(this);
                if (GetMainButtonBounds(bounds).Contains(e.Location))
                {
                    this.SplitButtonHotState = HotState.MainButton;
                }
                else if (GetDropDownButtonBounds(bounds).Contains(e.Location))
                {
                    this.SplitButtonHotState = HotState.DropDownButton;
                }
                else
                {
                    this.SplitButtonHotState = HotState.None;
                }
            }
            bool result = old != this.SplitButtonHotState;
            if (this.Group.Tab.Container.CapturedItem == this)
            {
                if (this.Group.GetItemFromPoint(e.Location) == this)
                {
                    return UpdateVisualStyleToPressed() || result;
                }
                else
                {
                    return UpdateVisualStyleToNormal() || result;
                }
            }
            else
            {
                return UpdateVisualStyleToHot() || result;
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

        protected virtual void DropDownExecuted()
        {
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
                case RibbonButtonStyle.DropDownButton:
                case RibbonButtonStyle.SplitButton:
                    {
                        if (this.SplitButtonHotState == HotState.DropDownButton)
                        {
                            DropDownExecuted();
                        }
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
        DropDownButton,
        SplitButton,
    }
}
