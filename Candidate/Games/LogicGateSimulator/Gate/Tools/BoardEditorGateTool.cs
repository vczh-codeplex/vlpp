using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;
using System.Windows.Forms;

namespace Gate.Tools
{
    class BoardEditorGateTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
        {
            private Point location = new Point(-100, -100);
            private BoardEditorPanel panel;
            private PrimitiveGates gate;

            private Point GetGatePosition()
            {
                Point p = this.location + new Size(this.panel.DisplayOffset);
                p.X -= p.X % BoardEditorPanel.GridSize;
                p.Y -= p.Y % BoardEditorPanel.GridSize;

                float gridSize = BoardEditorPanel.GridSize;
                PointF fp = new PointF(p.X + gridSize / 2, p.Y + gridSize / 2);
                return new Point((int)fp.X, (int)fp.Y);
            }

            public Command(PrimitiveGates gate)
            {
                this.gate = gate;
            }

            public void Attach(BoardEditorPanel panel)
            {
                this.panel = panel;
                this.panel.Cursor = Cursors.Default;
            }

            public void Detach(BoardEditorPanel panel)
            {
                this.panel = null;
            }

            public void OnMouseDown(System.Windows.Forms.MouseEventArgs e)
            {
                Point position = GetGatePosition();
                Rectangle bounds = GatePainter.GetGateBounds(BoardEditorPanel.GridSize, position);
                bool alert = this.panel.Board.IsConflict(bounds);
                if (!alert)
                {
                    var gate = new LogicGate()
                    {
                        Position = position,
                        GateType = this.gate,
                    };
                    this.panel.Board.AddComponent(gate);
                    this.panel.Refresh();
                }
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
                Point position = GetGatePosition();
                Rectangle bounds = GatePainter.GetGateBounds(BoardEditorPanel.GridSize, position);
                bool alert = this.panel.Board.IsConflict(bounds);
                GatePainter.PaintGate(e.Graphics, BoardEditorPanel.GridSize, position - new Size(this.panel.DisplayOffset), this.gate, alert);
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
}
