using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TokenizerBuilder.Shape
{
    public enum StateType
    {
        Normal,
        Start,
        Finish,
        Partial
    }

    public class StateShape : ShapeBase
    {
        public const int StateShapeProprity = 0;

        public string Name { get; set; }
        public Point Position { get; set; }
        public int Radius { get; set; }
        public StateType Type { get; set; }

        public List<ArrowShape> OutArrows { get; set; }
        public List<ArrowShape> InArrows { get; set; }

        public StateShape()
        {
            this.Name = "";
            this.Radius = 32;
            this.Type = StateType.Normal;
            this.OutArrows = new List<ArrowShape>();
            this.InArrows = new List<ArrowShape>();
        }

        public override int Priority
        {
            get
            {
                return 0;
            }
        }

        public override Rectangle SelectionBounds
        {
            get
            {
                return new Rectangle(this.Position.X - this.Radius - 5, this.Position.Y - this.Radius - 5, this.Radius * 2 + 10, this.Radius * 2 + 10);
            }
        }

        public override bool IsInShape(Graphics g, Point p)
        {
            return (p.X - this.Position.X) * (p.X - this.Position.X) + (p.Y - this.Position.Y) * (p.Y - this.Position.Y) <= this.Radius * this.Radius;
        }

        public override void Draw(Graphics g, Font font, Size offset)
        {
            Rectangle outer = new Rectangle(this.Position.X - this.Radius - 5 + offset.Width, this.Position.Y - this.Radius - 5 + offset.Height, this.Radius * 2 + 10, this.Radius * 2 + 10);
            Rectangle inner = new Rectangle(this.Position.X - this.Radius + offset.Width, this.Position.Y - this.Radius + offset.Height, this.Radius * 2, this.Radius * 2);
            switch (this.Type)
            {
                case StateType.Start:
                    g.FillEllipse(Brushes.White, outer);
                    g.DrawEllipse(Pens.LightGreen, outer);
                    g.DrawEllipse(Pens.LightGreen, inner);
                    break;
                case StateType.Finish:
                    g.FillEllipse(Brushes.LightGreen, outer);
                    g.FillEllipse(Brushes.White, inner);
                    break;
                case StateType.Normal:
                    g.FillEllipse(Brushes.White, inner);
                    g.DrawEllipse(Pens.LightGreen, inner);
                    break;
                case StateType.Partial:
                    g.FillEllipse(Brushes.LightGreen, inner);
                    break;
            }

            SizeF size = g.MeasureString(this.Name, font);
            g.DrawString(this.Name, font, Brushes.Black, this.Position.X - size.Width / 2 + offset.Width, this.Position.Y - size.Height / 2 + offset.Height);
        }

        public override void Move(Size size)
        {
            this.Position += size;
        }
    }
}
