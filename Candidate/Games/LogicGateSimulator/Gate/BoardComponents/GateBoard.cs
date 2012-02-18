using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Gate.BoardComponents
{
    class GateBoard
    {
        private List<IGateBoardComponent> components = new List<IGateBoardComponent>();
        private IGateBoardComponent selectedComponent;

        public GateBoard()
        {
        }

        public IGateBoardComponent SelectedComponent
        {
            get
            {
                return this.selectedComponent;
            }
            set
            {
                if (this.components.Contains(value))
                {
                    this.selectedComponent = value;
                }
                else
                {
                    this.selectedComponent = null;
                }
            }
        }

        public void AddComponent(IGateBoardComponent component)
        {
            this.components.Add(component);
            this.selectedComponent = null;
        }

        public void RemoveComponent(IGateBoardComponent component)
        {
            this.components.Remove(component);
            this.selectedComponent = null;
        }

        public void Clear()
        {
            this.components.Clear();
        }

        public bool IsConflict(Rectangle bounds)
        {
            bounds.Inflate(-1, -1);
            return this.components.Any(c => c.Bounds.IntersectsWith(bounds));
        }

        public IGateBoardComponent GetComponent(Point position)
        {
            return this.components.Where(c => c.Bounds.Contains(position)).FirstOrDefault();
        }

        public void PaintSelectBox(Graphics g, Rectangle bounds, Color color)
        {
            bounds.Inflate(10, 10);
            using (Pen pen = new Pen(color, 3))
            {
                pen.DashStyle = DashStyle.DashDotDot;
                g.DrawRectangle(pen, bounds);
            }
        }

        public void Paint(Graphics g, Point offset)
        {
            foreach (var component in this.components)
            {
                component.Paint(g, offset);
            }
            if (this.selectedComponent != null)
            {
                Rectangle bounds = this.selectedComponent.Bounds;
                bounds.Location += new Size(offset);
                PaintSelectBox(g, bounds, Color.Blue);
            }
        }
    }

    interface IGateBoardComponent
    {
        Point Position { get; set; }
        Point[] Inputs { get; }
        Point[] Outputs { get; }
        Rectangle Bounds { get; }

        void Paint(Graphics g, Point offset);
    }
}
