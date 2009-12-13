using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal class ShellCommandMenuItem : ShellCommand
    {
        private ToolStripMenuItem menuItem;

        public ShellCommandMenuItem()
        {
            this.menuItem = new ToolStripMenuItem();
            this.menuItem.Click += new EventHandler(menuItem_Click);
        }

        public override Image Icon
        {
            get
            {
                return this.menuItem.Image;
            }
            set
            {
                this.menuItem.Image = value;
            }
        }

        public override bool Checked
        {
            get
            {
                return this.menuItem.Checked;
            }
            set
            {
                this.menuItem.Checked = value;
            }
        }

        public override Keys ShortcutKeys
        {
            get
            {
                return this.menuItem.ShortcutKeys;
            }
            set
            {
                this.menuItem.ShortcutKeys = value;
            }
        }

        public override ToolStripItem MainControl
        {
            get
            {
                return this.menuItem;
            }
        }

        public override ToolStripItem[] Controls
        {
            get
            {
                return new ToolStripItem[] { this.menuItem };
            }
        }

        public override ToolStrip SubContainer
        {
            get
            {
                return this.menuItem.DropDown;
            }
        }

        private void menuItem_Click(object sender, EventArgs e)
        {
            OnExecute();
        }
    }
}
