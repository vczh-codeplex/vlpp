using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.BoardComponents
{
    class LogicWireNode : GateBoardComponent
    {
        public override Point[] Inputs
        {
            get
            {
                Point i;
                GatePainter.GetWireNodeInput(BoardEditorPanel.GridSize, this.Position, out i);
                return new Point[] { i };
            }
        }

        public override Point[] Outputs
        {
            get
            {
                Point o;
                GatePainter.GetWireNodeOutput(BoardEditorPanel.GridSize, this.Position, out o);
                return new Point[] { o };
            }
        }

        public override Rectangle Bounds
        {
            get
            {
                return GatePainter.GetWireNodeBounds(BoardEditorPanel.GridSize, this.Position);
            }
        }

        public override void Paint(Graphics g, Point offset, bool alert)
        {
            Point p = this.Position + new Size(offset);
            GatePainter.PaintWireNode(g, BoardEditorPanel.GridSize, p, alert);
        }
    }
}
