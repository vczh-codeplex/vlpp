using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NodeService.Endpoints;
using NodeService;
using NodeService.Protocols;

namespace CommClient
{
    public partial class MainForm : Form
    {
        private ICommunicationService service = null;

        public MainForm()
        {
            InitializeComponent();
            this.textBoxDisplayName.Text = Environment.MachineName + "[" + (new Random().Next()) + "]";
            this.buttonChange.Enabled = false;
            this.buttonSend.Enabled = false;
        }

        public void PrintMessage(string message)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke(new MethodInvoker(() => PrintMessage(message)));
            }
            else
            {
                textBoxMessages.Text += message + "\r\n";
                textBoxMessages.Select(textBoxMessages.Text.Length, 0);
                textBoxMessage.ScrollToCaret();
            }
        }

        private void buttonLogIn_Click(object sender, EventArgs e)
        {
            if (this.service != null)
            {
                this.service.Provider.Disconnect();
                this.service = null;
            }

            INodeEndpointProtocolFactory factory = new NamedPipeProtocolFactory();
            this.service = factory.WaitForClient<ICommunicationService, CommunicationCallback>(
                textBoxServerName.Text,
                "CommunicaitonService",
                new CommunicationCallback(this, textBoxDisplayName.Text)
                );
            if (this.service == null)
            {
                this.buttonChange.Enabled = false;
                this.buttonSend.Enabled = false;
                MessageBox.Show("Log in failed.", this.Text);
            }
            else
            {
                this.buttonChange.Enabled = true;
                this.buttonSend.Enabled = true;
            }
        }

        private void buttonChange_Click(object sender, EventArgs e)
        {
            string oldDisplayName = this.service.Callback.DisplayName;
            this.service.Callback.DisplayName = textBoxDisplayName.Text;
            this.service.ChangeDisplayName(oldDisplayName);
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            string message = textBoxMessage.Text;
            textBoxMessage.Text = "";
            textBoxMessage.Focus();
            this.service.Talk(message);
        }
    }

    [NodeEndpoint]
    public class CommunicationCallback : StrongTypedNodeEndpoint
    {
        public MainForm mainForm { get; set; }
        public string DisplayName { get; set; }

        public CommunicationCallback(MainForm mainForm, string displayName)
        {
            this.EnableAsynchronization = true;
            this.mainForm = mainForm;
            this.DisplayName = displayName;
        }

        [NodeEndpointMethod]
        public string GetDisplayName()
        {
            return this.DisplayName;
        }

        [NodeEndpointMethod]
        public void OnMessage(string message)
        {
            this.mainForm.PrintMessage(message);
        }
    }

    public interface ICommunicationService : IDuplexNodeEndpointClient<CommunicationCallback>
    {
        void Talk(string message);
        void ChangeDisplayName(string oldDisplayName);
    }
}
