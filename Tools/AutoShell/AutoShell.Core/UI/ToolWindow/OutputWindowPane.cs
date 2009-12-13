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
    internal class OutputWindowPane : ToolWindowPane, IOutputService
    {
        private class ChannelItem
        {
            public Guid ChannelGuid { get; private set; }
            public string ChannelName { get; private set; }

            public ChannelItem(Guid guid, string name)
            {
                this.ChannelGuid = guid;
                this.ChannelName = name;
            }

            public override string ToString()
            {
                return this.ChannelName;
            }
        }

        private OutputWindowControl control = null;
        private SortedList<Guid, string> channelNames = new SortedList<Guid, string>();
        private SortedList<Guid, string> channelMessages = new SortedList<Guid, string>();
        private ShellItem comboChannel = null;
        private bool opening = false;

        public OutputWindowPane()
        {
            this.control = new OutputWindowControl();
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
            this.Caption = Strings.ToolWindowOutput;
            this.Icon = Icons.OutputWindow;
            ShellGroup toolBarGroup = new ShellGroup(0x0000);

            this.comboChannel = new ShellItem(ShellItemKind.FixedComboBox, 0x0000);
            this.comboChannel.Text = Strings.ToolItemOutput;
            this.comboChannel.Executed += new EventHandler(comboChannel_Executed);
            toolBarGroup.AddEntry(this.comboChannel);

            ShellItem buttonClear = new ShellItem(ShellItemKind.Button, 0x0100);
            buttonClear.Icon = Icons.ClearOutputChannel;
            buttonClear.Text = Strings.ToolItemClearChannel;
            buttonClear.ToolTipText = Strings.ToolTipClearChannel;
            buttonClear.Executed += new EventHandler(buttonClear_Executed);
            toolBarGroup.AddEntry(buttonClear);

            this.ToolBarGroup = toolBarGroup;
            RegisterChannel(PublicShellCore.LoggerOutputChannel, Strings.OutputChannelAutoShell);
            this.Services.RegisterService<IOutputService>(this);
        }

        private void comboChannel_Executed(object sender, EventArgs e)
        {
            if (!this.opening)
            {
                if (this.comboChannel.SelectedItem != null)
                {
                    ChannelItem item = this.comboChannel.SelectedItem as ChannelItem;
                    OpenChannel(item.ChannelGuid);
                }
            }
        }

        private void buttonClear_Executed(object sender, EventArgs e)
        {
            if (this.comboChannel.SelectedItem != null)
            {
                ChannelItem item = this.comboChannel.SelectedItem as ChannelItem;
                Clear(item.ChannelGuid);
            }
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

        #region IOutputService Members

        public void RegisterChannel(Guid guid, string name)
        {
            if (this.channelNames.ContainsKey(guid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedOutputChannel, guid));
            }
            else
            {
                this.channelNames.Add(guid, name);
                this.channelMessages.Add(guid, string.Empty);
                this.comboChannel.SelectableItems = this.channelNames.Keys.Select(key => (object)(new ChannelItem(key, this.channelNames[key]))).ToArray();
                OpenChannel(guid);
            }
        }

        public void OpenChannel(Guid guid)
        {
            if (this.channelMessages.ContainsKey(guid))
            {
                this.opening = true;
                string message = this.channelMessages[guid];
                this.comboChannel.SelectedItem = this.comboChannel.SelectableItems[this.channelNames.Keys.IndexOf(guid)];
                this.control.OutputBox.Text = message;
                this.control.OutputBox.Select(message.Length, 0);
                this.control.OutputBox.ScrollToCaret();
                this.opening = false;
            }
            else
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOutputChannelNotExists, guid));
            }
        }

        public void Print(Guid guid, string message)
        {
            if (this.channelMessages.ContainsKey(guid))
            {
                message = this.channelMessages[guid] + message + "\r\n";
                this.channelMessages[guid] = message;
                if ((this.comboChannel.SelectedItem as ChannelItem).ChannelGuid == guid)
                {
                    this.control.OutputBox.Text = message;
                    this.control.OutputBox.Select(message.Length, 0);
                    this.control.OutputBox.ScrollToCaret();
                }
            }
            else
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOutputChannelNotExists, guid));
            }
        }

        public void Clear(Guid guid)
        {
            if (this.channelMessages.ContainsKey(guid))
            {
                this.channelMessages[guid] = string.Empty;
                if ((this.comboChannel.SelectedItem as ChannelItem).ChannelGuid == guid)
                {
                    this.control.OutputBox.Text = string.Empty;
                }
            }
            else
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOutputChannelNotExists, guid));
            }
        }

        #endregion
    }
}
