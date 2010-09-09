using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl;
using System.Drawing;
using System.Windows.Forms;
using CodeBoxControl.Core;

namespace CodeForm
{
    class CSharpControlPanel : ITextEditorControlPanel
    {
        private ITextEditorControlPanelCallBack callback = null;
        private int lastLine = -1;

        private bool GetBreakPoint(int index)
        {
            return callback.GetLineData(index) != null;
        }

        private void SetBreakPoint(int index, bool breakPoint)
        {
            callback.BindLineData(index, breakPoint ? new object() : null);
        }

        public int Width
        {
            get
            {
                return 20;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
            this.callback = callback;
        }

        public void OnEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            if (GetBreakPoint(lineIndex))
            {
                g.FillRectangle(Brushes.Brown, backgroundArea);
            }
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
            if (GetBreakPoint(lineIndex))
            {
                Bitmap image = Images.Breakpoints;
                int x = controlPanelArea.Left + (controlPanelArea.Width - image.Width) / 2;
                int y = controlPanelArea.Top + (controlPanelArea.Height - image.Height) / 2;
                g.DrawImage(image, x, y);
            }
        }

        public void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
            this.lastLine = lineIndex;
        }

        public void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
        }

        public void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
            if (lineIndex == this.lastLine)
            {
                SetBreakPoint(lineIndex, !GetBreakPoint(lineIndex));
                this.callback.TextEditorBox.RedrawEditor();
                this.lastLine = -1;
            }
        }
    }
}
