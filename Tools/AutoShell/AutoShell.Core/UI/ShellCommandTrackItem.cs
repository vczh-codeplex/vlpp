using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.UI
{
    internal class ShellCommandTrackButton : ShellCommandSplitButton
    {
        private ShellCommandTrackItemController controller = null;

        public override ShellCommand ActivatedCommand
        {
            get
            {
                return base.ActivatedCommand;
            }
            set
            {
                base.ActivatedCommand = value;
                this.controller.ActiveCommand(value);
            }
        }

        public override void SubItemsBuilt(ShellCommand[] items)
        {
            base.SubItemsBuilt(items);
            this.controller = new ShellCommandTrackItemController(this);
            if (this.SubItems.Length > 0)
            {
                this.ActivatedCommand = this.SubItems[0];
            }
        }
    }

    internal class ShellCommandTrackMenuItem : ShellCommandMenuItem
    {
        private ShellCommandTrackItemController controller = null;

        public override ShellCommand ActivatedCommand
        {
            get
            {
                return base.ActivatedCommand;
            }
            set
            {
                base.ActivatedCommand = value;
                this.controller.ActiveCommand(value);
            }
        }

        public override void SubItemsBuilt(ShellCommand[] items)
        {
            base.SubItemsBuilt(items);
            this.controller = new ShellCommandTrackItemController(this);
            if (this.SubItems.Length > 0)
            {
                this.ActivatedCommand = this.SubItems[0];
            }
        }
    }

    internal class ShellCommandTrackItemController
    {
        private ShellCommand command = null;
        private int activatedItem = 0;

        public ShellCommandTrackItemController(ShellCommand command)
        {
            this.command = command;
            this.command.Executed += new EventHandler(command_Executed);
            foreach (ShellCommand subCommand in command.SubItems)
            {
                subCommand.Executed += new EventHandler(subCommand_Executed);
            }
            UpdateStatus();
        }

        public void ActiveCommand(ShellCommand command)
        {
            for (int i = 0; i < this.command.SubItems.Length; i++)
            {
                if (this.command.SubItems[i] == command)
                {
                    this.activatedItem = i;
                    break;
                }
            }
            UpdateStatus();
        }

        private void command_Executed(object sender, EventArgs e)
        {
            this.command.SubItems[this.activatedItem].OnExecute();
        }

        private void subCommand_Executed(object sender, EventArgs e)
        {
            ActiveCommand(sender as ShellCommand);
            UpdateStatus();
        }

        private void UpdateStatus()
        {
            ShellCommand subCommand=this.command.SubItems[this.activatedItem];
            if (this.command.Text != string.Empty)
            {
                this.command.Text = subCommand.Text;
            }
            this.command.Icon = subCommand.Icon;
        }
    }
}
