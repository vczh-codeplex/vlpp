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

namespace TextEditor
{
    public partial class Form1 : Form
    {
        private string[] code = File.ReadAllLines(@"..\..\Form1.cs");
        private Bitmap bitmap = null;
        private Font font = null;
        private CharMeasurer charMeasurer = null;

        private void ResizeBitmap()
        {
            if (this.bitmap != null)
            {
                if (this.bitmap.Size != this.ClientSize)
                {
                    this.bitmap.Dispose();
                }
            }
            this.bitmap = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            using (Graphics g = Graphics.FromImage(this.bitmap))
            {
                g.FillRectangle(Brushes.White, 0, 0, this.bitmap.Width, this.bitmap.Height);
                this.charMeasurer.TextGraphics = g;
                int y = 0;
                foreach (string line in this.code)
                {
                    int x = 0;
                    foreach (char c in line)
                    {
                        x = this.charMeasurer.DrawChar(c, Brushes.Black, x, y);
                    }
                    y += this.font.Height;
                }
            }
        }

        public Form1()
        {
            this.font = new Font("Microsoft YaHei", 14, GraphicsUnit.Pixel);
            this.charMeasurer = new CharMeasurer(this.font);
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            ResizeBitmap();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            ResizeBitmap();
            e.Graphics.DrawImage(this.bitmap, 0, 0);
        }
    }

    public class TextContent : IScrollableContent
    {
        private string[] text = new string[] { };
        private Font font = null;
        private Size contentSize = new Size(20, 20);
        private Rectangle visibleArea = new Rectangle(0, 0, 0, 0);
        private Bitmap bitmap = null;
        private CharMeasurer charMeasurer = null;

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
                    20 + this.text
                        .Select(line => line.Select(c => this.charMeasurer[c].Width).Sum())
                        .Aggregate(0, Math.Max),
                    20 + this.text.Length * this.font.Height
                    );
                if (this.ContentSizeChanged != null)
                {
                    this.ContentSizeChanged(this, new EventArgs());
                }
            }
        }

        public TextContent()
        {
            this.font = new Font("Microsoft YaHei", 14, GraphicsUnit.Pixel);
            this.charMeasurer = new CharMeasurer(this.font);
        }

        public Size ContentSize
        {
            get
            {
                return this.contentSize;
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
                this.visibleArea = value;
            }
        }

        public void Initialize(Control host, ScrollableContentControl container)
        {
            throw new NotImplementedException();
        }

        public void RenderContent(Graphics g)
        {
            g.DrawImage(this.bitmap, 0, 0);
        }

        public event EventHandler ContentSizeChanged;
    }
}
