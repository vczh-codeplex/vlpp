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
        public int Width
        {
            get
            {
                return 16;
            }
        }

        public void InstallCallBack(ITextEditorControlPanelCallBack callback)
        {
        }

        public void OnEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
        }

        public void OnBindedLineDataRemoved(int index, object data)
        {
        }

        public void DrawLineBackground(Graphics g, char[] items, int lineIndex, Point textPosition, Rectangle backgroundArea)
        {
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
        }

        public void OnMouseDown(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
        }

        public void OnMouseMove(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
        }

        public void OnMouseUp(int lineIndex, Rectangle controlPanelArea, Point relativePosition, MouseButtons buttons)
        {
        }
    }
}
