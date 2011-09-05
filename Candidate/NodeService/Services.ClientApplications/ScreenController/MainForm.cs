using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NodeService.Endpoints;
using System.IO;
using NodeService;
using NodeService.Protocols;
using System.Threading;

namespace ScreenController
{
    public partial class MainForm : Form
    {
        private INodeEndpointProtocolFactory protocolFactory;
        private IMachineInfoService machineInfoService;
        private Bitmap screen = null;
        private ScreenInfo screenInfo = null;
        private Thread screenRefresher = null;

        public MainForm()
        {
            this.protocolFactory = new HttpProtocolFactory();
            InitializeComponent();
        }

        private void ScreenRefresherThreadProc()
        {
            try
            {
                while (true)
                {
                    DoService(() =>
                    {
                        Thread.Sleep(10);
                        Bitmap oldScreen = null;
                        using (Stream stream = this.machineInfoService.GetScreenImage(0))
                        {
                            Bitmap newScreen = new Bitmap(stream);
                            oldScreen = this.screen;
                            this.screen = newScreen;
                        }
                        if (this.screen != null)
                        {
                            this.Invoke(new MethodInvoker(() =>
                            {
                                if (pictureBoxScreen.Size != this.screen.Size)
                                {
                                    pictureBoxScreen.Visible = false;
                                    pictureBoxScreen.Size = this.screen.Size;
                                    panelScreen.AutoScrollPosition = new Point(0, 0);
                                    pictureBoxScreen.Visible = true;
                                }
                                pictureBoxScreen.Image = this.screen;
                            }));
                        }
                        if (oldScreen != null)
                        {
                            oldScreen.Dispose();
                        }
                    });
                }
            }
            catch (ThreadAbortException)
            {
            }
        }

        private void DoService(Action action)
        {
            if (this.machineInfoService != null)
            {
                try
                {
                    action();
                }
                catch (ThreadAbortException)
                {
                    throw;
                }
                catch (Exception ex)
                {
                    this.machineInfoService.Dispose();
                    this.machineInfoService = null;
                    MessageBox.Show(ex.Message, this.Text);
                }
            }
        }

        private void pictureBoxScreen_MouseDown(object sender, MouseEventArgs e)
        {
            DoService(() =>
            {
                switch (e.Button)
                {
                    case MouseButtons.Left:
                        this.machineInfoService.MouseEvent("leftdown", 0);
                        break;
                    case MouseButtons.Middle:
                        this.machineInfoService.MouseEvent("middledown", 0);
                        break;
                    case MouseButtons.Right:
                        this.machineInfoService.MouseEvent("rightdown", 0);
                        break;
                }
            });
            pictureBoxScreen.Focus();
        }

        private void pictureBoxScreen_MouseUp(object sender, MouseEventArgs e)
        {
            DoService(() =>
            {
                switch (e.Button)
                {
                    case MouseButtons.Left:
                        this.machineInfoService.MouseEvent("leftup", 0);
                        break;
                    case MouseButtons.Middle:
                        this.machineInfoService.MouseEvent("middleup", 0);
                        break;
                    case MouseButtons.Right:
                        this.machineInfoService.MouseEvent("rightup", 0);
                        break;
                }
            });
        }

        private void pictureBoxScreen_MouseMove(object sender, MouseEventArgs e)
        {
            DoService(() =>
            {
                this.machineInfoService.MouseMove(e.X + this.screenInfo.Bounds.Left, e.Y + this.screenInfo.Bounds.Top);
            });
        }

        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            DoService(() =>
            {
                this.machineInfoService.KeyEvent((byte)e.KeyValue, true);
            });
        }

        private void MainForm_KeyUp(object sender, KeyEventArgs e)
        {
            DoService(() =>
            {
                this.machineInfoService.KeyEvent((byte)e.KeyValue, false);
            });
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            var service = protocolFactory.WaitForClient<IMachineInfoService>("http://" + textBoxMachineName.Text + ":9010/", "MachineInfo");
            if (service != null)
            {
                if (this.machineInfoService != null)
                {
                    this.machineInfoService.Dispose();
                    this.machineInfoService = null;
                }
                this.machineInfoService = service;
                this.screenInfo = this.machineInfoService.GetScreenInfo(0);
            }
            else
            {
                MessageBox.Show("Failed to connect.");
            }
            this.Cursor = Cursors.Default;
        }

        private void MainForm_Shown(object sender, EventArgs e)
        {
            this.screenRefresher = new Thread(ScreenRefresherThreadProc);
            this.screenRefresher.Start();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.screenRefresher.Abort();
            this.screenRefresher.Join();
        }
    }

    public interface IMachineInfoService : INodeEndpointClient
    {
        int GetScreenCount();
        ScreenInfo GetScreenInfo(int index);
        Stream GetScreenImage(int index);
        void MouseMove(int x, int y);
        void MouseEvent(string operation, int data);
        void KeyEvent(byte key, bool down);
    }

    [NodeEndpointDataType]
    public class ScreenBounds
    {
        [NodeEndpointDataMember]
        public int Left { get; set; }

        [NodeEndpointDataMember]
        public int Top { get; set; }

        [NodeEndpointDataMember]
        public int Width { get; set; }

        [NodeEndpointDataMember]
        public int Height { get; set; }
    }

    [NodeEndpointDataType]
    public class ScreenInfo
    {
        [NodeEndpointDataMember]
        public string DeviceName { get; set; }

        [NodeEndpointDataMember]
        public bool IsPrimary { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds Bounds { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds WorkingArea { get; set; }
    }
}
