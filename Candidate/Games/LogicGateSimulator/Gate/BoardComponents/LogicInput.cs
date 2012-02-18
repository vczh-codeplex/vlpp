using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.BoardComponents
{
    class LogicInput : IGateBoardComponent
    {
        public bool InputValue { get; set; }
        public Point Position { get; set; }

        public Point[] Inputs
        {
            get
            {
                return new Point[0];
            }
        }

        public Point[] Outputs
        {
            get
            {
                Point o;
                GatePainter.GetInputOutput(BoardEditorPanel.GridSize, this.Position, out o);
                return new Point[] { o };
            }
        }

        public Rectangle Bounds
        {
            get
            {
                return GatePainter.GetInputBounds(BoardEditorPanel.GridSize, this.Position);
            }
        }

        public void Paint(Graphics g, Point offset, bool alert)
        {
            Point p = this.Position + new Size(offset);
            GatePainter.PaintInput(g, BoardEditorPanel.GridSize, p, this.InputValue, alert);
        }
    }
}
