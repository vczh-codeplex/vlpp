using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Developer.WinFormControls
{
    public class RichContentViewer : Panel
    {
        private Bitmap bitmap = null;

        public RichContentViewer()
        {
            this.SetStyle(ControlStyles.Opaque, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        public void SetContent(string text, int maxWidth)
        {
            text = text.TrimEnd(' ', '\t', '\r', '\n');
            Size textSize;
            using (Graphics g = Graphics.FromHwnd(this.Handle))
            {
                textSize = TextRenderer.MeasureText(g, text, this.Font, new Size(0, 0), TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
            }
            textSize.Width += 10;
            textSize.Height += 10;

            if (this.bitmap != null)
            {
                this.bitmap.Dispose();
            }
            this.bitmap = new Bitmap(textSize.Width, textSize.Height);
            using (Graphics g = Graphics.FromImage(this.bitmap))
            {
                g.FillRectangle(SystemBrushes.Info, 0, 0, textSize.Width, textSize.Height);
                TextRenderer.DrawText(g, text, this.Font, new Point(5, 5), SystemColors.InfoText, TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
                g.DrawRectangle(SystemPens.InfoText, 0, 0, textSize.Width - 1, textSize.Height - 1);
            }
            this.ClientSize = textSize;
        }

        public void ClearContent()
        {
            if (this.bitmap != null)
            {
                this.bitmap.Dispose();
                this.bitmap = null;
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (this.bitmap != null)
                {
                    this.bitmap.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.bitmap, 0, 0);
        }
    }
}
