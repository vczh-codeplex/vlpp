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

namespace NodeServiceGuard
{
    public partial class MainForm : Form
    {
        private GuardServiceEndpointServer server = null;

        private void DoService(Action<GuardServiceSharedData.ServiceData> action)
        {
            ListViewItem item = listViewServices.SelectedItems.Cast<ListViewItem>().FirstOrDefault();
            if (item != null)
            {
                Guid token = (Guid)item.Tag;
                GuardServiceSharedData.ServiceData data = this.server.SharedData.GetGuardedService(token);
                if (data != null)
                {
                    this.Cursor = Cursors.WaitCursor;
                    try
                    {
                        action(data);
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
            else
            {
                MessageBox.Show("Please select a service.", this.Text);
            }
        }

        public MainForm()
        {
            InitializeComponent();
            listViewServices
                .GetType()
                .GetProperty("DoubleBuffered", BindingFlags.NonPublic | BindingFlags.Instance)
                .SetValue(listViewServices, true, new object[] { });

            this.server = new GuardServiceEndpointServer();
            this.server.Start();
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
            foreach (var service in this.server.SharedData.GuardedServices)
            {
                ListViewItem item = new ListViewItem();
                item.Tag = service.Token;
                item.Text = service.Description.Name;
                item.SubItems.Add(service.Token.ToString());
                item.SubItems.Add(service.Description.ExecutablePath);
                item.SubItems.Add(service.Description.Arguments);
                listViewServices.Items.Add(item);
            }
            this.server.SharedData.RestartFailedServices();
        }

        private void contextMenuService_Opening(object sender, CancelEventArgs e)
        {
            ListViewItem item = listViewServices.SelectedItems.Cast<ListViewItem>().FirstOrDefault();
            contextMenuServiceViewDescription.Enabled = item != null;
            contextMenuServiceRestart.Enabled = item != null;
            contextMenuServiceStop.Enabled = item != null;
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
    }
}
