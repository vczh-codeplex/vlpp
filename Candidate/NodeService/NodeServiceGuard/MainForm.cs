using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using NodeServiceHost;
using NodeServiceHost.GuardService;
using System.Xml.Linq;
using NodeService;
using NodeServiceGuard.ServiceReflectoring;
using System.IO;
using System.Net;

namespace NodeServiceGuard
{
    public partial class MainForm : Form
    {
        private GuardServiceEndpointServer server = null;
        private Guid selectedServerToken = Guid.Empty;

        private void DoService(Guid token, Action<GuardServiceSharedData.ServiceData> action)
        {
            GuardServiceSharedData.ServiceData data = this.server.SharedData.GetGuardedService(token);
            if (data != null)
            {
                this.Cursor = Cursors.WaitCursor;
                try
                {
                    action(data);
                }
                catch (NodeEndpointRequestException e)
                {
                    MessageBox.Show(e.Message, this.Text);
                }
                catch (Exception)
                {
                    MessageBox.Show("The service is down.", this.Text);
                }
                this.Cursor = Cursors.Default;
                return;
            }
            else
            {
                MessageBox.Show("Cannot find the service", this.Text);
            }
        }

        private void DoService(Action<GuardServiceSharedData.ServiceData> action)
        {
            ListViewItem item = listViewServices.SelectedItems.Cast<ListViewItem>().FirstOrDefault();
            if (item != null)
            {
                Guid token = (Guid)item.Tag;
                DoService(token, action);
            }
            else
            {
                MessageBox.Show("Please select a service.", this.Text);
            }
        }

        private void SetListViewServiceInformation(int index, string value)
        {
            ListViewItem item = listViewServiceInformation.Items[index];
            if (item.SubItems.Count == 1)
            {
                item.SubItems.Add(value);
            }
            else
            {
                item.SubItems[1].Text = value;
            }
        }

        private void UpdateServiceProperties()
        {
            GuardServiceSharedData.ServiceData data = this.server.SharedData.GetGuardedService(this.selectedServerToken);
            if (data == null)
            {
                for (int i = 0; i < listViewServiceInformation.Items.Count; i++)
                {
                    SetListViewServiceInformation(i, "");
                }

                buttonStartTracing.Enabled = false;
                buttonStopTracing.Enabled = false;
                buttonReadTracingResult.Enabled = false;
                labelServiceTracing.Text = "Tracing: Unknown";
            }
            else
            {
                SetListViewServiceInformation(0, data.Description.Name);
                SetListViewServiceInformation(1, data.Token.ToString());
                SetListViewServiceInformation(2, data.SemaphoreName);
                SetListViewServiceInformation(3, data.Description.ExecutablePath);
                SetListViewServiceInformation(4, data.Description.Arguments);

                bool tracing = data.Service.Callback.IsTracing();
                buttonStartTracing.Enabled = !tracing;
                buttonStopTracing.Enabled = tracing;
                buttonReadTracingResult.Enabled = true;
                labelServiceTracing.Text = "Tracing: " + (tracing ? "On" : "Off");
            }
        }

        private void EnableDoubleBuffer(Control control)
        {
            control
                .GetType()
                .GetProperty("DoubleBuffered", BindingFlags.NonPublic | BindingFlags.Instance)
                .SetValue(listViewServices, true, new object[] { });
        }

        public MainForm()
        {
            InitializeComponent();
            EnableDoubleBuffer(listViewServices);
            EnableDoubleBuffer(listViewServiceInformation);

            this.server = new GuardServiceEndpointServer(9000);
            this.server.Start();

            this.Text += " (" + this.server.GetHttpListenerAddress("localhost") + ")";
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            this.server.SharedData.StopAll(token =>
            {
                ListViewItem item = listViewServices.Items.Cast<ListViewItem>()
                    .Where(i => token == (Guid)i.Tag)
                    .FirstOrDefault();
                if (item != null)
                {
                    listViewServices.Items.Remove(item);
                    listViewServices.Refresh();
                }
            });
            this.server.Stop();
        }

        private void timerRestart_Tick(object sender, EventArgs e)
        {
            listViewServices.Items.Clear();
            ListViewItem selectedItem = null;
            foreach (var service in this.server.SharedData.GuardedServices)
            {
                ListViewItem item = new ListViewItem();
                item.Tag = service.Token;
                item.Text = service.Description.Name;
                item.SubItems.Add(service.Token.ToString());
                listViewServices.Items.Add(item);
                if (service.Token == this.selectedServerToken)
                {
                    selectedItem = item;
                }
            }
            this.server.SharedData.RestartFailedServices();
            if (selectedItem == null)
            {
                this.selectedServerToken = Guid.Empty;
            }
            else
            {
                listViewServiceInformation.SelectedIndices.Clear();
                listViewServiceInformation.SelectedIndices.Add(selectedItem.Index);
            }
            UpdateServiceProperties();
        }

        private void contextMenuService_Opening(object sender, CancelEventArgs e)
        {
            ListViewItem item = listViewServices.SelectedItems.Cast<ListViewItem>().FirstOrDefault();
            foreach (ToolStripItem menuItem in contextMenuService.Items)
            {
                menuItem.Enabled = item != null;
            }
            timerRestart.Enabled = false;
        }

        private void contextMenuService_Closing(object sender, ToolStripDropDownClosingEventArgs e)
        {
            timerRestart.Enabled = true;
        }

