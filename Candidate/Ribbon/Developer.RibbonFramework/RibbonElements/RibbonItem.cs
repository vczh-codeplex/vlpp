using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonItem
    {
        public RibbonItemContainer ItemContainer { get; internal set; }
        public RibbonToolStrip ToolStrip { get; internal set; }
        public RibbonItemSize ItemSize { get; internal set; }
        public Image BigImage { get; set; }
        public Image SmallImage { get; set; }
        public string Name { get; set; }
        public bool Enabled { get; set; }
        public Size UpdatedSize { get; protected set; }

        public RibbonItem()
        {
            this.Enabled = true;
        }

        public abstract int GetWidth(Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize);
        public abstract int GetHeight(Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize);
        public abstract void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds);
        public abstract void Update(Graphics g, RibbonThemaSettingsBase settings);

        public virtual void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            int w = GetWidth(g, settings, this.ItemSize);
            int h = GetHeight(g, settings, this.ItemSize);
            this.UpdatedSize = new Size(w, h);
        }

        public abstract bool OnMouseDown(MouseEventArgs e);
        public abstract bool OnMouseMove(MouseEventArgs e);
        public abstract bool OnMouseUp(MouseEventArgs e);
        public abstract bool OnMouseLeave(EventArgs e);
    }

    public enum RibbonItemSize
    {
        Big,
        Small,
        SmallCompact,
        ToolStrip,
        MenuItem,
    }
}
