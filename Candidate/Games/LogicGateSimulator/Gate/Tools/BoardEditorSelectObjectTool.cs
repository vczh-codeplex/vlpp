using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Gate.BoardComponents;
using System.Drawing.Drawing2D;

namespace Gate.Tools
{
    class BoardEditorSelectObjectTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
        {
            private BoardEditorPanel panel;
            private Point location = new Point(-100, -100);
            private GateBoardComponent tracingComponent;

            private bool dragging;
            private Point draggingOffset;

            private GateBoardComponent GetComponent()
            {
                Point p = this.location + new Size(this.panel.DisplayOffset);
                return this.panel.Board.GetComponent(p);
            }

            public void Attach(BoardEditorPanel panel)
            {
                this.panel = panel;
                this.panel.Cursor = Cursors.Cross;
            }

            public void Detach(BoardEditorPanel panel)
            {
                this.panel.Board.SelectedComponent = null;
                this.panel.Refresh();
                this.panel = null;
            }

            public void OnMouseDown(MouseEventArgs e)
            {
                this.panel.Focus();
                if (e.Button == MouseButtons.Left)
                {
                    this.panel.Board.SelectedComponent = this.tracingComponent;
                    if (this.tracingComponent != null)
                    {
                        this.panel.Cursor = Cursors.SizeAll;
                        this.tracingComponent = null;
                        this.dragging = true;
                        this.draggingOffset = this.panel.Board.SelectedComponent.Position - new Size(this.location);
                    }
                    this.panel.Refresh();
                }
            }

            public void OnMouseUp(MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    this.dragging = false;
                    this.panel.Cursor = Cursors.Cross;
                }
            }

            public void OnMouseMove(MouseEventArgs e)
            {
                this.location = e.Location;
                if (this.dragging)
                {
                    Point p = this.location + new Size(this.draggingOffset);
                    Point d = p - new Size(this.panel.Board.SelectedComponent.Position);
                    d.X = d.X % BoardEditorPanel.GridSize;
                    d.Y = d.Y % BoardEditorPanel.GridSize;
                    p -= new Size(d);

                    this.panel.Board.SelectedComponent.Position = p;
                    this.panel.Refresh();
                }
                else
                {
                    var component = GetComponent();
                    if (this.tracingComponent != component)
                    {
                        this.tracingComponent = GetComponent();
                        this.panel.Refresh();
                    }
                }
            }

            public void OnKeyDown(KeyEventArgs e)
            {
                var component = this.panel.Board.SelectedComponent;
                if (component != null)
                {
                    component.ClearAccess();
                    this.tracingComponent = null;
                    this.panel.Board.RemoveComponent(component);
                    this.panel.Refresh();
                }
            }

            public void OnKeyPress(KeyPressEventArgs e)
            {
            }

            public void OnKeyUp(KeyEventArgs e)
            {
            }

            public void OnPaint(PaintEventArgs e)
            {
                if (this.tracingComponent != null && this.tracingComponent != this.panel.Board.SelectedComponent)
                {
                    Rectangle bounds = this.tracingComponent.Bounds;
                    bounds.Location -= new Size(this.panel.DisplayOffset);
                    this.panel.Board.PaintSelectBox(e.Graphics, bounds, Color.SkyBlue);
                }
            }
        }

        public Bitmap Icon
        {
            get
            {
                return ToolIcons.SelectObjects;
            }
        }

        public string Name
        {
            get
            {
                return "Select Object";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new Command();
        }
    }
}
