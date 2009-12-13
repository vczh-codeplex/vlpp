using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using AutoShell.Services;

namespace AutoShell.UI.ToolWindow
{
    internal partial class FindWindowControl : UserControl
    {
        private string findText = null;
        private int method = -1;
        private bool matchCase = false;
        private bool matchWholeWord = false;
        private Regex forwardRegex = null;
        private Regex backwordRegex = null;

        private string replaceText = null;
        private bool usePattern = false;
        private RegexPattern pattern = null;

        private List<IFindTargetGroup> groups = null;

        public event EventHandler Find;

        public FindWindowControl()
        {
            InitializeComponent();
            this.comboBoxMethod.SelectedIndex = 0;
        }

        public void UpdateGroups(List<IFindTargetGroup> groups)
        {
            this.groups = new List<IFindTargetGroup>(groups);
            string target = comboBoxTarget.Text;
            comboBoxTarget.Items.Clear();
            comboBoxTarget.Items.AddRange(this.groups.Select(group => (object)group.Name).ToArray());
            comboBoxTarget.Text = target;
            if (comboBoxTarget.Items.Count > 0 && comboBoxTarget.SelectedIndex == -1)
            {
                comboBoxTarget.SelectedIndex = 0;
            }
        }

        public bool EnableReplace
        {
            get
            {
                return textBoxReplace.Enabled;
            }
            set
            {
                textBoxReplace.Enabled = value;
                checkBoxRegexReplace.Enabled = value;
            }
        }

        public bool EnableBackward
        {
            get
            {
                return checkBoxBackward.Enabled;
            }
            set
            {
                checkBoxBackward.Enabled = value;
            }
        }

        public Regex ForwardRegex
        {
            get
            {
                return this.forwardRegex;
            }
        }

        public Regex BackwordRegex
        {
            get
            {
                UpdateRegex();
                return this.backwordRegex;
            }
        }

        public string FindText
        {
            get
            {
                UpdateRegex();
                return textBoxFind.Text;
            }
        }

        public RegexPattern ReplacementPattern
        {
            get
            {
                if (this.pattern == null
                    || this.replaceText != textBoxReplace.Text
                    || this.usePattern != checkBoxRegexReplace.Checked)
                {
                    this.replaceText = textBoxReplace.Text;
                    this.usePattern = checkBoxRegexReplace.Checked;
                    this.pattern = new RegexPattern(this.replaceText, this.usePattern);
                }
                return this.pattern;
            }
        }

        public bool Forward
        {
            get
            {
                return !checkBoxBackward.Checked;
            }
        }

        public IFindTargetGroup Group
        {
            get
            {
                return comboBoxTarget.SelectedIndex == -1 ? null : this.groups[comboBoxTarget.SelectedIndex];
            }
        }

        public void UpdateRegex()
        {
            if (this.forwardRegex == null
                || this.findText != textBoxFind.Text
                || this.method != comboBoxMethod.SelectedIndex
                || this.matchCase != checkBoxCase.Checked
                || this.matchWholeWord != checkBoxWord.Checked)
            {
                this.findText = textBoxFind.Text;
                this.method = comboBoxMethod.SelectedIndex;
                this.matchCase = checkBoxCase.Checked;
                this.matchWholeWord = checkBoxWord.Checked;

                string code = string.Empty;
                switch (this.method)
                {
                    case 0:
                        code = Regex.Escape(this.findText);
                        break;
                    case 1:
                        foreach (char c in this.findText)
                        {
                            switch (c)
                            {
                                case '*':
                                    code += ".*";
                                    break;
                                case '?':
                                    code += ".";
                                    break;
                                default:
                                    code += Regex.Escape(c.ToString());
                                    break;
                            }
                        }
                        break;
                    case 2:
                        code = this.findText;
                        break;
                }
                if (this.matchWholeWord)
                {
                    code = @"(^|\W)" + code + @"($|\W)";
                }
                if (this.matchCase)
                {
                    this.forwardRegex = new Regex(code, RegexOptions.Compiled | RegexOptions.Singleline);
                    this.backwordRegex = new Regex(code, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.RightToLeft);
                }
                else
                {
                    this.forwardRegex = new Regex(code, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.IgnoreCase);
                    this.backwordRegex = new Regex(code, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.RightToLeft | RegexOptions.IgnoreCase);
                }
            }
        }

        private void buttonFind_Click(object sender, EventArgs e)
        {
            if (this.Find != null)
            {
                this.Find(this, new EventArgs());
            }
        }

        private void textBoxFind_TextChanged(object sender, EventArgs e)
        {
            buttonFind.Enabled = textBoxFind.Text != string.Empty && comboBoxTarget.SelectedIndex != -1;
        }

        private void comboBoxTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            buttonFind.Enabled = textBoxFind.Text != string.Empty && comboBoxTarget.SelectedIndex != -1;
        }
    }

    internal class RegexPattern
    {
        private abstract class Item
        {
            public abstract string Generate(Match match);
        }

        private class TextItem : Item
        {
            private string text = null;

            public TextItem(string text)
            {
                this.text = text;
            }

            public override string Generate(Match match)
            {
                return this.text;
            }
        }

        private class GroupItem : Item
        {
            private string name = null;

            public GroupItem(string name)
            {
                this.name = name;
            }

            public override string Generate(Match match)
            {
                return match.Groups[this.name].Value;
            }
        }

        private class CaptureItem : Item
        {
            private int index = -1;

            public CaptureItem(int index)
            {
                this.index = index;
            }

            public override string Generate(Match match)
            {
                if (this.index >= 0 && this.index < match.Captures.Count)
                {
                    return match.Captures[this.index].Value;
                }
                else
                {
                    return string.Empty;
                }
            }
        }

        private List<Item> items = new List<Item>();
        private static Regex pattern = new Regex(@"\{(\$\d+|\w+)\}");

        public RegexPattern(string code, bool usePattern)
        {
            if (usePattern)
            {
                int start = 0;
                foreach (Match match in RegexPattern.pattern.Matches(code))
                {
                    if (match.Index > start)
                    {
                        this.items.Add(new TextItem(code.Substring(start, match.Index - start)));
                    }
                    if (match.Value.StartsWith("{$"))
                    {
                        this.items.Add(new CaptureItem(int.Parse(match.Value.Substring(2, match.Value.Length - 3))));
                    }
                    else
                    {
                        this.items.Add(new GroupItem(match.Value.Substring(1, match.Length - 2)));
                    }
                    start = match.Index + match.Length;
                }
                if (code.Length > start)
                {
                    this.items.Add(new TextItem(code.Substring(start)));
                }
            }
            else
            {
                this.items.Add(new TextItem(code));
            }
        }

        public string Generate(Match match)
        {
            if (this.items.Count == 0)
            {
                return string.Empty;
            }
            else
            {
                return items
                    .Select(item => item.Generate(match))
                    .Aggregate((a, b) => a + b);
            }
        }
    }
}
