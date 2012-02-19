using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;
using System.Windows.Forms;

namespace Gate.Tools
{
    class BoardEditorComponentCommand : IBoardEditorCommand
    {
        private Point location = new Point(-100, -100);
        private BoardEditorPanel panel;
        private GateBoardComponent placingComponent;
        private Func<GateBoardComponent> componentBuilder;

        private Point GetGatePosition()
        {
            Point p = this.location + new Size(this.panel.DisplayOffset);
            p.X -= p.X % BoardEditorPanel.GridSize;
            p.Y -= p.Y % BoardEditorPanel.GridSize;

            float gridSize = BoardEditorPanel.GridSize;
            PointF fp = new PointF(p.X + gridSize / 2, p.Y + gridSize / 2);
            return new Point((int)fp.X, (int)fp.Y);
        }

        public BoardEditorComponentCommand(Func<GateBoardComponent> componentBuilder)
        {
            this.componentBuilder = componentBuilder;
        }

        public void Attach(BoardEditorPanel panel)
        {
            this.panel = panel;
            this.panel.Cursor = Cursors.Default;

            this.placingComponent = this.componentBuilder();
            this.placingComponent.Position = GetGatePosition();
        }

        public void Detach(BoardEditorPanel panel)
        {
            this.panel = null;
        }

        public void OnMouseDown(MouseEventArgs e)
        {
            bool alert = this.panel.Board.IsConflict(this.placingComponent.Bounds);
            if (!alert)
            {
                this.panel.Board.AddComponent(this.placingComponent);
                this.placingComponent = null;
                this.panel.Refresh();

                this.placingComponent = this.componentBuilder();
                this.placingComponent.Position = GetGatePosition();
            }
        }

        public void OnMouseUp(MouseEventArgs e)
        {
        }

        public void OnMouseMove(MouseEventArgs e)
        {
            this.location = e.Location;
            this.placingComponent.Position = GetGatePosition();
            this.panel.Refresh();
        }

        public void OnPaint(PaintEventArgs e)
        {
            if (this.placingComponent != null)
            {
                Point offset = new Point(-this.panel.DisplayOffset.X, -this.panel.DisplayOffset.Y);
                if (this.panel.Board.IsConflict(this.placingComponent.Bounds))
                {
                    this.placingComponent.Paint(e.Graphics, offset, true);
                }
                else
                {
                    this.placingComponent.Paint(e.Graphics, offset, false);
                }
            }
        }
    }
}
