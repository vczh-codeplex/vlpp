using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Linq;
using Developer.WinFormControls;
using Tools.TokenizerBuilder.Shape;

namespace Tools.TokenizerBuilder
{
    public partial class TokenizerEditorBox : ScrollableContentControl
    {
        private Content content = null;
        private Control host = null;

        public TokenizerEditorBox()
            : base(new Content())
        {
            InitializeComponent();
        }

        public void LoadFromFile(string filename)
        {
            this.content.Manager = ShapeManager.LoadFromFile(filename);
        }

        public void SaveToFile(string filename)
        {
            this.content.Manager.SaveToFile(filename);
        }

        public Bitmap SaveToBitmap()
        {
            Bitmap bitmap = new Bitmap(this.ViewSize.Width, this.ViewSize.Height);
            using (Graphics g = Graphics.FromImage(bitmap))
            {
                g.FillRectangle(Brushes.White, 0, 0, bitmap.Width, bitmap.Height);
                this.content.Manager.Draw(g, this.Font, new Size(0, 0));
            }
            return bitmap;
        }

        public void Clear()
        {
            this.content.Manager = new ShapeManager();
        }

        public ShapeManager Document
        {
            get
            {
                return this.content.Manager;
            }
        }

        private class Content : IScrollableContent
        {
            private TokenizerEditorBox editor = null;
            private Control host = null;
            private Graphics temporaryGraphics = null;
            private ShapeManager manager = new ShapeManager();

            private MouseState mouseState = MouseState.Normal;
            private KeyState keyState = KeyState.Normal;

            private ShapeBase selectedShape = null;
            private Point lastMouse = new Point(0, 0);

            public ShapeManager Manager
            {
                get
                {
                    return this.manager;
                }
                set
                {
                    this.manager = value;
                    this.selectedShape = null;
                    UpdateViewSize();
                }
            }

            public ShapeBase SelectedShape
            {
                get
                {
                    return this.selectedShape;
                }
            }

            public void Dispose()
            {
            }

            public void Initialize(Control host, ScrollableContentControl control)
            {
                this.editor = (TokenizerEditorBox)control;
                this.editor.content = this;
                this.host = host;
                this.editor.host = host;
                this.temporaryGraphics = Graphics.FromHwnd(host.Handle);
                this.host.MouseDown += new MouseEventHandler(host_MouseDown);
                this.host.MouseMove += new MouseEventHandler(host_MouseMove);
                this.host.MouseUp += new MouseEventHandler(host_MouseUp);
                this.host.KeyDown += new KeyEventHandler(host_KeyDown);
                this.host.KeyUp += new KeyEventHandler(host_KeyUp);
                this.host.MouseWheel += new MouseEventHandler(host_MouseWheel);
            }

            private void host_MouseWheel(object sender, MouseEventArgs e)
            {
                int offset = -e.Delta / 3;
                this.editor.ViewPosition = new Point(this.editor.ViewPosition.X, this.editor.ViewPosition.Y + offset);
            }

