using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace VlTurtle.EditorControls
{
    class ClipboardMonitor : IComponent
    {
        private IntPtr thisClipboardViewer;
        private IntPtr nextClipboardViewer;

        public ClipboardMonitor(Control viewer)
        {
            this.thisClipboardViewer = viewer.Handle;
            this.nextClipboardViewer = SetClipboardViewer(this.thisClipboardViewer);
        }

        public void WndProc(ref Message m)
        {
            const int WM_DRAWCLIPBOARD = 0x308;
            const int WM_CHANGECBCHAIN = 0x030D; switch (m.Msg)
            {
                case WM_DRAWCLIPBOARD:
                    InvokeClipboardChanged();
                    SendMessage(nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    break;
                case WM_CHANGECBCHAIN:
                    if (m.WParam == nextClipboardViewer)
                    {
                        nextClipboardViewer = m.LParam;
                    }
                    else
                    {
                        SendMessage(nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    }
                    break;
            }
        }

        private void InvokeClipboardChanged()
        {
            if (this.ClipboardChanged != null)
            {
                this.ClipboardChanged(this, new EventArgs());
            }
        }

        public event EventHandler Disposed;
        public event EventHandler ClipboardChanged;
        public ISite Site { get; set; }

        public void Dispose()
        {
            ChangeClipboardChain(this.thisClipboardViewer, nextClipboardViewer);
            if (this.Disposed != null)
            {
                this.Disposed(this, new EventArgs());
            }
        }

        [DllImport("User32.dll")]
        private static extern IntPtr SetClipboardViewer(IntPtr hWndNewViewer);
        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        private static extern bool ChangeClipboardChain(IntPtr hWndRemove, IntPtr hWndNewNext);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, IntPtr wParam, IntPtr lParam);
    }
}
