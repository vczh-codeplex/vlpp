using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Gate
{
    class BoardEditorPanel : Panel
    {
        private const int GridSize = 15;

        private Point displayOffset;
        private IBoardEditorCommand currentCommand;

        public BoardEditorPanel()
        {
            this.DoubleBuffered = true;

            this.MouseDown += new MouseEventHandler(BoardEditorPanel_MouseDown);
            this.MouseUp += new MouseEventHandler(BoardEditorPanel_MouseUp);
            this.MouseMove += new MouseEventHandler(BoardEditorPanel_MouseMove);

            this.CurrentCommand = new BoardEditorDragViewCommand();
        }

        #region Event Handlers

        private void BoardEditorPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseDown(e);
            }
        }

        private void BoardEditorPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseUp(e);
            }
        }

        private void BoardEditorPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseMove(e);
            }
        }

        #endregion

        public Point DisplayOffset
        {
            get
            {
                return this.displayOffset;
            }
            set
            {
                this.displayOffset = value;
                Refresh();
            }
        }

        public IBoardEditorCommand CurrentCommand
        {
            get
            {
                return this.currentCommand;
            }
            set
            {
                if (this.currentCommand != null)
                {
                    this.currentCommand.Detach(this);
                }
                this.currentCommand = value;
                if (this.currentCommand != null)
                {
                    this.currentCommand.Attach(this);
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.FillRectangle(Brushes.White, e.Graphics.ClipBounds);
            int startX = (GridSize - this.displayOffset.X % GridSize) % GridSize;
            int startY = (GridSize - this.displayOffset.Y % GridSize) % GridSize;
            int countX = this.ClientSize.Width / GridSize + 1;
            int countY = this.ClientSize.Height / GridSize + 1;

            for (int i = 0; i < countX; i++)
            {
                int x = startX + i * GridSize;
                e.Graphics.DrawLine(Pens.LightGray, x, 0, x, this.ClientSize.Height);
            }
            for (int i = 0; i < countY; i++)
            {
                int y = startY + i * GridSize;
                e.Graphics.DrawLine(Pens.LightGray, 0, y, this.ClientSize.Width, y);
            }
        }
    }

    interface IBoardEditorCommand
    {
        void Attach(BoardEditorPanel panel);
        void Detach(BoardEditorPanel panel);
        void OnMouseDown(MouseEventArgs e);
        void OnMouseUp(MouseEventArgs e);
        void OnMouseMove(MouseEventArgs e);
    }

    class BoardEditorDragViewCommand : IBoardEditorCommand
    {
        private bool dragging;
        private Point draggingLocation;
        private BoardEditorPanel panel;

        public void Attach(BoardEditorPanel panel)
        {
            this.panel = panel;
            this.panel.Cursor = Cursors.SizeAll;
        }

        public void Detach(BoardEditorPanel panel)
        {
            this.panel = null;
        }

        public void OnMouseDown(MouseEventArgs e)
        {
            this.dragging = true;
            this.draggingLocation = e.Location;
        }

        public void OnMouseUp(MouseEventArgs e)
        {
            this.dragging = false;
        }

        public void OnMouseMove(MouseEventArgs e)
        {
            if (this.dragging)
            {
                this.panel.DisplayOffset += new Size(this.draggingLocation) - new Size(e.Location);
                this.draggingLocation = e.Location;
            }
        }
    }
}
