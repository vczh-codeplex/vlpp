using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.BoardComponents
{
    class LogicChipOutput : GateBoardComponent
    {
        public override Point[] Inputs
        {
            get
            {
                Point o;
                GatePainter.GetOutputInput(BoardEditorPanel.GridSize, this.Position, out o);
                return new Point[] { o };
            }
        }

        public override Point[] Outputs
        {
            get
            {
                return new Point[0];
            }
        }

        public override Rectangle Bounds
        {
            get
            {
                return GatePainter.GetOutputBounds(BoardEditorPanel.GridSize, this.Position);
            }
        }

        public override void Paint(Graphics g, Point offset, bool alert)
        {
            Point p = this.Position + new Size(offset);
            GatePainter.PaintChipOutput(g, BoardEditorPanel.GridSize, p, alert);
        }
    }
}
