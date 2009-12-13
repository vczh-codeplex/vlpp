using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AutoShell.UI
{
    public class RegexCodeFormatter : ICodeFormatter
    {
        private CodeFormatManager manager = new CodeFormatManager();
        private List<Regex> rules = new List<Regex>();

        private string text = null;
        private int textLength = 0;
        private int start = 0;
        private int cacheFormat = 0;
        private int cacheLength = 0;

        private Regex[] ruleCache = new Regex[] { };
        private int ruleCount = 0;
        private int ruleOffset = 0;
        private CodeFormat defaultFormat = null;

        public CodeFormat DefaultFormat
        {
            get
            {
                return this.defaultFormat;
            }
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException("value");
                }
                if (this.defaultFormat == null)
                {
                    this.manager.Insert(0, value);
                }
                else
                {
                    this.manager[0] = value;
                }
                this.defaultFormat = value;
                this.ruleOffset = 1;
            }
        }

        public void AddFormat(string regex, CodeFormat format)
        {
            this.rules.Add(new Regex(@"\G(" + regex + @")", RegexOptions.Compiled | RegexOptions.CultureInvariant));
            this.manager.Add(format);
            this.ruleCache = this.rules.ToArray();
            this.ruleCount = this.ruleCache.Length;
        }

        private bool GetTokenInternal(out int format, out int length)
        {
            format = 0;
            length = 0;
            if (this.start < this.textLength)
            {
                for (int i = 0; i < this.ruleCount; i++)
                {
                    Match match = this.ruleCache[i].Match(this.text, this.start);
                    if (match.Success && match.Length > length)
                    {
                        format = i + ruleOffset;
                        length = match.Length;
                    }
                }
                if (length == 0)
                {
                    length = 1;
                }
                this.start += length;
                return true;
            }
            else
            {
                return false;
            }
        }

        #region ICodeFormatter Members

        CodeFormatManager ICodeFormatter.Manager
        {
            get
            {
                return this.manager;
            }
        }

        void ICodeFormatter.BeginFormat(string text)
        {
            this.text = text;
            this.textLength = text.Length;
            this.start = 0;
            this.cacheFormat = 0;
            this.cacheLength = 0;
        }

        void ICodeFormatter.EndFormat()
        {
        }

        bool ICodeFormatter.GetToken(out int format, out int length)
        {
            format = this.cacheFormat;
            length = this.cacheLength;
            this.cacheFormat = 0;
            this.cacheLength = 0;
            int tempFormat = 0;
            int tempLength = 0;
            while (GetTokenInternal(out tempFormat, out tempLength))
            {
                if (format == tempFormat || length == 0)
                {
                    format = tempFormat;
                    length += tempLength;
                }
                else
                {
                    this.cacheFormat = tempFormat;
                    this.cacheLength = tempLength;
                    break;
                }
            }
            return length > 0;
        }

        #endregion
    }
}
