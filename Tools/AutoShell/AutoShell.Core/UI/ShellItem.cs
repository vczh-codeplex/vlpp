using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace AutoShell.UI
{
    /// <summary>
    /// 命令类型
    /// </summary>
    public enum ShellItemKind
    {
        /// <summary>
        /// 按钮
        /// </summary>
        Button,
        /// <summary>
        /// 子项跟踪按钮
        /// </summary>
        TrackButton,
        /// <summary>
        /// 下拉框
        /// </summary>
        FixedComboBox,
    }

    /// <summary>
    /// 命令项目
    /// </summary>
    public class ShellItem : ShellEntry
    {
        private List<ShellCommand> commands = new List<ShellCommand>();
        private ShellItemKind kind;
        private Image icon = null;
        private bool isChecked = false;
        private string text = string.Empty;
        private bool enabled = true;
        private bool visible = true;
        private string toolTipText = string.Empty;
        private object[] selectableItems = new object[] { };
        private object selectedItem = null;
        private Keys shortcutKeys;
        private ShellGroup subGroup = new ShellGroup(-1);

        public event EventHandler Executed;
        public event EventHandler QueryStatus;

        /// <summary>
        /// 创建命令项目
        /// </summary>
        /// <param name="kind">命令类型</param>
        /// <param name="priority">显示优先级</param>
        public ShellItem(ShellItemKind kind, int priority)
            : base(priority)
        {
            this.kind = kind;
        }

        /// <summary>
        /// 命令类型
        /// </summary>
        public ShellItemKind Kind
        {
            get
            {
                return this.kind;
            }
        }

        /// <summary>
        /// 图标
        /// </summary>
        public Image Icon
        {
            get
            {
                return this.icon;
            }
            set
            {
                this.icon = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.Icon = value;
                }
            }
        }

        /// <summary>
        /// 选中标记
        /// </summary>
        public bool Checked
        {
            get
            {
                return this.isChecked;
            }
            set
            {
                this.isChecked = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.Checked = value;
                }
            }
        }

        /// <summary>
        /// 下拉框中的条目集合
        /// </summary>
        public object[] SelectableItems
        {
            get
            {
                return this.selectableItems;
            }
            set
            {
                this.selectableItems = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.SelectableItems = value;
                }
            }
        }

        /// <summary>
        /// 下拉框中的选中条目
        /// </summary>
        public object SelectedItem
        {
            get
            {
                return this.selectedItem;
            }
            set
            {
                this.selectedItem = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.SelectedItem = value;
                }
            }
        }

        /// <summary>
        /// 命令项文字
        /// </summary>
        public string Text
        {
            get
            {
                return this.text;
            }
            set
            {
                this.text = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.Text = value;
                }
            }
        }

        /// <summary>
        /// 可用标记
        /// </summary>
        public bool Enabled
        {
            get
            {
                return this.enabled;
            }
            set
            {
                this.enabled = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.Enabled = value;
                }
            }
        }

        /// <summary>
        /// 可见标记
        /// </summary>
        public bool Visible
        {
            get
            {
                return this.visible;
            }
            set
            {
                this.visible = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.Visible = value;
                }
            }
        }

        /// <summary>
        /// 提示文字
        /// </summary>
        public string ToolTipText
        {
            get
            {
                return this.toolTipText;
            }
            set
            {
                this.toolTipText = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.ToolTipText = value;
                }
            }
        }

        /// <summary>
        /// 快捷键
        /// </summary>
        public Keys ShortcutKeys
        {
            get
            {
                return this.shortcutKeys;
            }
            set
            {
                this.shortcutKeys = value;
                foreach (ShellCommand command in this.commands)
                {
                    command.ShortcutKeys = value;
                }
            }
        }

        /// <summary>
        /// 子命令组
        /// </summary>
        public ShellGroup SubGroup
        {
            get
            {
                return subGroup;
            }
        }

        internal override ShellCommand[] CreateCommands(ShellEntryHostType hostType)
        {
            ShellEntry.ItemTag tag = new ShellEntry.ItemTag();
            tag.OwnerItem = this;

            ShellCommand command = BuildCommand(hostType);
            command.Icon = this.icon;
            command.Checked = this.isChecked;
            command.Text = this.text;
            command.Enabled = this.enabled;
            command.Visible = this.visible;
            command.ToolTipText = this.toolTipText;
            command.ShortcutKeys = this.shortcutKeys;
            command.SelectableItems = this.selectableItems;
            command.SelectedItem = this.selectedItem;
            foreach (ToolStripItem item in command.Controls)
            {
                item.Tag = tag;
            }
            command.Executed += new EventHandler(command_Executed);
            this.commands.Add(command);
            return new ShellCommand[] { command };
        }

        private ShellCommand BuildCommand(ShellEntryHostType hostType)
        {
            ShellCommand command = null;
            switch (this.kind)
            {
                case ShellItemKind.Button:
                    switch (hostType)
                    {
                        case ShellEntryHostType.MenuStrip:
                            command = new ShellCommandMenuItem();
                            break;
                        case ShellEntryHostType.ToolStrip:
                            if (this.subGroup.IsEmpty)
                            {
                                command = new ShellCommandButton();
                            }
                            else
                            {
                                command = new ShellCommandSplitButton();
                            }
                            break;
                    }
                    break;
                case ShellItemKind.TrackButton:
                    switch (hostType)
                    {
                        case ShellEntryHostType.MenuStrip:
                            command = new ShellCommandTrackMenuItem();
                            break;
                        case ShellEntryHostType.ToolStrip:
                            command = new ShellCommandTrackButton();
                            break;
                    }
                    break;
                case ShellItemKind.FixedComboBox:
                    {
                        ShellCommandComboBox combo = new ShellCommandComboBox();
                        combo.Fixed = true;
                        command = combo;
                    }
                    break;
            }
            return command;
        }

        private void command_Executed(object sender, EventArgs e)
        {
            OnExecute();
        }

        internal override void DestroyCommands(ShellCommand[] commands)
        {
            for (int i = 0; i < commands.Length; i++)
            {
                ShellCommand command = commands[i];
                if (this.commands.Contains(command))
                {
                    commands[i] = null;
                    this.commands.Remove(command);
                    command.Dispose();
                    break;
                }
            }
        }

        public override bool IsEmpty
        {
            get
            {
                return false;
            }
        }

        public void OnExecute()
        {
            if (this.Executed != null)
            {
                this.Executed(this, new EventArgs());
            }
        }

        public void OnQueryStatus()
        {
            if (this.QueryStatus != null)
            {
                this.QueryStatus(this, new EventArgs());
            }
        }

        /// <summary>
        /// 设置被激活子项
        /// </summary>
        public void SetActivatedItem(ShellItem item)
        {
            foreach (ShellCommand command in this.commands)
            {
                foreach (ShellCommand activatedCommand in item.commands)
                {
                    command.ActivatedCommand = activatedCommand;
                }
            }
        }
    }
}
