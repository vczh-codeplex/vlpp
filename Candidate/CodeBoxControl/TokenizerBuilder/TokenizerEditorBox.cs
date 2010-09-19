using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using TokenizerBuilder.Shape;
using CodeBoxControl;

namespace TokenizerBuilder
{
    public partial class TokenizerEditorBox : ScrollableContentControl
    {
        public TokenizerEditorBox()
            : base(new Content())
        {
            InitializeComponent();
        }

        private class Content : IScrollableContent
        {
            private TokenizerEditorBox editor = null;
            private Control host = null;
            private ShapeManager manager = new ShapeManager();
            private Graphics temporaryGraphics = null;

            private MouseState mouseState = MouseState.Normal;
            private KeyState keyState = KeyState.Normal;

            private ShapeBase selectedShape = null;
            private Point lastMouse = new Point(0, 0);

            public void Initialize(Control host, ScrollableContentControl control)
            {
                this.editor = (TokenizerEditorBox)control;
                this.host = host;
                this.temporaryGraphics = Graphics.FromHwnd(host.Handle);
                this.host.MouseDown += new MouseEventHandler(host_MouseDown);
                this.host.MouseMove += new MouseEventHandler(host_MouseMove);
                this.host.MouseUp += new MouseEventHandler(host_MouseUp);
                this.host.KeyDown += new KeyEventHandler(host_KeyDown);
                this.host.KeyUp += new KeyEventHandler(host_KeyUp);
            }

            private void host_MouseDown(object sender, MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    switch (this.keyState)
                    {
                        case KeyState.Normal:
                            {
                                this.selectedShape = this.manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition));
                                this.lastMouse = e.Location;
                                this.mouseState = MouseState.Dragging;
                            }
                            break;
                        case KeyState.Arrow:
                            {
                                this.selectedShape = this.manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition));
                                this.lastMouse = e.Location;
                                this.mouseState = MouseState.Connecting;
                            }
                            break;
                        case KeyState.State:
                            {
                                StateShape state = new StateShape();
                                state.Name = "New State";
                                state.Position = e.Location + new Size(this.editor.ViewPosition);
                                this.manager.AddShape(state);
                                this.mouseState = MouseState.Normal;
                            }
                            break;
                    }
                    this.host.Refresh();
                }
            }

            private void host_MouseMove(object sender, MouseEventArgs e)
            {
                switch (this.mouseState)
                {
                    case MouseState.Dragging:
                        if (this.selectedShape != null)
                        {
                            this.selectedShape.Move(new Size(e.Location - new Size(this.lastMouse)));
                            this.lastMouse = e.Location;
                            this.host.Refresh();
                        }
                        break;
                }
            }

            private void host_MouseUp(object sender, MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    switch (this.mouseState)
                    {
                        case MouseState.Connecting:
                            {
                                StateShape start = this.selectedShape as StateShape;
                                StateShape end = this.manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition)) as StateShape;
                                if (start != null && end != null)
                                {
                                    ArrowShape arrow = new ArrowShape();
                                    arrow.Start = start;
                                    arrow.End = end;
                                    arrow.Name = "New Arrow";
                                    if (start == end)
                                    {
                                        arrow.ControlPoint = new Point(start.Position.X, start.Position.Y - start.Radius - 10);
                                    }
                                    else
                                    {
                                        arrow.ControlPoint = new Point((start.Position.X + end.Position.X) / 2, (start.Position.Y + end.Position.Y) / 2);
                                    }
                                    this.manager.AddShape(arrow);
                                    start.OutArrows.Add(arrow);
                                    end.InArrows.Add(arrow);
                                    this.host.Refresh();
                                }
                            }
                            break;
                    }
                    this.mouseState = MouseState.Normal;
                }
            }

            private void host_KeyDown(object sender, KeyEventArgs e)
            {
                if (e.KeyCode == Keys.A)
                {
                    this.keyState = KeyState.Arrow;
                }
                else if (e.KeyCode == Keys.S)
                {
                    this.keyState = KeyState.State;
                }
                else if (e.KeyCode == Keys.Delete)
                {
                    if (this.selectedShape != null)
                    {
                        if (this.selectedShape is StateShape)
                        {
                            StateShape state = (StateShape)this.selectedShape;
                            foreach (ArrowShape arrow in state.InArrows)
                            {
                                arrow.Start.OutArrows.Remove(arrow);
                                arrow.End.InArrows.Remove(arrow);
                                this.manager.RemoveShape(arrow);
                            }
                            foreach (ArrowShape arrow in state.OutArrows)
                            {
                                arrow.Start.OutArrows.Remove(arrow);
                                arrow.End.InArrows.Remove(arrow);
                                this.manager.RemoveShape(arrow);
                            }
                            this.manager.RemoveShape(state);
                        }
                        else if (this.selectedShape is ArrowShape)
                        {
                            ArrowShape arrow = (ArrowShape)this.selectedShape;
                            arrow.Start.OutArrows.Remove(arrow);
                            arrow.End.InArrows.Remove(arrow);
                            this.manager.RemoveShape(arrow);
                        }
                        this.host.Refresh();
                    }
                }
                else
                {
                    this.keyState = KeyState.Normal;
                }
            }

            private void host_KeyUp(object sender, KeyEventArgs e)
            {
                if (e.KeyCode == Keys.A)
                {
                    if (this.keyState == KeyState.Arrow)
                    {
                        this.keyState = KeyState.Normal;
                    }
                }
                else if (e.KeyCode == Keys.S)
                {
                    if (this.keyState == KeyState.State)
                    {
                        this.keyState = KeyState.Normal;
                    }
                }
            }

            public void RenderContent(Graphics g, Rectangle viewVisibleBounds, Rectangle viewAreaBounds)
            {
                g.FillRectangle(Brushes.White, viewAreaBounds);
                this.manager.Draw(g, this.editor.Font, new Size(-viewVisibleBounds.X, -viewVisibleBounds.Y));
            }
        }

        private enum MouseState
        {
            Normal,
            Dragging,
            Connecting
        }

        private enum KeyState
        {
            Normal,
            State,
            Arrow
        }
    }
}
