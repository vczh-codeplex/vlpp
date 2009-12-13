using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal abstract class ShellCommand : IDisposable
    {
        public event EventHandler Executed;

        protected ShellCommand()
        {
        }

        public virtual Image Icon { get; set; }
        public virtual bool Checked { get; set; }
        public virtual object[] SelectableItems { get; set; }
        public virtual object SelectedItem { get; set; }
        public virtual Keys ShortcutKeys { get; set; }
        public virtual ShellCommand ActivatedCommand { get; set; }
        public abstract ToolStripItem MainControl { get; }
        public abstract ToolStripItem[] Controls { get; }
        public abstract ToolStrip SubContainer { get; }

        public virtual string Text
        {
            get
            {
                return this.MainControl.Text;
            }
            set
            {
                this.MainControl.Text = value;
            }
        }

        public virtual bool Enabled
        {
            get
            {
                return this.MainControl.Enabled;
            }
            set
            {
                this.MainControl.Enabled = value;
            }
        }

        public virtual bool Visible
        {
            get
            {
                return this.MainControl.Visible;
            }
            set
            {
                this.MainControl.Visible = value;
            }
        }

        public virtual string ToolTipText
        {
            get
            {
                return this.MainControl.ToolTipText;
            }
            set
            {
                this.MainControl.ToolTipText = value;
            }
        }

        public ShellCommand[] SubItems { get; private set; }

        public virtual void SubItemsBuilt(ShellCommand[] items)
        {
            this.SubItems = items;
        }

        public void OnExecute()
        {
            if (this.Executed != null)
            {
                this.Executed(this, new EventArgs());
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            foreach (ToolStripItem item in this.Controls)
            {
                item.Dispose();
            }
        }

        #endregion
    }
}