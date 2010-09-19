using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TokenizerBuilder.Shape
{
    public class ArrowShape : ShapeBase
    {
        public const int ArrowShapeProprity = 1;

        public string Name { get; set; }
        public StateShape Start { get; set; }
        public StateShape End { get; set; }
        public Point ControlPoint { get; set; }
        public int Radius { get; set; }

        public ArrowShape()
        {
            this.Name = "";
            this.Radius = 5;
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
                return new Rectangle(this.ControlPoint.X - this.Radius - 5, this.ControlPoint.Y - this.Radius - 5, this.Radius * 2 + 10, this.Radius * 2 + 10);
            }
        }

        public override bool IsInShape(Graphics g, Point p)
        {
            return (p.X - this.ControlPoint.X) * (p.X - this.ControlPoint.X) + (p.Y - this.ControlPoint.Y) * (p.Y - this.ControlPoint.Y) <= this.Radius * this.Radius;
        }

        public override void Draw(Graphics g, Font font, Size offset)
        {
            float vx = 0;
            float vy = 0;
            if (this.Start == this.End)
            {
                int x = (this.ControlPoint.X + this.Start.Position.X) / 2;
                int y = (this.ControlPoint.Y + this.Start.Position.Y) / 2;
                int r = (int)Math.Round(Math.Sqrt(Math.Pow(this.ControlPoint.X - this.Start.Position.X, 2) + Math.Pow(this.ControlPoint.Y - this.Start.Position.Y, 2))) / 2;
                g.DrawEllipse(Pens.LightBlue, x - r + offset.Width, y - r + offset.Height, r * 2, r * 2);

                vx = this.Start.Position.Y - this.ControlPoint.Y;
                vy = this.ControlPoint.X - this.Start.Position.X;
            }
            else
            {
                g.DrawCurve(Pens.LightBlue, new Point[] { this.Start.Position + offset, this.ControlPoint + offset, this.End.Position + offset }, 0.8f);

                vx = this.End.Position.X - this.Start.Position.X;
                vy = this.End.Position.Y - this.Start.Position.Y;
            }

            Rectangle handle = new Rectangle(this.ControlPoint.X - this.Radius + offset.Width, this.ControlPoint.Y - this.Radius + offset.Height, this.Radius * 2, this.Radius * 2);
            g.FillEllipse(Brushes.White, handle);
            g.DrawEllipse(Pens.LightBlue, handle);

            {
                float v = (float)Math.Sqrt(vx * vx + vy * vy);
                float sinv = vy / v;
                float cosv = vx / v;

                PointF[] points = new PointF[3];
                points[0].X = -0.5f;
                points[0].Y = -(float)Math.Sqrt(3) / 2;
                points[1].X = 1.0f;
                points[1].Y = 0.0f;
                points[2].X = -0.5f;
                points[2].Y = (float)Math.Sqrt(3) / 2;
                for (int i = 0; i < 3; i++)
                {
                    float px = cosv * points[i].X - sinv * points[i].Y;
                    float py = sinv * points[i].X + cosv * points[i].Y;
                    points[i].X = this.Radius * px + offset.Width + this.ControlPoint.X;
                    points[i].Y = this.Radius * py + offset.Height + this.ControlPoint.Y;
                }

                g.FillPolygon(Brushes.LightBlue, points);
            }

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
