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
            return this.callback.GetLineData(index) != null;
        }

        private void SetBreakPoint(int index, bool breakPoint)
        {
            this.callback.BindLineData(index, breakPoint ? new object() : null);
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

        public bool NeedColorLineForDisplay(int lineIndex)
        {
            return GetBreakPoint(lineIndex);
        }

        public void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            TextLine<TextEditorBox.LineInfo> line = this.callback.TextEditorBox.TextProvider[lineIndex];
            int count = line.CharCount;
            char[] chars = line.CharArray;
            int color = CSharpColorizer.NormalColorId;
            for (int i = 0; i < count; i++)
            {
                if (chars[i] != ' ' && chars[i] != '\t')
                {
                    color = CSharpColorizer.BreakPointColorId;
                }
                colors[i] = color;
            }
        }

        public void DrawLineBackground(Graphics g, int lineIndex, Rectangle backgroundArea)
        {
            if (GetBreakPoint(lineIndex))
            {
                TextLine<TextEditorBox.LineInfo> line = this.callback.TextEditorBox.TextProvider[lineIndex];
                int count = line.CharCount;
                char[] chars = line.CharArray;
                int start = -1;
                for (int i = 0; i < count; i++)
                {
                    if (chars[i] != ' ' && chars[i] != '\t')
                    {
                        start = i;
                        break;
                    }
                }
                if (start != -1)
                {
                    int x1 = this.callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, start)).X;
                    int x2 = this.callback.TextEditorBox.TextPositionToViewPoint(new TextPosition(lineIndex, count)).X;
                    if (x1 < backgroundArea.Left) x1 = backgroundArea.Left;
                    if (x2 > backgroundArea.Right) x2 = backgroundArea.Right;
                    if (x2 > x1)
                    {
                        g.FillRectangle(Brushes.Brown, x1, backgroundArea.Top, x2 - x1, backgroundArea.Height);
                    }
                }
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

        public void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea)
        {
            g.DrawLine(Pens.Gray, new Point(backgroundArea.Right - 1, backgroundArea.Top), new Point(backgroundArea.Right - 1, backgroundArea.Bottom));
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
