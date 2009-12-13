using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;
using System.Windows.Forms;

namespace AutoShell.UI
{
    /// <summary>
    /// 命令组
    /// </summary>
    public class ShellGroup : ShellEntry
    {
        private SortedDictionary<int, List<ShellEntry>> entries = new SortedDictionary<int, List<ShellEntry>>();

        /// <summary>
        /// 创建命令组
        /// </summary>
        /// <param name="priority">显示优先级</param>
        public ShellGroup(int priority)
            : base(priority)
        {
        }

        /// <summary>
        /// 往命令组添加命令包
        /// </summary>
        /// <param name="entry"></param>
        public void AddEntry(ShellEntry entry)
        {
            if (this.Services != null)
            {
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                if (uiShell.IsInitialized)
                {
                    throw new InvalidOperationException(Strings.ExceptionAddGroupAfterInitialization);
                }
            }
            if (!this.entries.ContainsKey(entry.Priority))
            {
                this.entries.Add(entry.Priority, new List<ShellEntry>());
            }
            this.entries[entry.Priority].Add(entry);
        }

        /// <summary>
        /// 所属服务容器
        /// </summary>
        public override IServiceContainer Services
        {
            get
            {
                return base.Services;
            }
            internal set
            {
                base.Services = value;
                foreach (List<ShellEntry> entryList in this.entries.Values)
                {
                    foreach (ShellEntry entry in entryList)
                    {
                        entry.Services = value;
                    }
                }
            }
        }

        internal override ShellCommand[] CreateCommands(ShellEntryHostType hostType)
        {
            List<ShellCommand> commands = new List<ShellCommand>();
            foreach (List<ShellEntry> entryList in this.entries.Values)
            {
                foreach (ShellEntry entry in entryList)
                {
                    commands.AddRange(entry.CreateCommands(hostType));
                }
            }
            foreach (ShellCommand command in commands)
            {
                foreach (ToolStripItem item in command.Controls)
                {
                    ShellEntry.ItemTag tag = item.Tag as ShellEntry.ItemTag;
                    if (tag.OwnerGroup == null)
                    {
                        tag.OwnerGroup = this;
                    }
                }
            }
            return commands.ToArray();
        }

        internal override void DestroyCommands(ShellCommand[] commands)
        {
            foreach (List<ShellEntry> entryList in this.entries.Values)
            {
                foreach (ShellEntry entry in entryList)
                {
                    entry.DestroyCommands(commands);
                }
            }
        }

        /// <summary>
        /// 是否空命令组
        /// </summary>
        public override bool IsEmpty
        {
            get
            {
                foreach (List<ShellEntry> entryList in this.entries.Values)
                {
                    foreach (ShellEntry entry in entryList)
                    {
                        if (!entry.IsEmpty)
                        {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
    }
}