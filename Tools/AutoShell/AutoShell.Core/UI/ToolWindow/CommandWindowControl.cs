using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;
using AutoShell.PublicShell;

namespace AutoShell.UI.ToolWindow
{
    internal partial class CommandWindowControl : UserControl
    {
        private ICommandService commandService;

        public CommandWindowControl(ICommandService commandService)
        {
            InitializeComponent();
            this.commandService = commandService;
        }

        public void Initialize()
        {
            CommandItem help = new CommandItem();
            help.Name = PublicShellCore.Command_Help;
            help.Description = Strings.CommandDescription_Help;
            help.Execute += new CommandHandler(help_Execute);
            this.commandService.RegisterCommand(help);

            CommandItem clear = new CommandItem();
            clear.Name = PublicShellCore.Command_Clear;
            clear.Description = Strings.CommandDescription_Clear;
            clear.Execute += new CommandHandler(clear_Execute);
            this.commandService.RegisterCommand(clear);
        }

        public void AddCommand(string command)
        {
            textCommand.AutoCompleteCustomSource.Add(command);
        }

        private void help_Execute(object sender, CommandEventArgs e)
        {
            foreach (CommandItem item in this.commandService.GetCommands())
            {
                e.Message += item.Name + " -- " + item.Description + "\r\n";
            }
        }

        private void clear_Execute(object sender, CommandEventArgs e)
        {
            textOutput.Text = "";
        }

        private void textCommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string command = textCommand.Text;
                string message = null;
                textCommand.Text = "";
                try
                {
                    message = commandService.ExecuteCommand(command);
                }
                catch (Exception ex)
                {
                    message = ex.GetType().FullName + " : " + ex.Message;
                }
                if (!string.IsNullOrEmpty(message))
                {
                    textOutput.Text += message + "\r\n----------\r\n";
                    textOutput.Select(textOutput.Text.Length, 0);
                    textOutput.ScrollToCaret();
                }
                textCommand.Focus();
            }
        }
    }
}
