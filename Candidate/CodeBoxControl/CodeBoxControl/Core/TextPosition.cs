using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.Core
{
    public struct TextPosition : IComparable, IComparable<TextPosition>
    {
        public int row;
        public int col;

        public TextPosition(int row, int col)
        {
            this.row = row;
            this.col = col;
        }

        public override string ToString()
        {
            return "Row: " + this.row.ToString() + "; Col: " + this.col.ToString();
        }

        #region Comparison

        public int CompareTo(TextPosition other)
        {
            if (this.row < other.row)
            {
                return -1;
            }
            else if (this.row > other.row)
            {
                return 1;
            }
            else if (this.col < other.col)
            {
                return -1;
            }
            else if (this.col > other.col)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        int IComparable.CompareTo(object obj)
        {
            if (obj == null || !(obj is TextPosition))
            {
                return 1;
            }
            else
            {
                return CompareTo((TextPosition)obj);
            }
        }

        public override bool Equals(object obj)
        {
            return (this as IComparable).CompareTo(obj) == 0;
        }

        public override int GetHashCode()
        {
            return row * 1000 + col;
        }

        public static bool operator <(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) < 0;
        }

        public static bool operator >(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) > 0;
        }

        public static bool operator ==(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) == 0;
        }

        public static bool operator <=(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) <= 0;
        }

        public static bool operator >=(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) >= 0;
        }

        public static bool operator !=(TextPosition p, TextPosition q)
        {
            return p.CompareTo(q) != 0;
        }

        #endregion
    }
}
