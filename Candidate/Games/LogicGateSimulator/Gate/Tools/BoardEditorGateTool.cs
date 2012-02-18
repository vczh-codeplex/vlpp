using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.Tools
{
    class BoardEditorGateTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
        {
            private Point location = new Point(-100, -100);
            private BoardEditorPanel panel;
            private PrimitiveGates gate;

            public Command(PrimitiveGates gate)
            {
                this.gate = gate;
            }

            public void Attach(BoardEditorPanel panel)
            {
                this.panel = panel;
            }

            public void Detach(BoardEditorPanel panel)
            {
                this.panel = null;
            }

            public void OnMouseDown(System.Windows.Forms.MouseEventArgs e)
            {
            }

            public void OnMouseUp(System.Windows.Forms.MouseEventArgs e)
            {
            }

            public void OnMouseMove(System.Windows.Forms.MouseEventArgs e)
            {
                this.location = e.Location;
                this.panel.Refresh();
            }

            public void OnPaint(System.Windows.Forms.PaintEventArgs e)
            {
                Point p = this.location + new Size(this.panel.DisplayOffset);
                p.X -= p.X % BoardEditorPanel.GridSize;
                p.Y -= p.Y % BoardEditorPanel.GridSize;
                p -= new Size(this.panel.DisplayOffset);
                GatePainter.PaintGate(e.Graphics, BoardEditorPanel.GridSize, p, this.gate);
            }
        }

        private PrimitiveGates gate;

        public Bitmap Icon
        {
            get
            {
                return ToolIcons.Gate;
            }
        }

        public string Name
        {
            get
            {
                return this.gate.ToString().ToUpper() + " Gate";
            }
        }

        public BoardEditorGateTool(PrimitiveGates gate)
        {
            this.gate = gate;
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new Command(this.gate);
        }
    }

    enum PrimitiveGates
    {
        And,
        Or,
        Not,
        Xor,
        Xnor,
        Nand,
        Nor,
    }

    static class GatePainter
    {
        private static void PaintOutputInternal(Graphics g, Pen pen, float gridSize, PointF position, float length, bool reverseOutput)
        {
            if (reverseOutput)
            {
                float d = gridSize / 2;
                g.DrawEllipse(pen, position.X, position.Y - d / 2, d, d);
                g.DrawLine(pen, position.X + d, position.Y, position.X + length, position.Y);
            }
            else
            {
                g.DrawLine(pen, position.X, position.Y, position.X + length, position.Y);
            }
        }

        private static void PaintOutput(Graphics g, Pen pen, float size, float gridSize, PointF position, bool reverseOutput)
        {
            PaintOutputInternal(g, pen, gridSize, new PointF(position.X + size, position.Y + size / 2), gridSize * 3 / 2, reverseOutput);
        }

        private static void PaintAndBody(Graphics g, Pen pen, float gridSize, PointF position, bool reverseOutput)
        {
            float size = gridSize * 3;
            position -= new SizeF(gridSize, gridSize);

            g.DrawLine(pen, position.X, position.Y, position.X + gridSize * 3 / 2 + 1, position.Y);
            g.DrawLine(pen, position.X, position.Y + size, position.X + gridSize * 3 / 2 + 1, position.Y + size);
            g.DrawLine(pen, position.X, position.Y, position.X, position.Y + size);
            g.DrawArc(pen, new RectangleF(position.X, position.Y, size, size), -90, 180);

            g.DrawLine(pen, position.X - gridSize * 3 / 2, position.Y + gridSize / 2, position.X + 1, position.Y + gridSize / 2);
            g.DrawLine(pen, position.X - gridSize * 3 / 2, position.Y + size - gridSize / 2, position.X + 1, position.Y + size - gridSize / 2);
            PaintOutput(g, pen, size, gridSize, position, reverseOutput);
        }

        private static void Solve(float a, float b, float c, float d, float e, float f, out float x, out float y)
        {
            float z = b * d - a * e;
            x = (b * f - c * e) / z;
            y = (c * d - a * f) / z;
        }

        private static void CalculateEllipse(PointF p1, PointF p2, PointF p3, out PointF o, out float r, out float a, out float da)
        {
            PointF pc1 = new PointF((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2);
            PointF pc2 = new PointF((p3.X + p2.X) / 2, (p3.Y + p2.Y) / 2);
            PointF vc1 = new PointF(-(p1.Y - p2.Y), (p1.X - p2.X));
            PointF vc2 = new PointF(-(p3.Y - p2.Y), (p3.X - p2.X));

            float dv1, dv2;
            Solve(vc1.X, -vc2.X, pc2.X - pc1.X, vc1.Y, -vc2.Y, pc2.Y - pc1.Y, out dv1, out dv2);
            float ox1 = pc1.X + vc1.X * dv1;
            float oy1 = pc1.Y + vc1.Y * dv1;
            float ox2 = pc2.X + vc2.X * dv2;
            float oy2 = pc2.Y + vc2.Y * dv2;
            float ox = (ox1 + ox2) / 2;
            float oy = (oy1 + oy2) / 2;
            o = new PointF(ox, oy);
            r = (float)Math.Sqrt((p2.X - ox) * (p2.X - ox) + (p2.Y - oy) * (p2.Y - oy));

            float a1 = (float)(Math.Atan2(p1.Y - oy, p1.X - ox) * 180 / Math.PI);
            float a2 = (float)(Math.Atan2(p3.Y - oy, p3.X - ox) * 180 / Math.PI);
            a = a1;
            da = a2 - a1;
        }

        private static void PaintArc(Graphics g, Pen pen, PointF p1, PointF p2, PointF p3)
        {
            PointF o;
            float r, a, da;
            CalculateEllipse(p1, p2, p3, out o, out r, out a, out da);
            g.DrawArc(pen, new RectangleF(o - new SizeF(r, r), new SizeF(r * 2, r * 2)), a, da);
        }

        private static void PaintArc(Graphics g, Pen pen, PointF p1, PointF p2, PointF p3, float y, out float x)
        {
            PointF o;
            float r, a, da;
            CalculateEllipse(p1, p2, p3, out o, out r, out a, out da);
            x = o.X + (float)Math.Sqrt(r * r - (o.Y - y) * (o.Y - y));
            g.DrawArc(pen, new RectangleF(o - new SizeF(r, r), new SizeF(r * 2, r * 2)), a, da);
        }

        private static void PaintOrBodyInternal(Graphics g, Pen pen, float gridSize, PointF position, bool reverseOutput, bool doubleLeftSide)
        {
            float size = gridSize * 3;
            position -= new SizeF(gridSize, gridSize);

            float x;
            PaintArc(g, pen,
                position,
                position + new SizeF(gridSize / 2, size / 2),
                position + new SizeF(0, size),
                position.Y + gridSize / 2,
                out x
                );
            if (doubleLeftSide)
            {
                PaintArc(g, pen,
                    position + new SizeF(-gridSize / 2, 0),
                    position + new SizeF(0, size / 2),
                    position + new SizeF(-gridSize / 2, size)
                    );
            }
            PaintArc(g, pen,
                position,
                position + new SizeF(gridSize * 2, gridSize / 2),
                position + new SizeF(size, size / 2)
                );
            PaintArc(g, pen,
                position + new SizeF(0, size),
                position + new SizeF(gridSize * 2, gridSize * 5 / 2),
                position + new SizeF(size, size / 2)
                );

            g.DrawLine(pen, position.X - gridSize * 3 / 2, position.Y + gridSize / 2, x, position.Y + gridSize / 2);
            g.DrawLine(pen, position.X - gridSize * 3 / 2, position.Y + size - gridSize / 2, x, position.Y + size - gridSize / 2);
            PaintOutput(g, pen, size, gridSize, position, reverseOutput);
        }

        private static void PaintOrBody(Graphics g, Pen pen, float gridSize, PointF position, bool reverseOutput)
        {
            PaintOrBodyInternal(g, pen, gridSize, position, reverseOutput, false);
        }

        private static void PaintXorBody(Graphics g, Pen pen, float gridSize, PointF position, bool reverseOutput)
        {
            PaintOrBodyInternal(g, pen, gridSize, position, reverseOutput, true);
        }

        private static void PaintNotBody(Graphics g, Pen pen, float gridSize, PointF position)
        {
            float size = gridSize * 3;
            position -= new SizeF(gridSize, gridSize);

            g.DrawLine(pen, position, position + new SizeF(0, size));
            g.DrawLine(pen, position, position + new SizeF(size, size / 2));
            g.DrawLine(pen, position + new SizeF(0, size), position + new SizeF(size, size / 2));

            g.DrawLine(pen, position.X - gridSize * 3 / 2, position.Y + size / 2, position.X + 1, position.Y + size / 2);
            PaintOutput(g, pen, size, gridSize, position, true);
        }

        public static void PaintGate(Graphics g, float gridSize, PointF position, PrimitiveGates gate)
        {
            using (Pen pen = new Pen(Color.Black, 3))
            {
                switch (gate)
                {
                    case PrimitiveGates.And:
                        PaintAndBody(g, pen, gridSize, position, false);
                        break;
                    case PrimitiveGates.Nand:
                        PaintAndBody(g, pen, gridSize, position, true);
                        break;
                    case PrimitiveGates.Or:
                        PaintOrBody(g, pen, gridSize, position, false);
                        break;
                    case PrimitiveGates.Nor:
                        PaintOrBody(g, pen, gridSize, position, true);
                        break;
                    case PrimitiveGates.Xor:
                        PaintXorBody(g, pen, gridSize, position, false);
                        break;
                    case PrimitiveGates.Xnor:
                        PaintXorBody(g, pen, gridSize, position, true);
                        break;
                    case PrimitiveGates.Not:
                        PaintNotBody(g, pen, gridSize, position);
                        break;
                }
            }
        }
    }
}
