using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TokenizerBuilder.Shape
{
    public abstract class ShapeBase
    {
        public abstract int Priority { get; }
        public abstract Rectangle SelectionBounds { get; }
        public abstract bool IsInShape(Graphics g, Point p);
        public abstract void Draw(Graphics g, Font font, Size offset);
        public abstract void Move(Size size);
    }

    public class ShapeManager
    {
        public Dictionary<int, List<ShapeBase>> Shapes { get; private set; }

        public ShapeManager()
        {
            this.Shapes = new Dictionary<int, List<ShapeBase>>();
        }

        public void AddShape(ShapeBase shape)
        {
            if (!this.Shapes.ContainsKey(shape.Priority))
            {
                this.Shapes.Add(shape.Priority, new List<ShapeBase>());
            }
            this.Shapes[shape.Priority].Add(shape);
        }

        public void RemoveShape(ShapeBase shape)
        {
            this.Shapes[shape.Priority].Remove(shape);
        }

        public ShapeBase Pick(Graphics g, Point p)
        {
            foreach (var pair in this.Shapes)
            {
                foreach (var shape in pair.Value.Reverse<ShapeBase>())
                {
                    if (shape.IsInShape(g, p))
                    {
                        return shape;
                    }
                }
            }
            return null;
        }

        public void Draw(Graphics g, Font font, Size offset)
        {
            foreach (var pair in this.Shapes.Reverse())
            {
                foreach (var shape in pair.Value)
                {
                    shape.Draw(g, font, offset);
                }
            }
        }
    }
}
