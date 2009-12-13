using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using AutoShell.Services;

namespace AutoShell.UI.ToolWindow
{
    internal class FindResultWindowPane : ToolWindowPane
    {
        private FindResultWindowControl control = new FindResultWindowControl();
        private ShellItem itemOpen = null;
        private ShellItem itemPrevious = null;
        private ShellItem itemNext = null;
        private ShellItem itemClear = null;
        private ShellItem itemStop = null;

        public override Control ToolWindowControl
        {
            get
            {
                return this.control;
            }
        }

        public override Size DefaultSize
        {
            get
            {
                return new Size(500, 200);
            }
        }

        public void SetResults(FindResult[] results)
        {
            this.control.SetResults(results);
            this.Caption = string.Format(Strings.ToolWindowFindResult, results.Length);
        }

        public void UpdateResults(FindResult[] results)
        {
            this.control.UpdateResults(results);
        }

        protected override void OnInitialize()
        {
            this.control.UIShell = this.Services.QueryService<IUIShell>();
            this.Caption = string.Format(Strings.ToolWindowFindResult, 0);
            this.Icon = Icons.FindResultWindow;

            ShellGroup toolBarGroup = new ShellGroup(0x0000);
            ShellGroup openGroup = new ShellGroup(0x0000);
            ShellGroup navigationGroup = new ShellGroup(0x0001);
            ShellGroup clearGroup = new ShellGroup(0x0002);
            ShellGroup stopGroup = new ShellGroup(0x0003);

            toolBarGroup.AddEntry(openGroup);
            toolBarGroup.AddEntry(navigationGroup);
            toolBarGroup.AddEntry(clearGroup);
            toolBarGroup.AddEntry(stopGroup);

            this.itemOpen = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemOpen.Icon = Icons.FileOpen;
            this.itemOpen.ToolTipText = Strings.ToolTipOpenFindResult;
            this.itemOpen.Executed += new EventHandler(this.control.itemOpen_Executed);
            this.itemOpen.QueryStatus += new EventHandler(this.control.itemOpen_QueryStatus);
            openGroup.AddEntry(this.itemOpen);

            this.itemPrevious = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemPrevious.Icon = Icons.Left;
            this.itemPrevious.ToolTipText = Strings.ToolTipPreviousFindResult;
            this.itemPrevious.Executed += new EventHandler(this.control.itemPrevious_Executed);
            this.itemPrevious.QueryStatus += new EventHandler(this.control.itemPrevious_QueryStatus);
            navigationGroup.AddEntry(this.itemPrevious);

            this.itemNext = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemNext.Icon = Icons.Right;
            this.itemNext.ToolTipText = Strings.ToolTipNextFindResult;
            this.itemNext.Executed += new EventHandler(this.control.itemNext_Executed);
            this.itemNext.QueryStatus += new EventHandler(this.control.itemNext_QueryStatus);
            navigationGroup.AddEntry(this.itemNext);

            this.itemClear = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemClear.Icon = Icons.Clear;
            this.itemClear.ToolTipText = Strings.ToolTipClearFindResult;
            this.itemClear.Executed += new EventHandler(itemClear_Executed);
            this.itemClear.QueryStatus += new EventHandler(this.control.itemClear_QueryStatus);
            clearGroup.AddEntry(this.itemClear);

            this.itemStop = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemStop.Icon = Icons.Stop;
            this.itemStop.ToolTipText = Strings.ToolTipStopBackgroundFind;
            this.itemStop.Executed += new EventHandler(itemStop_Executed);
            this.itemStop.QueryStatus += new EventHandler(this.control.itemStop_QueryStatus);
            stopGroup.AddEntry(this.itemStop);

            this.ToolBarGroup = toolBarGroup;
        }

        protected override void OnOpening()
        {
            this.Services.QueryService<IUIShell>().QueryUIStatus();
        }

        protected override void OnClosing(ref bool cancel)
        {
        }

        protected override void OnClosed()
        {
        }

        protected override void OnQueryStatus()
        {
        }

        public void itemClear_Executed(object sender, EventArgs e)
        {
            SetResults(new FindResult[] { });
        }

        public void itemStop_Executed(object sender, EventArgs e)
        {
        }
    }
}
