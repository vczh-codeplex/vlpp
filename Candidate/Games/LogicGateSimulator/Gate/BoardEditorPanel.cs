using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Drawing2D;
using Gate.BoardComponents;

namespace Gate
{
    class BoardEditorPanel : Panel
    {
        public const int GridSize = 15;

        private Point displayOffset;
        private IBoardEditorCommand currentCommand;
        private GateBoard board = new GateBoard();

        public BoardEditorPanel()
        {
            this.DoubleBuffered = true;
        }

        #region Event Handlers

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseDown(e);
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseUp(e);
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnMouseMove(e);
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnKeyDown(e);
            }
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnKeyPress(e);
            }
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);
            if (this.currentCommand != null)
            {
                this.currentCommand.OnKeyUp(e);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
            e.Graphics.CompositingQuality = CompositingQuality.HighQuality;
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

            this.board.Paint(e.Graphics, new Point(-this.displayOffset.X, -this.displayOffset.Y));
            if (this.currentCommand != null)
            {
                this.currentCommand.OnPaint(e);
            }
        }

        #endregion

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
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

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public GateBoard Board
        {
            get
            {
                return this.board;
            }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
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
                Refresh();
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
        void OnKeyDown(KeyEventArgs e);
        void OnKeyPress(KeyPressEventArgs e);
        void OnKeyUp(KeyEventArgs e);
        void OnPaint(PaintEventArgs e);
    }
}
