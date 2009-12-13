using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoShell.UI
{
    internal class DocumentWindowTabPage : ToolWindowTabPage
    {
        private ShellGroup dockingMenuGroup = null;
        private ShellGroup documentMenuGroup = null;
        private ShellGroup toolWindowMenuGroup = null;
        private ShellItem dockClose = null;
        private ShellItem dockCloseOthers = null;

        public DocumentWindowTabPage(MainForm mainForm)
            : base(mainForm)
        {
        }

        public override ShellGroup DockingMenuGroup
        {
            get
            {
                return this.dockingMenuGroup;
            }
        }

        public ShellGroup DocumentMenuGroup
        {
            get
            {
                return this.documentMenuGroup;
            }
        }

        public ShellGroup ToolWindowMenuGroup
        {
            get
            {
                return this.toolWindowMenuGroup;
            }
        }

        protected override void PrepareShellItems()
        {
            base.PrepareShellItems();
            this.toolWindowPane.HideDockHide = true;
            this.dockingMenuGroup = new ShellGroup(0x0000);

            this.documentMenuGroup = new ShellGroup(0x0100);
            this.dockingMenuGroup.AddEntry(this.documentMenuGroup);

            this.dockClose = new ShellItem(ShellItemKind.Button, 0x0100);
            this.dockClose.Text = Strings.MenuTextDockClose;
            this.dockClose.QueryStatus += new EventHandler(dockClose_QueryStatus);
            this.dockClose.Executed += new EventHandler(dockClose_Executed);
            this.documentMenuGroup.AddEntry(this.dockClose);

            this.dockCloseOthers = new ShellItem(ShellItemKind.Button, 0x0100);
            this.dockCloseOthers.Text = Strings.MenuTextDockCloseAllButThis;
            this.dockCloseOthers.Executed += new EventHandler(dockCloseOthers_Executed);
            this.documentMenuGroup.AddEntry(this.dockCloseOthers);

            this.toolWindowMenuGroup = new ShellGroup(0x0200);
            this.toolWindowMenuGroup.AddEntry(base.DockingMenuGroup);
            this.dockingMenuGroup.AddEntry(this.toolWindowMenuGroup);

            IDocumentTabbedWindow window = this.toolWindowPane as IDocumentTabbedWindow;
            if (window != null)
            {
                window.OnPrepareShellItems(this.documentMenuGroup, this.toolWindowMenuGroup);
            }
        }

        void dockClose_QueryStatus(object sender, EventArgs e)
        {
            this.dockClose.Text = string.Format(Strings.MenuTextDockClose, this.Text);
        }

        private void dockClose_Executed(object sender, EventArgs e)
        {
            this.toolWindowPane.Close();
        }

        private void dockCloseOthers_Executed(object sender, EventArgs e)
        {
            List<DocumentWindowTabPage> pages = new List<DocumentWindowTabPage>((this.Parent as TabControl).TabPages.Cast<DocumentWindowTabPage>());
            foreach (DocumentWindowTabPage page in pages)
            {
                if (page != this)
                {
                    page.toolWindowPane.Close();
                }
            }
        }
    }
}
