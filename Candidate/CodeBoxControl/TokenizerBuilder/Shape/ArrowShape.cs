using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TokenizerBuilder.Shape
{
    public class ArrowShape : ShapeBase
    {
        public string Name { get; set; }
        public StateShape Start { get; set; }
        public StateShape End { get; set; }
        public Point ControlPoint { get; set; }

        public ArrowShape()
        {
            this.Name = "";
        }

        public override int Priority
        {
            get
            {
                return 1;
            }
        }

        public override Rectangle SelectionBounds
        {
            get
            {
                return new Rectangle(this.ControlPoint.X - 10, this.ControlPoint.Y - 10, 20, 20);
            }
        }

        public override bool IsInShape(Graphics g, Point p)
        {
            return (p.X - this.ControlPoint.X) * (p.X - this.ControlPoint.X) + (p.Y - this.ControlPoint.Y) * (p.Y - this.ControlPoint.Y) <= 25;
        }

        public override void Draw(Graphics g, Font font, Size offset)
        {
            if (this.Start == this.End)
            {
                int x = (this.ControlPoint.X + this.Start.Position.X) / 2;
                int y = (this.ControlPoint.Y + this.Start.Position.Y) / 2;
                int r = (int)Math.Round(Math.Sqrt(Math.Pow(this.ControlPoint.X - this.Start.Position.X, 2) + Math.Pow(this.ControlPoint.Y - this.Start.Position.Y, 2))) / 2;
                g.DrawEllipse(Pens.Gray, x - r + offset.Width, y - r + offset.Height, r * 2, r * 2);
            }
            else
            {
                g.DrawCurve(Pens.Gray, new Point[] { this.Start.Position + offset, this.ControlPoint + offset, this.End.Position + offset });
            }

            Rectangle handle = new Rectangle(this.ControlPoint.X - 5 + offset.Width, this.ControlPoint.Y - 5 + offset.Height, 10, 10);
            g.FillRectangle(Brushes.White, handle);
            g.DrawEllipse(Pens.Gray, handle);

            SizeF size = g.MeasureString(this.Name, font);
            g.DrawString(this.Name, font, Brushes.Black, this.ControlPoint.X - size.Width / 2 + offset.Width, this.ControlPoint.Y - size.Height * 3 / 2 + offset.Height);
        }

        public override void Move(Size size)
        {
            this.ControlPoint += size;
        }

        private Point? CalculateCenter()
        {
            int Xa = (this.ControlPoint.X + this.Start.Position.X) / 2;
            int Ya = (this.ControlPoint.Y + this.Start.Position.Y) / 2;
            int Xb = (this.ControlPoint.X + this.End.Position.X) / 2;
            int Yb = (this.ControlPoint.Y + this.End.Position.Y) / 2;

            int Xav = this.ControlPoint.Y - this.Start.Position.Y;
            int Yav = this.Start.Position.X - this.ControlPoint.X;
            int Xbv = this.ControlPoint.Y - this.End.Position.Y;
            int Ybv = this.End.Position.X - this.ControlPoint.X;

            int A = Yav * Xa - Xav * Ya;
            int B = Ybv * Xb - Xbv * Yb;
            int C = Xbv * Yav - Xav * Ybv;

            if (C == 0)
            {
                return null;
            }
            else
            {
                int x = (Xbv * A - Xav * B) / C;
                int y = (Ybv * A - Yav * B) / C;
                return new Point(x, y);
            }
        }
    }
}
