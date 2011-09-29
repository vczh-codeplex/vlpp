using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Developer.WinFormControls;
using System.Diagnostics;

namespace TextEditor
{
    public partial class Form1 : Form
    {
        private TextContent textContent = new TextContent();

        public Form1()
        {
            InitializeComponent();
            scrollableContentControl.ScrollableContent = this.textContent;
            this.textContent.Text = File.ReadAllText(@"..\..\Form1.cs");
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            scrollableContentControl.ScrollableContent = null;
            this.textContent.Dispose();
            this.textContent = null;
        }
    }

    public class TextContent : IScrollableContent, IDisposable
    {
        private string[] text = new string[] { };
        private Font font = null;
        private Size contentSize = new Size(0, 0);
        private Size controlSize = new Size(0, 0);
        private Rectangle visibleArea = new Rectangle(0, 0, 0, 0);
        private CharMeasurer charMeasurer = null;

        private Bitmap bitmap = null;
        private Graphics bitmapGraphics = null;
        private bool bitmapCleared = true;

        private void ResizeBitmap(Size size)
        {
            if (this.bitmap != null && this.bitmap.Size != size)
            {
                this.bitmapGraphics.Dispose();
                this.bitmap.Dispose();
            }
            this.bitmap = new Bitmap(Math.Max(1, size.Width), Math.Max(1, size.Height));
            this.bitmapGraphics = Graphics.FromImage(this.bitmap);
            this.charMeasurer.TextGraphics = this.bitmapGraphics;
            this.bitmapCleared = true;
        }

        public string Text
        {
            get
            {
                return this.text.Aggregate("", (a, b) => a == "" ? b : a + "\r\n" + b);
            }
            set
            {
                this.text = value.Split(new string[] { "\r\n" }, StringSplitOptions.None);
                this.contentSize = new Size(
                     this.text
                        .Select(line => line.Select(c => this.charMeasurer[c].Width).Sum())
                        .Aggregate(0, Math.Max),
                     this.text.Length * this.font.Height
                    );
                if (this.ContentSizeChanged != null)
                {
                    this.ContentSizeChanged(this, new EventArgs());
                }
            }
        }

        private void RenderText(Rectangle oldVisibleArea)
        {
            int min = this.visibleArea.Y;
            int max = this.visibleArea.Bottom;

            if (oldVisibleArea.X != this.visibleArea.X)
            {
                this.bitmapCleared = true;
            }

            if (this.bitmapCleared)
            {
                this.bitmapCleared = false;
            }
            else if (oldVisibleArea != this.visibleArea)
            {
                int offset = oldVisibleArea.Y - this.visibleArea.Y;
                this.bitmapGraphics.DrawImageUnscaled(this.bitmap, 0, offset);
                if (oldVisibleArea.Y < this.visibleArea.Y)
                {
                    min = Math.Max(min, max + offset);
                }
                else if (oldVisibleArea.Y > this.visibleArea.Y)
                {
                    max = Math.Min(max, min + offset);
                }
            }

            int start = min / this.font.Height;
            int end = Math.Min(this.text.Length - 1, max / this.font.Height + 1);
            for (int i = start; i <= end; i++)
            {
                int y = i * this.font.Height - this.visibleArea.Y;
                this.bitmapGraphics.FillRectangle(Brushes.White, 0, y, this.bitmap.Width, this.font.Height);

                int x = -this.visibleArea.X;
                foreach (char c in this.text[i])
                {
                    x = this.charMeasurer.DrawChar(c, Brushes.Black, x, y);
                }
            }
        }

        public TextContent()
        {
            this.font = new Font("Microsoft YaHei", 14, GraphicsUnit.Pixel);
            this.charMeasurer = new CharMeasurer(this.font);
        }

        public void Dispose()
        {
            this.bitmapGraphics.Dispose();
            this.bitmap.Dispose();
            this.font.Dispose();
        }

        public Size ContentSize
        {
            get
            {
                return this.contentSize;
            }
        }
        public Size ControlSize
        {
            get
            {
                return this.controlSize;
            }
            set
            {
                this.controlSize = value;
                ResizeBitmap(this.controlSize);
            }
        }

        public Rectangle VisibleArea
        {
            get
            {
                return this.visibleArea;
            }
            set
            {
                Rectangle oldVisibleArea = this.visibleArea;
                this.visibleArea = value;
                RenderText(oldVisibleArea);
                if (this.ContentChanged != null)
                {
                    this.ContentChanged(this, new EventArgs());
                }
            }
        }

        public void Initialize(Control host, ScrollableContentControl container)
        {
            host.Cursor = Cursors.IBeam;
        }

        public void RenderContent(Graphics g)
        {
            g.DrawImage(this.bitmap, 0, 0);
        }

        public event EventHandler ContentSizeChanged;
        public event EventHandler ContentChanged;
    }
}
