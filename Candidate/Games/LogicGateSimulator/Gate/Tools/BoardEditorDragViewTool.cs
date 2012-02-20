using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Gate.Tools
{
    class BoardEditorDragViewTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
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

            public void OnKeyDown(KeyEventArgs e)
            {
            }

            public void OnKeyPress(KeyPressEventArgs e)
            {
            }

            public void OnKeyUp(KeyEventArgs e)
            {
            }

            public void OnPaint(PaintEventArgs e)
            {
            }
        }

        public Bitmap Icon
        {
            get
            {
                return ToolIcons.DragView;
            }
        }

        public string Name
        {
            get
            {
                return "Drag View";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new Command();
        }
    }
}
