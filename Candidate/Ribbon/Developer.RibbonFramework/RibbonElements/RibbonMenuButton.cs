using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonMenuButton : RibbonMenuItem
    {
        public int MenuButtonLeftPadding = 5;
        public int MenuDropDownArrowArea = 10;
        public const int MenuDropDownWidth = 3;
        public const int MenuDropDownHeight = 5;
        public const int MenuIconPadding = 4;

        public RibbonDropDownBase DropDown { get; set; }
        public bool Hot { get; protected set; }

        public event EventHandler Executed;

        public RibbonMenuButton()
        {
            this.Hot = false;
        }

        public override int GetHeight(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            if (itemSize != RibbonItemSize.MenuItem) throw new NotSupportedException();
            return this.UpdatedMenuIconAreaSize.Height;
        }

        public override int GetWidth(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            if (itemSize != RibbonItemSize.MenuItem) throw new NotSupportedException();
            SizeF size = g.MeasureString(this.Name, settings.Font);
            return this.UpdatedMenuIconAreaSize.Width + 2 * MenuButtonLeftPadding + (int)size.Width + (this.DropDown != null ? MenuDropDownArrowArea : 0);
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            if (this.Hot)
            {
                RibbonColorItem i1 = settings.ButtonReleasedBackground1;
                RibbonColorItem i2 = settings.ButtonReleasedBackground2;
                RibbonColorItem i3 = settings.ButtonReleasedBackground3;
                RibbonColorItem i4 = settings.ButtonReleasedBackground4;
                settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(itemBounds, 0, -1), 0.5);

                RibbonColorItem outTop = settings.ButtonReleasedOuterBorderTop;
                RibbonColorItem outBottom = settings.ButtonReleasedOuterBorderBottom;
                RibbonColorItem inTop = settings.ButtonReleasedInnerBorderTop;
                RibbonColorItem inBottom = settings.ButtonReleasedInnerBorderBottom;
                settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
            }
            if (this.SmallImage != null)
            {
                Rectangle r = GetIconBounds(itemBounds);
                int x = r.Left + (r.Width - this.SmallImage.Width) / 2;
                int y = r.Top + (r.Height - this.SmallImage.Height) / 2;
                if (this.Enabled)
                {
                    g.DrawImage(this.SmallImage, new Rectangle(x, y, this.SmallImage.Width, this.SmallImage.Height));
                }
                else
                {
                    ControlPaint.DrawImageDisabled(g, this.SmallImage, x, y, Color.Transparent);
                }
            }
            {
                Rectangle r = GetPanelBounds(itemBounds);
                SizeF size = g.MeasureString(this.Name, settings.Font);
                int tx = r.Left + MenuButtonLeftPadding;
                int ty = r.Top + (int)(r.Height - size.Height) / 2;
                Font font = settings.Font;

                if (this.Enabled)
                {
                    g.DrawString(this.Name, font, settings.TabText.Brush, tx, ty);
                }
                else
                {
                    Rectangle tr = new Rectangle(tx, ty, (int)size.Width, (int)size.Height);
                    settings.DrawCarvedText(g, settings.MenuLightText, settings.MenuDarkText, tr, this.Name, font);
                }
            }
            if (this.DropDown != null)
            {
                int x = itemBounds.Right - MenuDropDownArrowArea + (MenuDropDownArrowArea - MenuDropDownWidth) / 2;
                int y = itemBounds.Top + (itemBounds.Height - MenuDropDownHeight) / 2;
                Rectangle dr = new Rectangle(x, y, MenuDropDownWidth, MenuDropDownHeight);
                settings.DrawRightDropDownTriangle(g, settings.ButtonDropDownLight, settings.ButtonDropDownDark, dr);
            }
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            if (this.SmallImage != null)
            {
                int w = this.SmallImage.Width + 2 * MenuIconPadding;
                int h = this.SmallImage.Height + 2 * MenuIconPadding;
                Size s = this.UpdatedMenuIconAreaSize;
                this.UpdatedMenuIconAreaSize = new Size(Math.Max(s.Width, w), Math.Max(s.Height, h));
            }
        }

        public override bool OnMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            return false;
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            if (!this.Hot)
            {
                this.Hot = true;
                foreach (var item in this.ItemContainer.Items)
                {
                    RibbonMenuButton menuItem = item as RibbonMenuButton;
                    if (menuItem != null && menuItem != this && menuItem.DropDown != null)
                    {
                        menuItem.DropDown.Close();
                    }
                }
                if (this.DropDown != null)
                {
                    Rectangle itemBounds = this.ItemContainer.GetItemBounds(this);
                    this.DropDown.Open(this, new Point(itemBounds.Width, 0));
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        public override bool OnMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            OnExecuted();
            return false;
        }

        public override bool OnMouseLeave(EventArgs e)
        {
            if (this.Hot)
            {
                this.Hot = false;
                return true;
            }
            else
            {
                return false;
            }
        }

        protected virtual void OnExecuted()
        {
            if (this.DropDown == null)
            {
                if (this.Executed != null)
                {
                    this.Executed(this, new EventArgs());
                }
                this.ItemContainer.Services.ItemExecuted(this);
            }
        }
    }
}
