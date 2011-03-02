using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using System.Drawing;
using Developer.WinFormControls;
using System.Windows.Forms;

namespace VlTurtle.EditorControls
{
    class LineNumberProvider : ITextEditorBoxHeaderExtension
    {
        private readonly TextFormatFlags flags = TextFormatFlags.NoPrefix | TextFormatFlags.NoPadding | TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter;
        private ITextEditorControlPanelCallBack callback = null;
        private Font currentFont = null;
        private int width = 0;

        public int Width
        {
            get
            {
                if (this.currentFont != this.callback.TextEditorBox.Font)
                {
                    this.currentFont = this.callback.TextEditorBox.Font;
                    using (Graphics g = Graphics.FromHwnd(this.callback.TextEditorBox.Handle))
                    {
                        this.width = TextRenderer.MeasureText("0000", this.currentFont, Size.Empty, this.flags).Width;
                    }
                }
                return this.width;
            }
        }

        public void DrawControlPanel(Graphics g, int lineIndex, Rectangle controlPanelArea)
        {
            string s = (lineIndex + 1).ToString();
            while (s.Length < 4)
            {
                s = "0" + s;
            }
            if (s.Length > 4)
            {
                s = s.Substring(s.Length - 4);
            }

            Size size = TextRenderer.MeasureText(s, this.callback.TextEditorBox.Font, Size.Empty, this.flags);
            TextRenderer.DrawText(g, s, this.callback.TextEditorBox.Font, controlPanelArea, SystemColors.ButtonShadow, this.callback.TextEditorBox.BackColor, this.flags);
        }

        public void DrawControlPanelBackground(Graphics g, Rectangle backgroundArea)
        {
            int x = backgroundArea.Right - 1;
            int y1 = backgroundArea.Top;
            int y2 = backgroundArea.Bottom;
            g.DrawLine(SystemPens.ButtonShadow, new Point(x, y1), new Point(x, y2));
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

        public void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.callback = callback;
        }
    }
}
