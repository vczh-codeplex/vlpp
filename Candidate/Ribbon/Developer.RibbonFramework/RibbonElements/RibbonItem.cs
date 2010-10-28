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
        public RibbonGroup Group { get; internal set; }
        public RibbonItemSize ItemSize { get; internal set; }
        public Image Image { get; set; }
        public string Name { get; set; }

        public abstract Size GetSize(Graphics g);
        public abstract void Render(Graphics g, Rectangle itemBounds);
        public abstract void Update(Graphics g);

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
    }
}
