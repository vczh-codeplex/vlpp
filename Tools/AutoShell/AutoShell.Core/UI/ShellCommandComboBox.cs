using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal class ShellCommandComboBox : ShellCommand
    {
        private ToolStripLabel label;
        private ToolStripComboBox combo;

        public ShellCommandComboBox()
        {
            this.label = new ToolStripLabel();
            this.combo = new ToolStripComboBox();
            this.combo.SelectedIndexChanged += new EventHandler(combo_SelectedIndexChanged);
        }

        public override string Text
        {
            get
            {
                return this.label.Text;
            }
            set
            {
                this.label.Text = value;
                this.label.Visible = this.combo.Visible && this.label.Text != string.Empty;
            }
        }

        public override bool Enabled
        {
            get
            {
                return this.combo.Enabled;
            }
            set
            {
                this.label.Enabled = value;
                this.combo.Enabled = value;
            }
        }

        public override bool Visible
        {
            get
            {
                return this.combo.Visible;
            }
            set
            {
                this.combo.Visible = value;
                this.label.Visible = value && this.label.Text != string.Empty;
            }
        }

        public override object[] SelectableItems
        {
            get
            {
                object[] result = new object[this.combo.Items.Count];
                for (int i = 0; i < this.combo.Items.Count; i++)
                {
                    result[i] = this.combo.Items[i];
                }
                return result;
            }
            set
            {
                this.combo.Items.Clear();
                this.combo.Items.AddRange(value);
            }
        }

        public override object SelectedItem
        {
            get
            {
                return this.combo.SelectedItem;
            }
            set
            {
                if (value == null)
                {
                    this.combo.SelectedIndex = -1;
                }
                else if (this.combo.Items.Contains(value))
                {
                    this.combo.SelectedIndex = this.combo.Items.IndexOf(value);
                }
                else
                {
                    this.combo.Text = value.ToString();
                }
            }
        }

        public override ToolStripItem MainControl
        {
            get
            {
                return this.combo;
            }
        }

        public override ToolStripItem[] Controls
        {
            get
            {
                return new ToolStripItem[] { this.label, this.combo };
            }
        }

        public override ToolStrip SubContainer
        {
            get
            {
                return null;
            }
        }

        public bool Fixed
        {
            get
            {
                return this.combo.DropDownStyle == ComboBoxStyle.DropDownList;
            }
            set
            {
                if (value)
                {
                    this.combo.DropDownStyle = ComboBoxStyle.DropDownList;
                }
                else
                {
                    this.combo.DropDownStyle = ComboBoxStyle.DropDown;
                }
            }
        }

        void combo_SelectedIndexChanged(object sender, EventArgs e)
        {
            OnExecute();
        }
    }
}