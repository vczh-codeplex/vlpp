using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace TextEditor
{
    public class CharMeasurer
    {
        private TextFormatFlags textFormatFlags;
        private Size[] charSizes = new Size[65536];
        private string[] charStrings = new string[65536];

        public Font TextFont { get; private set; }
        public Graphics TextGraphics { get; set; }

        private Size GetSize(char c)
        {
            int index = (int)c;
            if (this.charSizes[index].IsEmpty)
            {
                this.charSizes[index] = TextRenderer.MeasureText(this.TextGraphics, c.ToString(), this.TextFont, new Size(0, 0), this.textFormatFlags);
            }
            return this.charSizes[index];
        }

        private string GetString(char c)
        {
            int index = (int)c;
            if (this.charStrings[index] == null)
            {
                this.charStrings[index] = c.ToString();
            }
            return this.charStrings[index];
        }

        public CharMeasurer(Font textFont)
        {
            this.TextFont = textFont;

            this.textFormatFlags =
                 TextFormatFlags.Left
                 | TextFormatFlags.NoPrefix
                 | TextFormatFlags.NoPadding
                 | TextFormatFlags.NoClipping
                 | TextFormatFlags.SingleLine
                 | TextFormatFlags.GlyphOverhangPadding
                 | TextFormatFlags.PreserveGraphicsTranslateTransform
                 ;
        }

        public Size this[char c]
        {
            get
            {
                return GetSize(c);
            }
        }

        public int DrawChar(char c, Brush brush, int x, int y)
        {
            this.TextGraphics.DrawString(GetString(c), this.TextFont, brush, new Point(x, y), StringFormat.GenericDefault);
            return x + GetSize(c).Width;
        }
    }
}