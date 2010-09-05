using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CodeBoxControl.Core
{
    public class TextProvider<T> : IDisposable
        where T : new()
    {
        private List<TextLine<T>> lines = null;

        public int Count
        {
            get
            {
                return this.lines.Count;
            }
        }

        public TextLine<T> this[int index]
        {
            get
            {
                return this.lines[index];
            }
        }

        public string Text
        {
            get
            {
                return GetString(
                    new TextPosition(0, 0),
                    new TextPosition(this.lines.Count - 1, this.lines[this.lines.Count - 1].CharCount)
                    );
            }
        }

        public TextProvider()
        {
            this.lines = new List<TextLine<T>>();
            this.lines.Add(new TextLine<T>());
        }

        public void Dispose()
        {
            foreach (var line in this.lines)
            {
                line.Dispose();
            }
        }

        private void EditSingleLine(TextPosition start, TextPosition end, string text)
        {
            if (start.row == end.row)
            {
                this.lines[start.row].Edit(start.col, end.col - start.col, text);
            }
            else
            {
                this.lines[start.row].Edit(
                    start.col,
                    this.lines[start.row].CharCount - start.col,
                    text + this.lines[end.row].GetString(end.col, this.lines[end.row].CharCount - end.col));
                for (int i = end.row; i > start.row; i--)
                {
                    this.lines[i].Dispose();
                    this.lines.RemoveAt(i);
                }
            }
        }

        private void EditMultipleLine(TextPosition start, TextPosition end, string[] texts)
        {
            if (start.row == end.row)
            {
                this.lines.Insert(end.row + 1, new TextLine<T>());
                this.lines[end.row + 1].Edit(0, 0, this.lines[end.row], end.col, this.lines[end.row].CharCount - end.col);
                end.row++;
                end.col = 0;
            }

            int affectedLines = end.row - start.row + 1;
            if (affectedLines > texts.Length)
            {
                int startRow = end.row - (affectedLines - texts.Length);
                for (int i = end.row - 1; i >= startRow; i--)
                {
                    this.lines[i].Dispose();
                }
                this.lines.RemoveRange(startRow, end.row - startRow);
                end.row = startRow;
            }
            else if (affectedLines < texts.Length)
            {
                int count = texts.Length - affectedLines;
                TextLine<T>[] allocatedLines = new TextLine<T>[count];
                for (int i = 0; i < count; i++)
                {
                    allocatedLines[i] = new TextLine<T>();
                }
                this.lines.InsertRange(end.row, allocatedLines);
                end.row += count;
            }

            this.lines[start.row].Edit(start.col, this.lines[start.row].CharCount - start.col, texts[0]);
            this.lines[end.row].Edit(0, end.col, texts[texts.Length - 1]);
            for (int i = start.row + 1; i < end.row; i++)
            {
                this.lines[i].Edit(0, this.lines[i].CharCount, texts[i - start.row]);
            }
        }

        public void Edit(TextPosition start, TextPosition end, params string[] texts)
        {
            if (texts.Length == 0)
            {
                EditSingleLine(start, end, "");
            }
            else if (texts.Length == 1)
            {
                EditSingleLine(start, end, texts[0]);
            }
            else
            {
                EditMultipleLine(start, end, texts);
            }
        }

        public string GetString(TextPosition start, TextPosition end)
        {
            if (start.row == end.row)
            {
                return this.lines[start.row].GetString(start.col, end.col - start.col);
            }
            else
            {
                StringBuilder builder = new StringBuilder();
                builder.AppendLine(this.lines[start.row].GetString(start.col, this.lines[start.row].CharCount - start.col));
                for (int i = start.row + 1; i < end.row; i++)
                {
                    builder.AppendLine(this.lines[i].Text);
                }
                builder.Append(this.lines[end.row].GetString(0, end.col));
                return builder.ToString();
            }
        }
    }
}
