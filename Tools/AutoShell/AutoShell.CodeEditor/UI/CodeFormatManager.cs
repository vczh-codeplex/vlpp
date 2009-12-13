using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;

namespace AutoShell.UI
{
    public class CodeFormatManager : IList<CodeFormat>
    {
        private List<CodeFormat> formats = new List<CodeFormat>();
        private List<string> fonts = new List<string>();
        private List<Color> colors = new List<Color>();
        private int textSize = 12;
        private string header = null;

        private int GetColorIndex(Color color)
        {
            int index = this.colors.IndexOf(color);
            if (index == -1)
            {
                this.colors.Add(color);
                return this.colors.Count;
            }
            else
            {
                return index + 1;
            }
        }

        private int GetFontIndex(string font)
        {
            if (font == string.Empty)
            {
                return 0;
            }
            else
            {
                int index = this.fonts.IndexOf(font);
                if (index == -1)
                {
                    this.fonts.Add(font);
                    return this.fonts.Count;
                }
                else
                {
                    return index + 1;
                }
            }
        }

        private void CollectInformation()
        {
            this.fonts.Clear();
            this.colors.Clear();
            foreach (CodeFormat format in this.formats)
            {
                format.indexTextColor = GetColorIndex(format.TextColor);
                format.indexBackColor = format.Background ? GetColorIndex(format.BackColor) : 0;
                format.indexUnderlineColor = GetColorIndex(format.UnderlineColor);
                format.indexTextFont = GetFontIndex(format.TextFont);
            }
        }

        private void BuildHeader()
        {
            this.header = string.Empty;
            this.header += @"{\rtf1\ansi\ansicpg936\deff0\uc0";

            this.header += @"{\fonttbl{\f0\fmodern 宋体;}";
            for (int i = 0; i < this.fonts.Count; i++)
            {
                this.header += @"{\f" + (i + 1).ToString() + "\fmodern ";
                this.header += this.fonts[i];
                this.header += @";}";
            }
            this.header += @"}";

            this.header += @"{\colortbl;";
            foreach (Color color in this.colors)
            {
                this.header += @"\red" + color.R.ToString() + @"\green" + color.G.ToString() + @"\blue" + color.B.ToString() + ";";
            }
            this.header += @"}";

            using (Font font = new Font(Control.DefaultFont.FontFamily, this.textSize, GraphicsUnit.Pixel))
            {
                this.header += @"\f0\cf0\fs" + ((int)(font.SizeInPoints * 2)).ToString() + " ";
            }
        }

        internal void BuildFormat()
        {
            CollectInformation();
            BuildHeader();
            foreach (CodeFormat format in this.formats)
            {
                format.BuildCode();
            }
        }

        public string Header
        {
            get
            {
                return this.header;
            }
        }

        public int TextSize
        {
            get
            {
                return this.textSize;
            }
            set
            {
                this.textSize = value;
                BuildFormat();
            }
        }

        #region IList<CodeFormat> Members

        public int IndexOf(CodeFormat item)
        {
            return this.formats.IndexOf(item);
        }

        public void Insert(int index, CodeFormat item)
        {
            this.formats.Insert(index, item);
            item.manager = this;
            BuildFormat();
        }

        public void RemoveAt(int index)
        {
            this.formats.RemoveAt(index);
            BuildFormat();
        }

        public CodeFormat this[int index]
        {
            get
            {
                return this.formats[index];
            }
            set
            {
                this.formats[index] = value;
                value.manager = this;
                BuildFormat();
            }
        }

        #endregion

        #region ICollection<CodeFormat> Members

        public void Add(CodeFormat item)
        {
            this.formats.Add(item);
            item.manager = this;
            BuildFormat();
        }

        public void Clear()
        {
            this.formats.Clear();
            BuildFormat();
        }

        public bool Contains(CodeFormat item)
        {
            return this.formats.Contains(item);
        }

        public void CopyTo(CodeFormat[] array, int arrayIndex)
        {
            this.formats.CopyTo(array, arrayIndex);
        }

