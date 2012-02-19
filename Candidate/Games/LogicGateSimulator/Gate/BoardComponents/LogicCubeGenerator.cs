﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.BoardComponents
{
    class LogicCubeGenerator : GateBoardComponent
    {
        public override Point[] Inputs
        {
            get
            {
                return new Point[0];
            }
        }

        public override Point[] Outputs
        {
            get
            {
                Point o;
                GatePainter.GetInputOutput(BoardEditorPanel.GridSize, this.Position, out o);
                return new Point[] { o };
            }
        }

        public override Rectangle Bounds
        {
            get
            {
                return GatePainter.GetInputBounds(BoardEditorPanel.GridSize, this.Position);
            }
        }

        public override void Paint(Graphics g, Point offset, bool alert)
        {
            Point p = this.Position + new Size(offset);
            GatePainter.PaintCubeGenerator(g, BoardEditorPanel.GridSize, p, alert);
        }
    }
}