        private void contextMenuServiceViewDescription_Click(object sender, EventArgs e)
        {
            DoService(data =>
            {
                XElement description = data.Service.Callback.GetServiceDescription();
                using (var form = new ServiceDescriptionForm())
                {
                    form.ShowDescription(description);
                }
            });
        }

        private void contextMenuServiceConnect_Click(object sender, EventArgs e)
        {
            XElement description = null;
            DoService(data =>
            {
                description = data.Service.Callback.GetServiceDescription();
            });

            if (description != null)
            {
                try
                {
                    ServiceReflector reflector = new ServiceReflector(description);
                    using (var form = new ServiceTestClientForm())
                    {
                        form.Reflector = reflector;
                        form.ShowDialog();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, this.Text);
                }
            }
        }

        private void contextMenuServiceRestart_Click(object sender, EventArgs e)
        {
            DoService(data =>
            {
                data.Service.Callback.Stop();
            });
        }

        private void contextMenuServiceStop_Click(object sender, EventArgs e)
        {
            DoService(data =>
            {
                this.server.SharedData.Unregister(data.Token);
                data.Service.Callback.Stop();
            });
        }

        private void listViewServices_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.selectedServerToken = Guid.Empty;
            if (listViewServices.SelectedItems.Count > 0)
            {
                DoService(data =>
                {
                    this.selectedServerToken = data.Token;
                });
            }
            UpdateServiceProperties();
        }

        private void buttonStartTracing_Click(object sender, EventArgs e)
        {
            DoService(this.selectedServerToken, data =>
            {
                data.Service.Callback.StartTracing();
                labelServiceTracing.Text = "Tracing: ...";
                buttonStartTracing.Enabled = false;
                buttonStopTracing.Enabled = false;
            });
        }

        private void buttonStopTracing_Click(object sender, EventArgs e)
        {
            DoService(this.selectedServerToken, data =>
            {
                data.Service.Callback.StopTracing();
                labelServiceTracing.Text = "Tracing: ...";
                buttonStartTracing.Enabled = false;
                buttonStopTracing.Enabled = false;
            });
        }

        private void buttonReadTracingResult_Click(object sender, EventArgs e)
        {
            DoService(this.selectedServerToken, data =>
            {
                textBoxTracingResult.Text = data.Service.Callback.GetTracingResult().ToString();
            });
        }

        private void QueryHttp(string url, Action<string> callback, Action callbackEnd)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Method = "GET";
            try
            {
                request.BeginGetResponse((a) =>
                {
                    try
                    {
                        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                        string message = "";

                        using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
                        {
                            message = reader.ReadToEnd();
                        }

                        this.Invoke(new MethodInvoker(() =>
                        {
                            try
                            {
                                callback(message);
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message, this.Text);
                            }
                        }));
                    }
                    catch (Exception ex)
                    {
                        this.Invoke(new MethodInvoker(() =>
                        {
                            MessageBox.Show(ex.Message, this.Text);
                        }));
                    }
                    finally
                    {
                        this.Invoke(new MethodInvoker(() =>
                        {
                            callbackEnd();
                        }));
                    }
                }, null);
            }
            catch (WebException ex)
            {
                MessageBox.Show(ex.Message, this.Text);
            }
        }

        private void buttonConnectRemoteServiceGuard_Click(object sender, EventArgs e)
        {
            string host = textBoxRemoteServiceGuard.Text;
            string urlTemplate = this.server.GetHttpListenerAddress(host) + "/{0}.xml";
            string url = string.Format(urlTemplate, "ServiceList");

            buttonConnectRemoteServiceGuard.Enabled = false;
            listViewRemoteServices.Items.Clear();
            listViewRemoteServices.Enabled = false;

            QueryHttp(
                url,
                message =>
                {
                    XElement serviceList = XElement.Parse(message);
                    foreach (XElement elementService in serviceList.Elements("Service"))
                    {
                        ListViewItem item = new ListViewItem(elementService.Element("Name").Value);
                        item.SubItems.Add(string.Format(urlTemplate, elementService.Element("Token").Value));
                        item.Tag = host;
                        listViewRemoteServices.Items.Add(item);
                    }
                },
                () =>
                {
                    buttonConnectRemoteServiceGuard.Enabled = true;
                    listViewRemoteServices.Enabled = true;
                }
            );
        }

        private void buttonViewRemoteServiceDescription_Click(object sender, EventArgs e)
        {
            ListViewItem item = listViewRemoteServices.SelectedItems[0];
            string url = item.SubItems[1].Text;

            QueryHttp(
                url,
                message =>
                {
                    XElement description = XElement.Parse(message);
                    using (var form = new ServiceDescriptionForm())
                    {
                        form.ShowDescription(description);
                    }
                },
                () => { }
            );
        }

        private void buttonConnectRemoteService_Click(object sender, EventArgs e)
        {
            ListViewItem item = listViewRemoteServices.SelectedItems[0];
            string url = item.SubItems[1].Text;

            QueryHttp(
                url,
                message =>
                {
                    XElement description = XElement.Parse(message);
                    ServiceReflector reflector = new ServiceReflector(description);
                    using (var form = new ServiceTestClientForm())
                    {
                        form.Reflector = reflector;
                        form.ShowDialog();
                    }
                },
                () => { }
            );
        }

        private void listViewRemoteServices_SelectedIndexChanged(object sender, EventArgs e)
        {
            buttonViewRemoteServiceDescription.Enabled = listViewRemoteServices.SelectedIndices.Count > 0;
            buttonConnectRemoteService.Enabled = listViewRemoteServices.SelectedIndices.Count > 0;
        }
    }
}
