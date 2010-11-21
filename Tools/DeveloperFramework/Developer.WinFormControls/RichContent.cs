using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.WinFormControls
{
    public class RichContent
    {
        #region Content Builder

        public class Text : XCData
        {
            public Text(string value)
                : base(value)
            {
            }
        }

        public class Bold : XElement
        {
            public Bold(params object[] contents)
                : base("b", contents)
            {
            }
        }

        public class Italic : XElement
        {
            public Italic(params object[] contents)
                : base("i", contents)
            {
            }
        }

        public class Underline : XElement
        {
            public Underline(params object[] contents)
                : base("u", contents)
            {
            }
        }

        public class Content : XDocument
        {
            public Content(params object[] contents)
                : base(new XElement("content", contents))
            {
            }
        }

        #endregion

        #region Content Renderer

        public abstract class ContentBlock
        {
            public Size BlockSize { get; protected set; }
            public Point BlockLocation { get; internal set; }
            public abstract void Render(Graphics g, Point location);
            public abstract void Update(Graphics g);
        }

        public class TextBlock : ContentBlock
        {
            public Font FontSkeleton { get; set; }
            public bool Bold { get; set; }
            public bool Italic { get; set; }
            public bool Underline { get; set; }
            public Color Color { get; set; }
            public string Text { get; set; }

            public override void Render(Graphics g, Point location)
            {
                using (Font font = CreateFont())
                {
                    TextRenderer.DrawText(g, this.Text, font, location, this.Color, TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
                }
            }

            public override void Update(Graphics g)
            {
                using (Font font = CreateFont())
                {
                    this.BlockSize = TextRenderer.MeasureText(g, this.Text, font, new Size(0, 0), TextFormatFlags.ExpandTabs | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
                }
            }

            private Font CreateFont()
            {
                FontStyle style = FontStyle.Regular;
                if (this.Bold) style |= FontStyle.Bold;
                if (this.Italic) style |= FontStyle.Italic;
                if (this.Underline) style |= FontStyle.Underline;
                return new Font(this.FontSkeleton, style);
            }
        }

        #endregion

        private XDocument content = null;
        private Font font = null;
        private List<List<ContentBlock>> blockLines = null;

        public Size ContentSize { get; private set; }

        public RichContent(XDocument content, Font font, Graphics g)
        {
            CheckContent(content.Root);
            this.content = content;
            this.font = font;
            this.blockLines = Compile();
            int counter = 0;
            foreach (var block in this.blockLines.SelectMany(l => l))
            {
                block.Update(g);
                counter++;
            }
            if (counter > 0)
            {
                int totalHeight = 0;
                foreach (var line in this.blockLines)
                {
                    int totalWidth = 0;
                    int maxHeight = 0;
                    foreach (var block in line)
                    {
                        maxHeight = Math.Max(maxHeight, block.BlockSize.Height);
                    }
                    foreach (var block in line)
                    {
                        block.BlockLocation = new Point(totalWidth, totalHeight + maxHeight - block.BlockSize.Height);
                        totalWidth += block.BlockSize.Width;
                    }
                    totalHeight += maxHeight;
                }
                int width = this.blockLines.SelectMany(l => l).Max(b => b.BlockLocation.X + b.BlockSize.Width);
                int height = this.blockLines.SelectMany(l => l).Max(b => b.BlockLocation.Y + b.BlockSize.Height);
                this.ContentSize = new Size(width, height);
            }
        }

        public void Render(Graphics g, Size offset)
        {
            foreach (var block in this.blockLines.SelectMany(l => l))
            {
                block.Render(g, block.BlockLocation + offset);
            }
        }

        private List<List<ContentBlock>> Compile()
        {
            List<List<ContentBlock>> lines = new List<List<ContentBlock>>();
            List<ContentBlock> current = new List<ContentBlock>();
            lines.Add(current);
            CompileSingle(this.content.Root, this.font, false, false, false, lines, ref current);
            return lines;
        }

        private void CheckContent(XElement element)
        {
            if (element.Name != "content") throw new ArgumentException("Wrong schema.");
            if (element.Attributes().Count() != 0) throw new ArgumentException("Wrong schema.");
            foreach (var node in element.Nodes())
            {
                CheckSubContent(node);
            }
        }

        private void CheckSubContent(XNode node)
        {
            XElement element = node as XElement;
            if (element != null)
            {
                if (element.Name != "b" && element.Name != "i" && element.Name != "u") throw new ArgumentException("Wrong schema.");
                if (element.Attributes().Count() != 0) throw new ArgumentException("Wrong schema.");
            }
            else
            {
                if (!(node is XText))
                {
                    throw new ArgumentException("Wrong schema.");
                }
            }
        }

        private void CompileSingle(XNode node, Font font, bool bold, bool italic, bool underline, List<List<ContentBlock>> lines, ref List<ContentBlock> current)
        {
            XElement element = node as XElement;
            XText text = node as XText;
            if (element != null)
            {
                switch (element.Name.LocalName)
                {
                    case "content":
                        foreach (var n in element.Nodes())
                        {
                            CompileSingle(n, font, bold, italic, underline, lines, ref current);
                        }
                        break;
                    case "b":
                        foreach (var n in element.Nodes())
                        {
                            CompileSingle(n, font, true, italic, underline, lines, ref current);
                        }
                        break;
                    case "i":
                        foreach (var n in element.Nodes())
                        {
                            CompileSingle(n, font, bold, true, underline, lines, ref current);
                        }
                        break;
                    case "u":
                        foreach (var n in element.Nodes())
                        {
                            CompileSingle(n, font, bold, italic, true, lines, ref current);
                        }
                        break;
                }
            }
            else
            {
                string[] textLines = text.Value
                    .Split(new string[] { "\r\n" }, StringSplitOptions.None)
                    .SelectMany(s => s.Split(new string[] { "\r", "\n" }, StringSplitOptions.None))
                    .Select(s => string.IsNullOrEmpty(s) ? " " : s)
                    .ToArray();
                for (int i = 0; i < textLines.Length; i++)
                {
                    if (i > 0)
                    {
                        current = new List<ContentBlock>();
                        lines.Add(current);
                    }
                    TextBlock block = new TextBlock()
                    {
                        FontSkeleton = font,
                        Bold = bold,
                        Italic = italic,
                        Underline = underline,
                        Color = Color.Black,
                        Text = textLines[i]
                    };
                    current.Add(block);
                }
            }
        }
    }
}
