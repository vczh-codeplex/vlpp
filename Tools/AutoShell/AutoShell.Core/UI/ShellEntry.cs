using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;

namespace AutoShell.UI
{
    public enum ShellEntryHostType
    {
        MenuStrip,
        ToolStrip
    }

    /// <summary>
    /// 命令包
    /// </summary>
    public abstract class ShellEntry
    {
        public class ItemTag
        {
            public ShellItem OwnerItem { get; set; }
            public ShellGroup OwnerGroup { get; set; }
        }

        /// <summary>
        /// 构造命令包
        /// </summary>
        /// <param name="priority">显示优先级</param>
        public ShellEntry(int priority)
        {
            this.Priority = priority;
        }

        /// <summary>
        /// 用户对象
        /// </summary>
        public object Tag { get; set; }
        /// <summary>
        /// 显示优先级
        /// </summary>
        public int Priority { get; private set; }
        /// <summary>
        /// 所属服务容器
        /// </summary>
        public virtual IServiceContainer Services { get; internal set; }

        /// <summary>
        /// 是否空命令包
        /// </summary>
        public abstract bool IsEmpty { get; }
        internal abstract ShellCommand[] CreateCommands(ShellEntryHostType hostType);
        internal abstract void DestroyCommands(ShellCommand[] commands);
    }
}