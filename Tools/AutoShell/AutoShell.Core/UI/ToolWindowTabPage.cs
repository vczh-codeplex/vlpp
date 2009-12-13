using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AutoShell.UI
{
    internal class ToolWindowTabPage : TabPage, IToolWindowHost
    {
        protected Control paneControl = null;
        protected ToolWindowPane toolWindowPane = null;
        private Image icon = null;
        private MainForm mainForm = null;
        private ContextMenuStrip dockingMenuStrip = null;
        private ShellToolStripController dockingMenuController = null;

        public ToolWindowTabPage(MainForm mainForm)
        {
            this.mainForm = mainForm;
        }

        public ContextMenuStrip DockingMenu
        {
            get
            {
                return this.dockingMenuStrip;
            }
        }

        public Image Icon
        {
            get
            {
                return this.icon;
            }
        }

        public virtual ShellGroup DockingMenuGroup
        {
            get
            {
                return this.toolWindowPane.DockingMenuGroup;
            }
        }

        public ToolWindowPane AssociatedToolWindow
        {
            get
            {
                return this.toolWindowPane;
            }
        }

        protected virtual void PrepareShellItems()
        {
        }

        private void RearrangeImageList()
        {
            TabControl tabControl = this.Parent as TabControl;
            tabControl.ImageList.Images.Clear();
            foreach (ToolWindowTabPage page in tabControl.TabPages)
            {
                Image icon = page.Icon;
                if (icon == null)
                {
                    page.ImageIndex = -1;
                }
                else
                {
                    tabControl.ImageList.Images.Add(page.Icon);
                    page.ImageIndex = tabControl.ImageList.Images.Count - 1;
                }
            }
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);
            if (this.toolWindowPane != null)
            {
                this.toolWindowPane.ToolWindowControl.Focus();
                this.mainForm.SetToolWindowActive(this.toolWindowPane);
            }
        }

        #region IToolWindowHost Members

        void IToolWindowHost.SetIcon(Image icon)
        {
            this.icon = icon;
            RearrangeImageList();
        }

        void IToolWindowHost.SetCaption(string caption)
        {
            this.Text = caption;
        }

        void IToolWindowHost.AttachPane(ToolWindowPane toolWindowPane)
        {
            this.toolWindowPane = toolWindowPane;
            this.paneControl = this.toolWindowPane.GetHostControl();
            this.paneControl.Dock = DockStyle.Fill;
            this.Controls.Add(this.paneControl);
            RearrangeImageList();

            this.toolWindowPane.HideDockHide = false;
            PrepareShellItems();
            this.dockingMenuStrip = new ContextMenuStrip();
            this.dockingMenuController = new ShellToolStripController(this.toolWindowPane.Services, this.dockingMenuStrip, this.DockingMenuGroup);
            this.Parent.ContextMenuStrip = this.DockingMenu;
        }

        void IToolWindowHost.DetachPane()
        {
            this.mainForm.SetToolWindowNotActive(this.toolWindowPane);

            this.dockingMenuController.Dispose();
            this.dockingMenuController = null;
            this.dockingMenuStrip.Dispose();
            this.dockingMenuStrip = null;

            this.toolWindowPane = null;
            this.Controls.Remove(this.paneControl);
            this.Parent.Controls.Remove(this);
            this.Dispose();
            this.mainForm.RearrangeToolWindowAreas();
        }

        void IToolWindowHost.Activate()
        {
            (this.Parent as TabControl).SelectedTab = this;
            this.toolWindowPane.ToolWindowControl.Focus();
            this.mainForm.SetToolWindowActive(this.toolWindowPane);
        }

        #endregion
    }
}