            private void host_MouseDown(object sender, MouseEventArgs e)
            {
                if (e.Button == MouseButtons.Left)
                {
                    switch (this.keyState)
                    {
                        case KeyState.Normal:
                            {
                                this.selectedShape = this.Manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition));
                                this.lastMouse = e.Location;
                                this.mouseState = MouseState.Dragging;
                                this.host.Refresh();
                            }
                            break;
                        case KeyState.Arrow:
                            {
                                this.selectedShape = this.Manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition));
                                this.lastMouse = e.Location;
                                this.mouseState = MouseState.Connecting;
                                this.host.Refresh();
                            }
                            break;
                        case KeyState.State:
                            {
                                StateShape state = new StateShape();
                                state.Name = "New State";
                                state.Position = e.Location + new Size(this.editor.ViewPosition);
                                this.Manager.AddShape(state);
                                this.mouseState = MouseState.Normal;
                                UpdateViewSize();
                            }
                            break;
                    }
                }
                else if (e.Button == MouseButtons.Right)
                {
                    if (this.mouseState == MouseState.Normal)
                    {
                        this.selectedShape = this.Manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition));
                        this.host.Refresh();

                        if (this.selectedShape is StateShape)
                        {
                            StateShape state = this.selectedShape as StateShape;
                            this.host.ContextMenuStrip = this.editor.contextMenuState;
                            this.editor.menuItemStateEditColor.Enabled = state.Type == StateType.Finish;
                        }
                        else if (this.selectedShape is ArrowShape)
                        {
                            this.host.ContextMenuStrip = this.editor.contextMenuArrow;
                        }
                        else
                        {
                            this.host.ContextMenuStrip = null;
                        }
                    }
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
                            UpdateViewSize();
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
                                StateShape end = this.Manager.Pick(this.temporaryGraphics, e.Location + new Size(this.editor.ViewPosition)) as StateShape;
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
                                    this.Manager.AddShape(arrow);
                                    start.OutArrows.Add(arrow);
                                    end.InArrows.Add(arrow);
                                    UpdateViewSize();
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
                                this.Manager.RemoveShape(arrow);
                            }
                            foreach (ArrowShape arrow in state.OutArrows)
                            {
                                arrow.End.InArrows.Remove(arrow);
                                this.Manager.RemoveShape(arrow);
                            }
                            this.Manager.RemoveShape(state);
                        }
                        else if (this.selectedShape is ArrowShape)
                        {
                            ArrowShape arrow = (ArrowShape)this.selectedShape;
                            arrow.Start.OutArrows.Remove(arrow);
                            arrow.End.InArrows.Remove(arrow);
                            this.Manager.RemoveShape(arrow);
                        }
                        this.selectedShape = null;
                        UpdateViewSize();
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
                this.Manager.Draw(g, this.editor.Font, new Size(-viewVisibleBounds.X, -viewVisibleBounds.Y));
                if (this.selectedShape != null)
                {
                    Rectangle bounds = this.selectedShape.SelectionBounds;
                    bounds.Offset(-viewVisibleBounds.X, -viewVisibleBounds.Y);
                    using (Pen pen = new Pen(SystemColors.Highlight))
                    {
                        pen.DashStyle = System.Drawing.Drawing2D.DashStyle.DashDot;
                        g.DrawRectangle(pen, bounds);
                    }
                }
            }

            private void UpdateViewSize()
            {
                Rectangle bounds = this.Manager.Shapes.Values.SelectMany(v => v).Select(s => s.SelectionBounds).Aggregate(new Rectangle(0, 0, 0, 0), Rectangle.Union);
                bounds.X = Math.Min(bounds.X, 0);
                bounds.Y = Math.Min(bounds.Y, 0);
                Size offset = new Size(-bounds.X, -bounds.Y);
                this.Manager.Move(offset);
                this.editor.ViewSize = bounds.Size;
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

        private void menuItemStateEditName_Click(object sender, EventArgs e)
        {
            using (StringEditorForm form = new StringEditorForm())
            {
                form.Value = (this.content.SelectedShape as StateShape).Name;
                if (form.ShowDialog() == DialogResult.OK)
                {
                    (this.content.SelectedShape as StateShape).Name = form.Value;
                    this.host.Refresh();
                }
            }
        }

        private void menuItemStateEditColor_Click(object sender, EventArgs e)
        {
            using (StringEditorForm form = new StringEditorForm())
            {
                form.Value = (this.content.SelectedShape as StateShape).Color;
                if (form.ShowDialog() == DialogResult.OK)
                {
                    (this.content.SelectedShape as StateShape).Color = form.Value;
                    this.host.Refresh();
                }
            }
        }

        private void menuItemStateSwitchTypeNormal_Click(object sender, EventArgs e)
        {
            (this.content.SelectedShape as StateShape).Type = StateType.Normal;
            this.host.Refresh();
        }

        private void menuItemStateSwitchTypeStart_Click(object sender, EventArgs e)
        {
            (this.content.SelectedShape as StateShape).Type = StateType.Start;
            this.host.Refresh();
        }

        private void menuItemStateSwitchTypeFinish_Click(object sender, EventArgs e)
        {
            (this.content.SelectedShape as StateShape).Type = StateType.Finish;
            this.host.Refresh();
        }

        private void menuItemStateSwitchTypePartial_Click(object sender, EventArgs e)
        {
            (this.content.SelectedShape as StateShape).Type = StateType.Partial;
            this.host.Refresh();
        }

        private void menuItemArrowEditName_Click(object sender, EventArgs e)
        {
            using (StringEditorForm form = new StringEditorForm())
            {
                form.Value = (this.content.SelectedShape as ArrowShape).Name;
                if (form.ShowDialog() == DialogResult.OK)
                {
                    (this.content.SelectedShape as ArrowShape).Name = form.Value;
                    this.host.Refresh();
                }
            }
        }
    }
}
