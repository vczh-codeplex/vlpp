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
using System.Xml.Linq;
using System.IO;

namespace NodeServiceGuard.ServiceReflectoring
{
    public partial class ServiceTestClientForm : Form
    {
        private Thread connectionThread = null;
        private INodeEndpointProtocolClient protocolClient = null;
        private INodeEndpointClientProvider clientProvider = null;
        private INodeEndpointResponse currentResponse = null;
        private byte[] currentResponseStream = null;

        public ServiceReflector Reflector { get; set; }
        public string Host { get; set; }

        public ServiceTestClientForm()
        {
            this.Host = "localhost";
            InitializeComponent();
            buttonSend.Enabled = false;
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
                    this.protocolClient.BeginListen();
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
                        buttonSend.Enabled = true;
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
            textBoxAddress.Text = this.Reflector.GetSuggestedAddress(this.Host);
            textBoxProtocolAddress.Text = this.Reflector.ProtocolAddress;
            textBoxEndpointName.Text = this.Reflector.EndpointName;
            comboBoxMethod.Items.AddRange(this.Reflector.Methods.ToArray());
            if (comboBoxMethod.Items.Count > 0)
            {
                comboBoxMethod.SelectedIndex = 0;
            }
            buttonConnect_Click(buttonConnect, new EventArgs());
        }

        private void ServiceTestClientForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            if (this.connectionThread != null)
            {
                this.connectionThread.Abort();
            }
            if (this.clientProvider != null)
            {
                this.clientProvider.Disconnect();
            }
            this.Cursor = Cursors.Default;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            try
            {
                tabControlResponse.SelectedTab = tabPageResponseXml;
                this.currentResponseStream = null;
                XElement body = XElement.Parse(textBoxParameterXml.Text);
                this.currentResponse = this.clientProvider.Send(comboBoxMethod.SelectedItem.ToString(), body);
                textBoxResponseXml.Text = "Waiting for response...";
                buttonSend.Enabled = false;

                this.currentResponse.RegisterCallback((r) =>
                {
                    try
                    {
                        if (r == this.currentResponse)
                        {
                            this.Invoke(new MethodInvoker(() =>
                            {
                                switch (r.RequestState)
                                {
                                    case RequestState.RaisedException:
                                        {
                                            textBoxResponseXml.Text = r.Exception.Message;
                                        }
                                        break;
                                    case RequestState.ReceivedResponse:
                                        {
                                            textBoxResponseXml.Text = r.Response.ToString();
                                        }
                                        break;
                                    case RequestState.ReceivedStream:
                                        {
                                            using (Stream stream = r.Stream)
                                            {
                                                this.currentResponseStream = stream.ReadAllBytes();
                                                byte[] show = this.currentResponseStream.Take(1024).ToArray();
                                                textBoxResponseXml.Text =
                                                    show.ByteArrayToHex()
                                                    + (this.currentResponseStream.Length > show.Length ? "..." : "")
                                                    + " (" + this.currentResponseStream.Length.ToString() + " bytes)"
                                                    ;
                                            }
                                        }
                                        break;
                                }
                                buttonSend.Enabled = true;
                            }));
                            this.currentResponse = null;
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "Sending Request");
                    }
                });
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, this.Text);
            }
        }

        private void comboBoxMethod_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxMethod.SelectedIndex != -1)
            {
                var method = (ServiceReflector.MethodContract)comboBoxMethod.SelectedItem;
                XElement parameters = new XElement(
                    "Parameters",
                    method
                        .Parameters
                        .Select(p => new XElement(p.Name, " "))
                        .ToArray()
                    );
                textBoxParameterXml.Text = parameters.ToString();
            }
        }

        private void ShowTextBoxResponseString(TextBox textBox, Encoding encoding)
        {
            if (this.currentResponseStream != null)
            {
                try
                {
                    textBox.Text = "[" + encoding.GetType().Name + "]" + encoding.GetString(this.currentResponseStream);
                }
                catch (Exception ex)
                {
                    textBox.Text = "[EXCEPTION]" + ex.Message;
                }
            }
            else
            {
                textBox.Text = "";
            }
        }

        private void tabControlResponse_Selected(object sender, TabControlEventArgs e)
        {
            if (e.TabPage == tabPageResponseString)
            {
                ShowTextBoxResponseString(textBoxResponseStringAscii, Encoding.ASCII);
                ShowTextBoxResponseString(textBoxResponseStringUtf16, Encoding.Unicode);
                ShowTextBoxResponseString(textBoxResponseStringUtf16BE, Encoding.BigEndianUnicode);
                ShowTextBoxResponseString(textBoxResponseStringUtf8, Encoding.UTF8);
            }
            else if (e.TabPage == tabPageResponseImage)
            {
                e.TabPage.AutoScrollPosition = new Point(0, 0);
                pictureBoxResponseImage.Visible = false;
                labelResponseImageException.Visible = false;
                if (this.currentResponseStream != null)
                {
                    try
                    {
                        Image oldImage = pictureBoxResponseImage.Image;
                        Bitmap newImage = null;
                        using (Stream stream = new MemoryStream(this.currentResponseStream, false))
                        {
                            newImage = new Bitmap(stream);
                        }

                        pictureBoxResponseImage.Image = newImage;
                        if (oldImage != null)
                        {
                            oldImage.Dispose();
                        }
                        pictureBoxResponseImage.Bounds = new Rectangle(0, 0, newImage.Width, newImage.Height);
                        pictureBoxResponseImage.Visible = true;
                    }
                    catch (Exception ex)
                    {
                        labelResponseImageException.Text = ex.Message;
                        labelResponseImageException.Visible = true;
                    }
                }
                else
                {
                    labelResponseImageException.Text = "(Empty)";
                    labelResponseImageException.Visible = true;
                }
            }
        }
    }
}
