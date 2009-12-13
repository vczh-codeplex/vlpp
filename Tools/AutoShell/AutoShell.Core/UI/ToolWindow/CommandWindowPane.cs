using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using AutoShell.Services;
using AutoShell.PublicShell;

namespace AutoShell.UI.ToolWindow
{
    internal class CommandWindowPane : ToolWindowPane, ICommandService
    {
        private CommandWindowControl control;
        private SortedDictionary<string, CommandItem> commands = new SortedDictionary<string, CommandItem>();

        public CommandWindowPane()
        {
            this.control = new CommandWindowControl(this);
            this.control.Initialize();
        }

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

        protected override void OnInitialize()
        {
            this.Caption = Strings.ToolWindowCommand;
            this.Icon = Icons.CommandWindow;
            this.Services.RegisterService<ICommandService>(this);
        }

        protected override void OnOpening()
        {
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

        #region ICommandService Members

        public void RegisterCommand(CommandItem command)
        {
            if (string.IsNullOrEmpty(command.Name))
            {
                throw new InvalidOperationException(Strings.ExceptionNullCommandName);
            }
            else if (this.commands.ContainsKey(command.Name.ToUpper()))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedCommand, command.Name));
            }
            else
            {
                this.commands.Add(command.Name.ToUpper(), command);
                this.control.AddCommand(command.Name);
            }
        }

        public string ExecuteCommand(string name, params string[] parameters)
        {
            if (this.commands.ContainsKey(name.ToUpper()))
            {
                string message = this.commands[name.ToUpper()].OnExecute(parameters);
                IOutputService outputService = this.Services.QueryService<IOutputService>();
                outputService.Print(PublicShellCore.LoggerOutputChannel, string.Format(Strings.LogCommandExecuted, this.commands[name.ToUpper()].Name));
                return message;
            }
            else
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionCommandNotExists, name));
            }
        }

        public CommandItem[] GetCommands()
        {
            return this.commands.Values.ToArray();
        }

        #endregion
    }
}
