using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal class ShellCommandSplitButton : ShellCommand
    {
        private ToolStripSplitButton button;

        public ShellCommandSplitButton()
        {
            this.button = new ToolStripSplitButton();
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
                return this.button.DropDown;
            }
        }

        private void button_Click(object sender, EventArgs e)
        {
            if (!this.button.DropDown.Visible)
            {
                OnExecute();
            }
        }
    }
}