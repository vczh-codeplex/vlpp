using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal class ShellCommandButton : ShellCommand
    {
        private ToolStripButton button;

        public ShellCommandButton()
        {
            this.button = new ToolStripButton();
            this.button.Click += new EventHandler(button_Click);
        }

        public override Image Icon
        {
            get
            {
                return this.button.Image;
            }
            set
            {
                this.button.Image = value;
            }
        }

        public override bool Checked
        {
            get
            {
                return this.button.Checked;
            }
            set
            {
                this.button.Checked = value;
            }
        }

        public override ToolStripItem MainControl
        {
            get
            {
                return this.button;
            }
        }

        public override ToolStripItem[] Controls
        {
            get
            {
                return new ToolStripItem[] { this.button };
            }
        }

        public override ToolStrip SubContainer
        {
            get
            {
                return null;
            }
        }

        private void button_Click(object sender, EventArgs e)
        {
            OnExecute();
        }
    }
}