﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.BoardComponents
{
    class LogicGate : GateBoardComponent
    {
        public PrimitiveGates GateType { get; private set; }

        public LogicGate(PrimitiveGates gateType)
        {
            this.GateType = gateType;
        }

        public override Point[] Inputs
        {
            get
            {
                if (this.GateType == PrimitiveGates.Not)
                {
                    Point i;
                    GatePainter.GetGateInput(BoardEditorPanel.GridSize, this.Position, out i);
                    return new Point[] { i };
                }
                else
                {
                    Point i1, i2;
                    GatePainter.GetGateInput(BoardEditorPanel.GridSize, this.Position, out i1, out i2);
                    return new Point[] { i1, i2 };
                }
            }
        }

        public override Point[] Outputs
        {
            get
            {
                Point o;
                GatePainter.GetGateOutput(BoardEditorPanel.GridSize, this.Position, out o);
                return new Point[] { o };
            }
        }

        public override Rectangle Bounds
        {
            get
            {
                return GatePainter.GetGateBounds(BoardEditorPanel.GridSize, this.Position);
            }
        }

        public override void Paint(Graphics g, Point offset, bool alert)
        {
            Point p = this.Position + new Size(offset);
            GatePainter.PaintGate(g, BoardEditorPanel.GridSize, p, this.GateType, alert);
        }
    }
}
