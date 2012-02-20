using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Gate.BoardComponents;

namespace Gate.Tools
{
    class BoardEditorWireTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
        {
            private BoardEditorPanel panel;
            private Point mouseLocation = new Point(-100, -100);
            private GateBoardAccess selectedOutput;
            private GateBoardAccess selectedInput;

            private int ChoosePoint(Point[] points, Point position)
            {
                var d2 = BoardEditorPanel.GridSize * BoardEditorPanel.GridSize;
                var r = points
                    .Select((p, i) => Tuple.Create(p, i))
                    .FirstOrDefault(t =>
                        ((position.X - t.Item1.X) * (position.X - t.Item1.X) + (position.Y - t.Item1.Y) * (position.Y - t.Item1.Y)) * 4 <= d2
                        );
                return r == null ? -1 : r.Item2;
            }

            private GateBoardAccess GetAccess(bool output, Point position)
            {
                position += new Size(this.panel.DisplayOffset);
                var component = this.panel.Board.GetComponent(position);
                if (component != null)
                {
                    int index = ChoosePoint(output ? component.Outputs : component.Inputs, position);
                    if (index != -1)
                    {
                        return new GateBoardAccess
                        {
                            Component = component,
                            PortIndex = index,
                        };
                    }
                }
                return null;
            }

            public void Attach(BoardEditorPanel panel)
            {
                this.panel = panel;
                this.panel.Cursor = Cursors.Cross;
            }

            public void Detach(BoardEditorPanel panel)
            {
                this.panel = null;
            }

            public void OnMouseDown(MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    if (this.selectedOutput == null)
                    {
                        this.selectedOutput = GetAccess(true, this.mouseLocation);
                    }
                    else
                    {
                        this.selectedInput = GetAccess(false, this.mouseLocation);
                        if (this.selectedInput != null)
                        {
                            var i = this.selectedInput;
                            var o = this.selectedOutput;
                            if (i.Component.SetInputAccess(i.PortIndex, o.Component, o.PortIndex))
                            {
                                this.selectedOutput = null;
                            }
                            this.selectedInput = null;
                        }
                    }
                }
                else if (e.Button == MouseButtons.Right)
                {
                    this.selectedOutput = null;
                    this.selectedInput = null;
                }
                this.panel.Refresh();
            }

            public void OnMouseUp(MouseEventArgs e)
            {
            }

            public void OnMouseMove(MouseEventArgs e)
            {
                this.mouseLocation = e.Location;
                this.panel.Refresh();
            }

            public void OnKeyDown(KeyEventArgs e)
            {
                if (e.KeyCode == Keys.Escape)
                {
                    this.selectedOutput = null;
                    this.selectedInput = null;
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
                bool output = this.selectedOutput == null;
                var access = GetAccess(output, this.mouseLocation);
                if (access != null)
                {
                    Point p = (output ? access.Component.Outputs : access.Component.Inputs)[access.PortIndex];
                    p -= new Size(this.panel.DisplayOffset);
                    int d = BoardEditorPanel.GridSize / 2;
                    Rectangle bounds = new Rectangle(p - new Size(d, d), new Size(d * 2, d * 2));
                    this.panel.Board.PaintSelectBox(e.Graphics, bounds, Color.SkyBlue);
                }

                if (this.selectedOutput != null)
                {
                    Point p = this.selectedOutput.Component.Outputs[this.selectedOutput.PortIndex];
                    p -= new Size(this.panel.DisplayOffset);
                    e.Graphics.DrawLine(Pens.Purple, p, this.mouseLocation);
                }
            }
        }

        public Bitmap Icon
        {
            get
            {
                return ToolIcons.Wire;
            }
        }

        public string Name
        {
            get
            {
                return "Wire";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new Command();
        }
    }
}
