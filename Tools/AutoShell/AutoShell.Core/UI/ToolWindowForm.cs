using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace AutoShell.UI
{
    internal partial class ToolWindowForm : Form, IToolWindowHost
    {
        private MainForm mainForm = null;
        private Control paneControl = null;
        private ToolWindowPane toolWindowPane = null;
        private ContextMenuStrip dockingMenuStrip = null;
        private ShellToolStripController dockingMenuController = null;

        public ToolWindowForm(MainForm mainForm)
        {
            InitializeComponent();
            this.mainForm = mainForm;
        }

        public ToolWindowPane AssociatedToolWindow
        {
            get
            {
                return this.toolWindowPane;
            }
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_NCRBUTTONDOWN:
                    break;
                case WM_NCRBUTTONUP:
                    {
                        if (SendMessage(this.Handle, WM_NCHITTEST, 0, m.LParam) == HTCAPTION)
                        {
                            this.dockingMenuStrip.Show(Control.MousePosition);
                        }
                    }
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }

        #region IToolWindowHost Members

        void IToolWindowHost.SetIcon(Image icon)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                icon.Save(stream, ImageFormat.Png);
                using (Bitmap bitmap = new Bitmap(stream))
                {
                    this.Icon = System.Drawing.Icon.FromHandle(bitmap.GetHicon());
                }
            }
        }

        void IToolWindowHost.SetCaption(string caption)
        {
            this.Text = caption;
        }

        void IToolWindowHost.AttachPane(ToolWindowPane toolWindowPane)
        {
            this.toolWindowPane = toolWindowPane;
            this.paneControl = this.toolWindowPane.GetHostControl();
            this.paneControl.Dock = DockStyle.Fill;
            this.Controls.Add(this.paneControl);
            this.Size = this.toolWindowPane.DefaultSize;

            this.toolWindowPane.HideDockHide = false;
            this.dockingMenuStrip = new ContextMenuStrip();
            this.dockingMenuController = new ShellToolStripController(this.toolWindowPane.Services, this.dockingMenuStrip, this.toolWindowPane.DockingMenuGroup);

            int offsetx = this.Width - this.ClientSize.Width;
            int offsety = this.Height - this.ClientSize.Height;
            int width = this.paneControl.MinimumSize.Width;
            int height = this.paneControl.MinimumSize.Height;
            this.MinimumSize = new Size(width + offsetx, height + offsety);
            this.Show(this.mainForm);
        }

        void IToolWindowHost.DetachPane()
        {
            this.mainForm.SetToolWindowNotActive(this.toolWindowPane);

            this.dockingMenuController.Dispose();
            this.dockingMenuController = null;
            this.dockingMenuStrip.Dispose();
            this.dockingMenuStrip = null;

            this.toolWindowPane = null;
            this.Controls.Remove(this.paneControl);
            this.Close();
            this.Dispose();
        }

        void IToolWindowHost.Activate()
        {
            this.BringToFront();
        }

        #endregion

        private void ToolWindowForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.toolWindowPane != null)
            {
                if (!this.toolWindowPane.Close())
                {
                    e.Cancel = true;
                }
            }
        }

        [DllImport("user32")]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, IntPtr lParam);

        [DllImport("user32")]
        private extern static int SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int x, int y, int cx, int cy, uint flag);

        private const uint SWP_NOSIZE = 0x0001;
        private const uint SWP_NOMOVE = 0x0002;
        private const uint SWP_NOACTIVATE = 0x0010;
        private IntPtr HWND_TOP = new IntPtr(0);

        private const int WM_NCHITTEST = 0x0084;
        private const int WM_NCRBUTTONDOWN = 0x00A4;
        private const int WM_NCRBUTTONUP = 0x00A5;
        private const int HTCAPTION = 2;

        private void ToolWindowForm_Activated(object sender, EventArgs e)
        {
            this.toolWindowPane.ToolWindowControl.Focus();
            this.mainForm.SetToolWindowActive(this.toolWindowPane);
        }

        private void ToolWindowForm_Deactivate(object sender, EventArgs e)
        {
            if (this.toolWindowPane != null && !this.Disposing)
            {
                this.mainForm.SetToolWindowNotActive(this.toolWindowPane);
            }
        }
    }
}