        public int Count
        {
            get
            {
                return this.formats.Count;
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        public bool Remove(CodeFormat item)
        {
            if (this.formats.Remove(item))
            {
                BuildFormat();
                return true;
            }
            else
            {
                return false;
            }
        }

        #endregion

        #region IEnumerable<CodeFormat> Members

        public IEnumerator<CodeFormat> GetEnumerator()
        {
            return this.formats.GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.formats.GetEnumerator();
        }

        #endregion
    }

    public enum CodeUnderline
    {
        None,
        Line,
        Wave
    }

    public class CodeFormat
    {
        private bool bold = false;
        private bool italic = false;
        private CodeUnderline underline = CodeUnderline.None;
        private string textFont = string.Empty;
        private Color textColor = Color.Black;
        private Color backColor = Color.Black;
        private Color underlineColor = Color.Black;
        private bool background = false;

        internal int indexTextFont = -1;
        internal int indexTextColor = -1;
        internal int indexBackColor = -1;
        internal int indexUnderlineColor = -1;
        internal CodeFormatManager manager = null;

        public string Header { get; internal set; }
        public string Tail { get; internal set; }

        private void BuildFormat()
        {
            if (this.manager != null)
            {
                this.manager.BuildFormat();
            }
        }

        internal void BuildCode()
        {
            this.Header = string.Empty;
            if (this.bold) this.Header += @"\b ";
            if (this.italic) this.Header += @"\i ";
            if (this.underline == CodeUnderline.Line)
            {
                this.Header += @"\ul \ulw ";
            }
            else if (this.underline == CodeUnderline.Wave)
            {
                this.Header += @"\ul \ulwave ";
            }
            if (this.indexTextColor != 0) this.Header += @"\cf" + this.indexTextColor.ToString() + " ";
            if (this.indexBackColor != 0) this.Header += @"\highlight" + this.indexBackColor.ToString() + " ";
            if (this.indexUnderlineColor != 0) this.Header += @"\ulc" + this.indexUnderlineColor.ToString() + " ";
            if (this.indexTextFont != 0) this.Header += @"\f" + this.indexTextFont.ToString() + " ";

            this.Tail = string.Empty;
            if (this.bold) this.Tail += @"\b0 ";
            if (this.italic) this.Tail += @"\i0 ";
            if (this.underline != CodeUnderline.None)
            {
                this.Tail += @"\ulnone ";
            }
            if (this.indexTextColor != 0) this.Tail += @"\cf0 ";
            if (this.indexBackColor != 0) this.Tail += @"\highlight0 ";
            if (this.indexUnderlineColor != 0) this.Tail += @"\ulc0 ";
            if (this.indexTextFont != 0) this.Tail += @"\f0 ";
        }

        #region Format Properties

        public bool Bold
        {
            get
            {
                return this.bold;
            }
            set
            {
                this.bold = value;
                BuildFormat();
            }
        }

        public bool Italic
        {
            get
            {
                return this.italic;
            }
            set
            {
                this.italic = value;
                BuildFormat();
            }
        }

        public CodeUnderline Underline
        {
            get
            {
                return this.underline;
            }
            set
            {
                this.underline = value;
                BuildFormat();
            }
        }

        public string TextFont
        {
            get
            {
                return this.textFont;
            }
            set
            {
                this.textFont = value;
                BuildFormat();
            }
        }

        public Color TextColor
        {
            get
            {
                return this.textColor;
            }
            set
            {
                this.textColor = value;
                BuildFormat();
            }
        }

        public Color BackColor
        {
            get
            {
                return this.backColor;
            }
            set
            {
                this.backColor = value;
                BuildFormat();
            }
        }

        public Color UnderlineColor
        {
            get
            {
                return this.underlineColor;
            }
            set
            {
                this.underlineColor = value;
                BuildFormat();
            }
        }

        public bool Background
        {
            get
            {
                return this.background;
            }
            set
            {
                this.background = value;
                BuildFormat();
            }
        }

        #endregion
    }

    internal static class RtfStringEncoder
    {
        private static string[] cache = new string[char.MaxValue - char.MinValue + 1];

        static RtfStringEncoder()
        {
            for (int i = char.MinValue; i <= char.MaxValue; i++)
            {
                char c = (char)i;
                switch (c)
                {
                    case '\n':
                        cache[(uint)c] = "\\par\r\n";
                        break;
                    case '\r':
                        cache[(uint)c] = "";
                        break;
                    case '\\':
                    case '{':
                    case '}':
                    case ' ':
                        cache[(uint)c] = "\\" + c.ToString();
                        break;
                    default:
                        cache[(uint)c] = c.ToString();
                        break;
                }
            }
        }

        public static void ToRtf(this string s, StringBuilder builder, string header, string tail)
        {
            builder.Append(header);
            int length = s.Length;
            for (int i = 0; i < length; i++)
            {
                builder.Append(cache[(uint)s[i]]);
            }
            builder.Append(tail);
        }
    }
}
