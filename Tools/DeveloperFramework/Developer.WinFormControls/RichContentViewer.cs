﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public class RichContentViewer : Panel
    {
        private RichContent content = null;

        public RichContentViewer()
        {
            this.SetStyle(ControlStyles.Opaque, true);
            this.DoubleBuffered = true;
        }

        public void SetContent(XDocument content, int maxWidth)
        {
            using (Graphics g = Graphics.FromHwnd(this.Handle))
            {
                this.content = new RichContent(content, this.Font, g);
                this.ClientSize = new Size(this.content.ContentSize.Width + 10, this.content.ContentSize.Height + 10);
            }
        }

        public void ClearContent()
        {
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Size textSize = this.ClientSize;
            e.Graphics.FillRectangle(SystemBrushes.Info, 0, 0, textSize.Width, textSize.Height);
            this.content.Render(e.Graphics, new Size(5, 5));
            e.Graphics.DrawRectangle(SystemPens.InfoText, 0, 0, textSize.Width - 1, textSize.Height - 1);
        }
    }
}
