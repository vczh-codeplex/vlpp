using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using NodeService;
using NodeService.Providers;

namespace NodeServiceGuard.ServiceReflectoring
{
    public partial class ServiceTestClientForm : Form
    {
        private Thread connectionThread = null;
        private INodeEndpointProtocolClient protocolClient = null;
        private INodeEndpointClientProvider clientProvider = null;

        public ServiceReflector Reflector { get; set; }

        public ServiceTestClientForm()
        {
            InitializeComponent();
        }

        private void ConnectionThreadProc(string address, string endpointName)
        {
            bool aborted = false;
            try
            {
                INodeEndpointProtocolClient client = this.Reflector.Factory.CreateClient();
                if (client.Connect(address, endpointName, 5000))
                {
                    this.protocolClient = client;
                    this.clientProvider = new ProtocolEnabledClientProvider();
                    this.clientProvider.Protocol = this.protocolClient;
                }
            }
            catch (ThreadAbortException)
            {
                aborted = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Connecting Service");
            }

            if (!aborted)
            {
                this.Invoke(new MethodInvoker(() =>
                {
                    if (this.clientProvider == null)
                    {
                        labelConnectionState.Text = "Failed to connect.";
                        buttonConnect.Enabled = true;
                    }
                    else
                    {
                        labelConnectionState.Text = "Connected to the service.";
                    }
                    this.connectionThread = null;
                }));
            }
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            labelConnectionState.Text = "Connecting...";
            buttonConnect.Enabled = false;
            this.connectionThread = new Thread(() => ConnectionThreadProc(textBoxAddress.Text, textBoxEndpointName.Text));
            this.connectionThread.Start();
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void ServiceTestClientForm_Shown(object sender, EventArgs e)
        {
            textBoxAddress.Text = this.Reflector.SuggestedAddress;
            textBoxProtocolAddress.Text = this.Reflector.ProtocolAddress;
            textBoxEndpointName.Text = this.Reflector.EndpointName;
            buttonConnect_Click(buttonConnect, new EventArgs());
        }

        private void ServiceTestClientForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.connectionThread != null)
            {
                this.connectionThread.Abort();
            }
            if (this.clientProvider != null)
            {
                this.clientProvider.Disconnect();
            }
        }
    }
}
