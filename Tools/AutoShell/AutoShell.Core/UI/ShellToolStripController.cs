using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;

namespace AutoShell.UI
{
    /// <summary>
    /// 工具栏条控制器
    /// </summary>
    public class ShellToolStripController : IDisposable
    {
        private IServiceContainer serviceContainer;
        private ToolStrip toolStrip;
        private ShellGroup group;
        private ShellCommand[] commands;

        /// <summary>
        /// 创建工具条控制器
        /// </summary>
        /// <param name="serviceContainer">服务容器</param>
        /// <param name="toolStrip">承载的工具条控件</param>
        /// <param name="group">命令组</param>
        public ShellToolStripController(IServiceContainer serviceContainer, ToolStrip toolStrip, ShellGroup group)
        {
            this.serviceContainer = serviceContainer;
            this.toolStrip = toolStrip;
            this.group = group;
            ShellEntryHostType hostType = ShellEntryHostType.MenuStrip;
            if (this.toolStrip is MenuStrip)
            {
                hostType = ShellEntryHostType.MenuStrip;
                this.toolStrip.VisibleChanged += new EventHandler(toolStrip_VisibleChanged);
            }
            else if (this.toolStrip is ToolStripDropDown)
            {
                hostType = ShellEntryHostType.MenuStrip;
                (this.toolStrip as ToolStripDropDown).Opening += new System.ComponentModel.CancelEventHandler(ShellToolStripController_Opening);
            }
            else
            {
                hostType = ShellEntryHostType.ToolStrip;
                this.toolStrip.VisibleChanged += new EventHandler(toolStrip_VisibleChanged);
            }
            this.commands = this.group.CreateCommands(hostType);
            BuildToolStrip();
            this.serviceContainer.QueryService<IUIShell>().RegisterToolStripController(this);
        }

        /// <summary>
        /// 更新工具条上的命令的状态
        /// </summary>
        public void OnQueryStatus()
        {
            // 在所有情况下都判断避免了Copy等编辑菜单项覆盖了ToolWindow的预定义操作
            OnQueryStatusInternal();
        }

        private void OnQueryStatusInternal()
        {
            foreach (ShellCommand command in this.commands)
            {
                (command.MainControl.Tag as ShellEntry.ItemTag).OwnerItem.OnQueryStatus();
            }
        }

        private void BuildToolStrip()
        {
            ShellGroup lastGroup = null;
            foreach (ShellCommand command in this.commands)
            {
                ShellEntry.ItemTag tag = command.MainControl.Tag as ShellEntry.ItemTag;
                if (lastGroup != null && lastGroup != tag.OwnerGroup && tag.OwnerItem.Visible)
                {
                    this.toolStrip.Items.Add(new ToolStripSeparator());
                }
                foreach (ToolStripItem item in command.Controls)
                {
                    this.toolStrip.Items.Add(item);
                }
                if (tag.OwnerItem.Visible)
                {
                    lastGroup = tag.OwnerGroup;
                }
                command.MainControl.VisibleChanged += new EventHandler(Control_VisibleChanged);
                if (command.SubContainer != null)
                {
                    ShellToolStripController subController = new ShellToolStripController(this.serviceContainer, command.SubContainer, tag.OwnerItem.SubGroup);
                    command.SubItemsBuilt(subController.commands);
                }
            }
        }

        private void Control_VisibleChanged(object sender, EventArgs e)
        {
            ShellGroup lastGroup = null;
            ToolStripSeparator currentSeparator = null;
            for (int i = 0; i < this.toolStrip.Items.Count; i++)
            {
                ToolStripItem item = this.toolStrip.Items[i];
                if (item is ToolStripSeparator)
                {
                    currentSeparator = item as ToolStripSeparator;
                }
                else if (item.Visible)
                {
                    ShellGroup currentGroup = (item.Tag as ShellEntry.ItemTag).OwnerGroup;
                    if (currentGroup == lastGroup)
                    {
                        if (currentSeparator != null)
                        {
                            this.toolStrip.Items.Remove(currentSeparator);
                            currentSeparator.Dispose();
                            i--;
                        }
                    }
                    else if (lastGroup != null)
                    {
                        if (currentSeparator == null)
                        {
                            this.toolStrip.Items.Insert(this.toolStrip.Items.IndexOf(item), new ToolStripSeparator());
                            i++;
                        }
                    }
                    currentSeparator = null;
                    lastGroup = currentGroup;
                }
            }
        }

        private void toolStrip_VisibleChanged(object sender, EventArgs e)
        {
            if (this.toolStrip.Visible)
            {
                OnQueryStatusInternal();
            }
        }

        void ShellToolStripController_Opening(object sender, System.ComponentModel.CancelEventArgs e)
        {
            OnQueryStatusInternal();
        }

        #region IDisposable Members

        public void Dispose()
        {
            this.serviceContainer.QueryService<IUIShell>().UnregisterToolStripController(this);
            this.group.DestroyCommands(this.commands);
        }

        #endregion
    }
}