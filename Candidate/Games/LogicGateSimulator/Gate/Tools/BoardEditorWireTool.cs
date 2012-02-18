using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Gate.Tools
{
    class BoardEditorWireTool : IBoardEditorTool
    {
        private class Command : IBoardEditorCommand
        {
            private BoardEditorPanel panel;

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
            }

            public void OnMouseUp(MouseEventArgs e)
            {
            }

            public void OnMouseMove(MouseEventArgs e)
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
