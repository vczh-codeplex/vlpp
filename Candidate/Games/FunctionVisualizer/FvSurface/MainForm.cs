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
        const string input = "(2/exp(2*(x*x+y*y)^0.5)) * (cos(10*(x*x+y*y)^0.5)/4)";
        const string v1 = "y";
        const string v2 = "x";
        static readonly Dictionary<string, double> values = new Dictionary<string, double>
        {
        };

        private RawExpression surfaceExpression = RawExpression.Parse(input).Apply(values).Simplify();
        private double[,] surfaceValues = null;

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

        private Point3D Project(Point3D p)
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

            return new Point3D(cx, cy, scale);
        }

        private Point Project(Point3D p, int w, int h)
        {
            Point3D q = Project(p);
            int m = Math.Min(w, h);
            return new Point((int)((q.x + 0.5) * m) + (w - m) / 2, (int)((0.5 - q.y) * m) + (h - m) / 2);
        }

        private double ScaleToAxis(int i)
        {
            return 1.5 * (2 * i / (double)scale - 1);
        }

        private Point3D GetPoint(int x, int y)
        {
            double vx = ScaleToAxis(x);
            double vy = ScaleToAxis(y);
            double vz = this.surfaceValues[x, y];
            return new Point3D(vx, vy, vz);
        }

        private Point3D GetAxisPoint(int x, int y)
        {
            double vx = ScaleToAxis(x);
            double vy = ScaleToAxis(y);
            return new Point3D(vx, vy, 0);
        }

        private void Line(Point3D p1, Point3D p2, Graphics g, Pen pen)
        {
            int w = this.ClientSize.Width;
            int h = this.ClientSize.Height;
            Point q1 = Project(p1, w, h);
            Point q2 = Project(p2, w, h);
            g.DrawLine(pen, q1, q2);
        }

        private void Polygon(Point3D[] ps, Graphics g, Pen pen, Brush brush)
        {
            int w = this.ClientSize.Width;
            int h = this.ClientSize.Height;
            Point[] projectedPs = ps
                .Select(p => Project(p, w, h))
                .ToArray();
            g.FillPolygon(brush, projectedPs);
            g.DrawPolygon(pen, projectedPs);
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
        }

        private void PaintPart(Point far, Point near, Graphics g)
        {
            int vx = (near.X - far.X);
            int vy = (near.Y - far.Y);
            vx = vx < 0 ? -1 : vx > 0 ? 1 : 0;
            vy = vy < 0 ? -1 : vy > 0 ? 1 : 0;
            for (int x = far.X; x != near.X; x += vx)
            {
                for (int y = far.Y; y != near.Y; y += vy)
                {
                    int x1 = x;
                    int y1 = y;
                    int x2 = x + vx;
                    int y2 = y + vy;
                    Point3D[] points = new Point3D[] { GetPoint(x1, y1), GetPoint(x1, y2), GetPoint(x2, y2), GetPoint(x2, y1) };
                    Polygon(points, g, Pens.Black, Brushes.White);
                }
            }
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            e.Graphics.FillRectangle(Brushes.White, e.ClipRectangle);

            Point[] corners = new Point[]
            {
                new Point(0, 0),
                new Point(0, scale),
                new Point(scale, 0),
                new Point(scale, scale),
            };

            Point3D[] projectedCorners = corners
                .Select(p => Project(GetAxisPoint(p.X, p.Y)))
                .ToArray();

            int[] indices = Enumerable
                .Range(0, 4)
                .OrderBy(i => projectedCorners[i].z)
                .ToArray();

            int fl = projectedCorners[indices[0]].x < projectedCorners[indices[1]].x ? indices[0] : indices[1];
            int fr = indices[0] + indices[1] - fl;
            int nl = projectedCorners[indices[2]].x < projectedCorners[indices[3]].x ? indices[2] : indices[3];
            int nr = indices[2] + indices[3] - nl;

            if (projectedCorners[fl].x <= projectedCorners[nl].x && projectedCorners[fr].x <= projectedCorners[nr].x)
            {
                PaintPart(corners[fr], corners[nl], e.Graphics);
            }
            else if (projectedCorners[fl].x >= projectedCorners[nl].x && projectedCorners[fr].x >= projectedCorners[nr].x)
            {
                PaintPart(corners[fl], corners[nr], e.Graphics);
            }
            else
            {
                int vx = (corners[fr].X - corners[fl].X) / scale;
                int vy = (corners[fr].Y - corners[fl].Y) / scale;
                int ux = (corners[nl].X - corners[fl].X) / scale;
                int uy = (corners[nl].Y - corners[fl].Y) / scale;
                int ms = -1;
                for (int i = 0; i < scale; i++)
                {
                    int cvx1 = vx * i;
                    int cvx2 = vx * (i + 1);
                    int cvy1 = vy * i;
                    int cvy2 = vy * (i + 1);

                    Point cfl = new Point(corners[fl].X + cvx1, corners[fl].Y + cvy1);
                    Point cfr = new Point(corners[fl].X + cvx2, corners[fl].Y + cvy2);
                    Point cnl = new Point(corners[nl].X + cvx1, corners[nl].Y + cvy1);
                    Point cnr = new Point(corners[nl].X + cvx2, corners[nl].Y + cvy2);

                    Point3D pcfl = Project(GetAxisPoint(cfl.X, cfl.Y));
                    Point3D pcfr = Project(GetAxisPoint(cfr.X, cfr.Y));
                    Point3D pcnl = Project(GetAxisPoint(cnl.X, cnl.Y));
                    Point3D pcnr = Project(GetAxisPoint(cnr.X, cnr.Y));

                    ms = i;
                    break;
                }

                if (ms != 1)
                {
                    for (int u = 0; u < scale; u++)
                    {
                        Point ufl = new Point(corners[fl].X + u * ux, corners[fl].Y + u * uy);
                        Point ufr = new Point(corners[fr].X + u * ux, corners[fr].Y + u * uy);
                        Point unl = new Point(corners[fl].X + (u + 1) * ux, corners[fl].Y + (u + 1) * uy);
                        Point unr = new Point(corners[fr].X + (u + 1) * ux, corners[fr].Y + (u + 1) * uy);

                        Point cfl = new Point(ufl.X + ms * vx, ufl.Y + ms * vy);
                        Point cfr = new Point(ufl.X + (ms + 1) * vx, ufl.Y + (ms + 1) * vy);
                        Point cnl = new Point(unl.X + ms * vx, unl.Y + ms * vy);
                        Point cnr = new Point(unl.X + (ms + 1) * vx, unl.Y + (ms + 1) * vy);

                        PaintPart(ufl, cnl, e.Graphics);
                        PaintPart(ufr, cnr, e.Graphics);
                        PaintPart(cfl, cnr, e.Graphics);
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
