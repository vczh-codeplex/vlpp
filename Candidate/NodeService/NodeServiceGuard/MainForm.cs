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

namespace NodeServiceGuard
{
    public partial class MainForm : Form
    {
        private GuardServiceEndpointServer server = null;

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
            this.server.Stop();
        }

        private void timerRestart_Tick(object sender, EventArgs e)
        {
            listViewServices.Items.Clear();
            foreach (var service in this.server.SharedData.GuardedServices)
            {
                ListViewItem item = new ListViewItem();
                item.Text = service.Description.Name;
                item.SubItems.Add(service.Token.ToString());
                item.SubItems.Add(service.Description.ExecutablePath);
                item.SubItems.Add(service.Description.Arguments);
                listViewServices.Items.Add(item);
            }
            this.server.SharedData.RestartFailedServices();
        }
    }
}
