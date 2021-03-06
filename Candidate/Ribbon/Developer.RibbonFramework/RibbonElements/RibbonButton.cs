﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonButton : RibbonItem
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

        private bool dropDownOpening = false;

        public VisualState BorderVisualState { get; protected set; }
        public VisualState PanelVisualState { get; protected set; }
        public HotState SplitButtonHotState { get; protected set; }

        public RibbonDropDownBase DropDown { get; set; }
        public RibbonButtonStyle ButtonStyle { get; set; }
        public int ToggleGroup { get; set; }
        public bool Toggled { get; set; }

        public event EventHandler Executed;

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

        #region Helper Sizing Functions

        private int GetBigWidth(Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            return Math.Max((int)size.Width + 2 * ButtonTextPadding, ButtonBigIconSize + 2 * ButtonBorder);
        }

        private int GetSmallWidth(Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
            return ButtonSmallIconSize + dropDownWidth + 2 * ButtonBorder + 2 * ButtonTextPadding + (int)size.Width;
        }

        private int GetCompactWidth(Graphics g, RibbonThemaSettingsBase settings)
        {
            int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
            return ButtonSmallIconSize + dropDownWidth + 2 * ButtonBorder;
        }

        private int GetToolStripWidth(Graphics g, RibbonThemaSettingsBase settings)
        {
            int dropDownWidth = this.EnabledDropDown ? ButtonDropDownSpaceAdditionalWidth : 0;
            return ButtonToolWidth + dropDownWidth;
        }

        #endregion

        public override int GetWidth(Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            switch (itemSize)
            {
                case RibbonItemSize.Big: return GetBigWidth(g, settings);
                case RibbonItemSize.Small: return GetSmallWidth(g, settings);
                case RibbonItemSize.Compact: return GetCompactWidth(g, settings);
                case RibbonItemSize.ToolStrip: return GetToolStripWidth(g, settings);
                default: throw new NotSupportedException();
            }
        }

        public override int GetHeight(Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            switch (itemSize)
            {
                case RibbonItemSize.Big:
                case RibbonItemSize.Small:
                case RibbonItemSize.Compact:
                case RibbonItemSize.ToolStrip:
                    return 0;
                default: throw new NotSupportedException();
            }
        }

        #region Helper Bounding Functions

        private Rectangle GetIconBounds(Rectangle itemBounds)
        {
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
                case RibbonItemSize.Compact:
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
                case RibbonItemSize.Compact:
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
                switch (this.dropDownOpening ? HotState.DropDownButton : this.SplitButtonHotState)
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

        private void RenderPanel(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            switch (this.dropDownOpening ? VisualState.Pressed : this.PanelVisualState)
            {
                case VisualState.Hot:
                    {
                        settings.DrawHotButtonPanel(g, GetHotBoundsForRender(itemBounds), (double)2 / 5);
                    }
                    break;
                case VisualState.Pressed:
                    {
                        settings.DrawPressedButtonPanel(g, GetHotBoundsForRender(itemBounds), (double)2.25 / 5);
                    }
                    break;
            }
        }

        private void RenderBorder(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            if (this.ButtonStyle == RibbonButtonStyle.SplitButton && this.BorderVisualState != VisualState.Normal)
            {
                settings.DrawHotButtonBorder(g, itemBounds);
            }
            switch (this.dropDownOpening ? VisualState.Pressed : this.PanelVisualState)
            {
                case VisualState.Hot:
                    {
                        settings.DrawHotButtonBorder(g, GetHotBoundsForRender(itemBounds));
                    }
                    break;
                case VisualState.Pressed:
                    {
                        settings.DrawPressedButtonBorder(g, GetHotBoundsForRender(itemBounds));
                    }
                    break;
            }
        }

        private void RenderImage(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds, Rectangle iconBounds)
        {
            Image image = this.ItemSize == RibbonItemSize.Big ? this.BigImage : this.SmallImage;
            if (image != null)
            {
                if (this.Enabled)
                {
                    g.DrawImage(image, iconBounds);
                }
                else
                {
                    using (Bitmap tempImage = new Bitmap(image, iconBounds.Size))
                    {
                        ControlPaint.DrawImageDisabled(g, tempImage, iconBounds.X, iconBounds.Y, Color.Transparent);
                    }
                }
            }
        }

        private void RenderText(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds, Rectangle iconBounds)
        {
            Font font = settings.Font;
            SizeF size = g.MeasureString(this.Name, font);
            int tx = 0;
            int ty = 0;
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        tx = itemBounds.Left + (int)(itemBounds.Width - size.Width) / 2;
                        ty = iconBounds.Bottom + (int)(itemBounds.Bottom - iconBounds.Bottom - size.Height - ButtonDropDownHeight - ButtonTextPadding * 2) / 2;
                    }
                    break;
                case RibbonItemSize.Small:
                    {
                        tx = iconBounds.Right + ButtonTextPadding;
                        ty = iconBounds.Top + (int)(iconBounds.Height - size.Height) / 2;
                    }
                    break;
                default:
                    return;
            }
            if (this.Enabled)
            {
                Brush textBrush = settings.TabText.Brush;
                g.DrawString(this.Name, font, textBrush, tx, ty);
            }
            else
            {
                Rectangle r = new Rectangle(tx, ty, (int)size.Width, (int)size.Height);
                settings.DrawCarvedText(g, settings.LightBorder, settings.Border, r, this.Name, font);
            }
        }

        private void RenderDropDown(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds, Rectangle iconBounds)
        {
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        if (this.EnabledDropDown)
                        {
                            int dx = itemBounds.Left + (int)(itemBounds.Width - ButtonDropDownWidth) / 2;
                            int dy = itemBounds.Bottom - ButtonDropDownHeight - ButtonTextPadding;
                            Rectangle dr = new Rectangle(dx, dy, ButtonDropDownWidth, ButtonDropDownHeight);
                            settings.DrawDownDropDownTriangle(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
                        }
                    }
                    break;
                case RibbonItemSize.Small:
                case RibbonItemSize.Compact:
                case RibbonItemSize.ToolStrip:
                    {
                        if (this.EnabledDropDown)
                        {
                            int dx = itemBounds.Right - ButtonDropDownWidth - ButtonTextPadding;
                            int dy = itemBounds.Top + (int)(itemBounds.Height - ButtonDropDownHeight) / 2;
                            Rectangle dr = new Rectangle(dx, dy, ButtonDropDownWidth, ButtonDropDownHeight);
                            settings.DrawDownDropDownTriangle(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
                        }
                    }
                    break;
            }
        }

        #endregion

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            RenderPanel(g, settings, itemBounds);
            Rectangle iconBounds = GetIconBounds(itemBounds);
            RenderImage(g, settings, itemBounds, iconBounds);
            RenderText(g, settings, itemBounds, iconBounds);
            RenderDropDown(g, settings, itemBounds, iconBounds);
            RenderBorder(g, settings, itemBounds);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
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
            this.ItemContainer.Services.Capture(this);
            return UpdateVisualStyleToPressed();
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            HotState old = this.SplitButtonHotState;
            if (this.ButtonStyle == RibbonButtonStyle.SplitButton)
            {
                Rectangle bounds = this.ItemContainer.GetItemBounds(this);
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
            if (this.ItemContainer.Services.CapturedItem == this)
            {
                if (this.ItemContainer.GetItemFromPoint(e.Location) == this)
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
            this.ItemContainer.Services.Capture(null);
            if (this.ItemContainer.GetItemFromPoint(e.Location) == this)
            {
                OnExecuted();
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

        protected virtual void OnDropDownExecuted()
        {
            if (this.DropDown != null)
            {
                Rectangle bounds = this.ItemContainer.GetItemBounds(this);
                this.DropDown.Open(this, new Point(0, bounds.Height));
                this.dropDownOpening = true;
                this.DropDown.DropDownHost.Closed += new ToolStripDropDownClosedEventHandler(DropDownHost_Closed);
            }
        }

        private void DropDownHost_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            RibbonDropDownHost host = (RibbonDropDownHost)sender;
            host.Closed -= new ToolStripDropDownClosedEventHandler(DropDownHost_Closed);
            this.dropDownOpening = false;
            UpdateVisualStyleToNormal();
            this.ItemContainer.Services.RefreshItemContainer();
        }

        protected virtual void OnExecuted()
        {
            bool dropDownExecuted = false;
            switch (this.ButtonStyle)
            {
                case RibbonButtonStyle.GroupedToggleButton:
                    {
                        if (!this.Toggled)
                        {
                            foreach (var item in this.ItemContainer.Items)
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
                    {
                        OnDropDownExecuted();
                        dropDownExecuted = true;
                    }
                    break;
                case RibbonButtonStyle.SplitButton:
                    {
                        if (this.SplitButtonHotState == HotState.DropDownButton)
                        {
                            OnDropDownExecuted();
                            dropDownExecuted = true;
                        }
                    }
                    break;
            }
            if (!dropDownExecuted)
            {
                if (this.Executed != null)
                {
                    this.Executed(this, new EventArgs());
                }
                this.ItemContainer.Services.ItemExecuted(this);
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
