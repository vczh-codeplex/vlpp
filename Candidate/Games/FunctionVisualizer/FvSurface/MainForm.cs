using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FvCalculation;

namespace FvSurface
{
    public partial class MainForm : Form
    {
        struct Point3D
        {
            public double x;
            public double y;
            public double z;

            public Point3D(double _x, double _y, double _z)
            {
                x = _x;
                y = _y;
                z = _z;
            }

            public static Point3D operator +(Point3D p, Point3D q)
            {
                return new Point3D(p.x + q.x, p.y + q.y, p.z + q.z);
            }

            public static Point3D operator -(Point3D p, Point3D q)
            {
                return new Point3D(p.x - q.x, p.y - q.y, p.z - q.z);
            }

            public static Point3D operator *(Point3D p, double s)
            {
                return new Point3D(p.x * s, p.y * s, p.z * s);
            }

            public static Point3D operator /(Point3D p, double s)
            {
                return new Point3D(p.x / s, p.y / s, p.z / s);
            }

            public static double Dot(Point3D p, Point3D q)
            {
                return p.x * q.x + p.y * q.y + p.z * q.z;
            }

            public static Point3D Mul(Point3D p, Point3D q)
            {
                return new Point3D(
                    p.y * q.z - p.z * q.y,
                    p.z * q.x - p.x * q.z,
                    p.x * q.y - p.y * q.x
                    );
            }

            public static double Cos(Point3D p, Point3D q)
            {
                return Dot(p, q) / (p.Distance * q.Distance);
            }

            public double Distance
            {
                get
                {
                    return Math.Sqrt(x * x + y * y + z * z);
                }
            }
        }

        const int scale = 60;
        const string input = "(sin(x*10)+cos(y*10))/10";
        const string v1 = "y";
        const string v2 = "x";
        static readonly Dictionary<string, double> values = new Dictionary<string, double>
        {
        };

        private RawExpression surfaceExpression = RawExpression.Parse(input).Apply(values).Simplify();
        private double[,] surfaceValues = null;
        private double surfaceMin = 0;
        private double surfaceMax = 0;

        private double viewAlpha = Math.PI / 4;
        private double viewBeta = Math.PI / 4;
        private double viewDistance = 3;

        private bool dragging = false;
        private Point draggingPosition;

        private static Point3D ViewPoint(double alpha, double beta, double distance)
        {
            double sa = Math.Sin(alpha);
            double ca = Math.Cos(alpha);
            double sb = Math.Sin(beta);
            double cb = Math.Cos(beta);
            Point3D v = new Point3D(distance * ca * cb, distance * sa * cb, distance * sb);
            return v;
        }

        private Point Convert(Point3D p, int w, int h)
        {
            Point3D v = ViewPoint(viewAlpha, viewBeta, viewDistance);
            Point3D vy = ViewPoint(viewAlpha, viewBeta + Math.PI / 2, 1);
            Point3D vx = Point3D.Mul(vy, v);
            vx = vx / vx.Distance;

            Point3D vp = p - v;
            double scale = v.Distance / vp.Distance;
            Point3D proj = vp * scale + v;
            double cy = Point3D.Dot(proj, vy) / viewDistance;
            double cx = Point3D.Dot(proj, vx) / viewDistance;

            int m = Math.Min(w, h);
            return new Point((int)((cx + 0.5) * m) + (w - m) / 2, (int)((0.5 - cy) * m) + (h - m) / 2);
        }

        private double ScaleToAxis(int i)
        {
            return 2 * i / (double)scale - 1;
        }

        private Point3D GetPoint(int x, int y)
        {
            double vx = ScaleToAxis(x);
            double vy = ScaleToAxis(y);
            double vz = this.surfaceValues[x, y];
            return new Point3D(vx, vy, vz);
        }

        private void Line(Point3D p1, Point3D p2, Graphics g, Pen pen)
        {
            int w = this.ClientSize.Width;
            int h = this.ClientSize.Height;
            Point q1 = Convert(p1, w, h);
            Point q2 = Convert(p2, w, h);
            g.DrawLine(pen, q1, q2);
        }

        public MainForm()
        {
            InitializeComponent();
            DoubleBuffered = true;
            this.Text = string.Format(this.Text, v1, v2, input);

            surfaceValues = new double[scale + 1, scale + 1];
            for (int x = 0; x <= scale; x++)
            {
                for (int y = 0; y <= scale; y++)
                {
                    double vx = ScaleToAxis(x);
                    double vy = ScaleToAxis(y);
                    this.surfaceValues[x, y] = surfaceExpression.Execute(new Dictionary<string, double> { { v1, vx }, { v2, vy } });
                }
            }
            this.surfaceMin = this.surfaceValues.Cast<double>().Where(d => !double.IsNaN(d)).Min();
            this.surfaceMax = this.surfaceValues.Cast<double>().Where(d => !double.IsNaN(d)).Max();
            for (int x = 0; x <= scale; x++)
            {
                for (int y = 0; y <= scale; y++)
                {
                    //this.surfaceValues[x, y] = 2 * (this.surfaceValues[x, y] - this.surfaceMin) / (this.surfaceMax - this.surfaceMin) - 1;
                }
            }
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            e.Graphics.FillRectangle(Brushes.White, e.ClipRectangle);

            for (int x = 0; x <= scale; x++)
            {
                for (int y = 0; y < scale; y++)
                {
                    Point3D p1 = GetPoint(x, y);
                    Point3D p2 = GetPoint(x, y + 1);
                    if (!double.IsNaN(p1.z) && !double.IsNaN(p2.z))
                    {
                        Line(p1, p2, e.Graphics, Pens.Black);
                    }
                }
            }
            for (int y = 0; y <= scale; y++)
            {
                for (int x = 0; x < scale; x++)
                {
                    Point3D p1 = GetPoint(x, y);
                    Point3D p2 = GetPoint(x + 1, y);
                    if (!double.IsNaN(p1.z) && !double.IsNaN(p2.z))
                    {
                        Line(p1, p2, e.Graphics, Pens.Black);
                    }
                }
            }

            Line(new Point3D(0, 0, 0), new Point3D(1, 0, 0), e.Graphics, Pens.Red);
            Line(new Point3D(0, 0, 0), new Point3D(0, 1, 0), e.Graphics, Pens.Green);
            Line(new Point3D(0, 0, 0), new Point3D(0, 0, 1), e.Graphics, Pens.Blue);
        }

        private void timerRefresh_Tick(object sender, EventArgs e)
        {
            Refresh();
        }

        private void MainForm_MouseDown(object sender, MouseEventArgs e)
        {
            dragging = true;
            draggingPosition = e.Location;
        }

        private void MainForm_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                int dx = e.X - draggingPosition.X;
                int dy = e.Y - draggingPosition.Y;
                viewAlpha -= dx * Math.PI / 250;
                viewBeta += dy * Math.PI / 250;
                draggingPosition = e.Location;
            }
        }

        private void MainForm_MouseUp(object sender, MouseEventArgs e)
        {
            dragging = false;
        }
    }
}
