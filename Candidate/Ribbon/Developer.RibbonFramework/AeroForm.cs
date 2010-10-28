using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Windows.Forms.VisualStyles;

namespace Developer.RibbonFramework
{
    public partial class AeroForm : Form
    {
        public AeroForm()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            OnSizeChanged(new EventArgs());
        }

        protected virtual void InitializeAero()
        {
            if (this.IsAeroFullBorderControlEnabled)
            {
                WindowsAPI.ExtendGlassIntoClientArea(this, this.AeroAreaPadding.Left, this.AeroAreaPadding.Top, this.AeroAreaPadding.Right, this.AeroAreaPadding.Bottom);
            }
        }

        public virtual Padding AeroAreaPadding
        {
            get
            {
                return new Padding(-1, -1, -1, -1);
            }
        }

        public virtual Padding AeroBorderPadding
        {
            get
            {
                return new Padding(10, 10, 10, 10);
            }
        }

        public virtual int AeroBorderCaptionHeight
        {
            get
            {
                return 30;
            }
        }

        public virtual bool AeroFullBorderControl
        {
            get
            {
                return false;
            }
        }

        protected bool IsAeroFullBorderControlEnabled
        {
            get
            {
                return this.AeroFullBorderControl && this.IsAeroEnabled;
            }
        }

        protected bool IsAeroEnabled
        {
            get
            {
                return !this.DesignMode && WindowsAPI.IsCompositionEnabled && Application.VisualStyleState == VisualStyleState.ClientAndNonClientAreasEnabled;
            }
        }

        protected override void WndProc(ref Message m)
        {
            if (!this.IsAeroFullBorderControlEnabled || !this.AeroFullBorderControl)
            {
                base.WndProc(ref m);
                return;
            }
            {
                IntPtr result = IntPtr.Zero;
                if (WindowsAPI.DwmDefWindowProc(m.HWnd, m.Msg, m.WParam, m.LParam, out result))
                {
                    m.Result = result;
                    return;
                }
            }
            switch (m.Msg)
            {
                case WindowsAPI.WM_NCCALCSIZE:
                    {
                        if (m.WParam == (IntPtr)1)
                        {
                            WindowsAPI.NCCALCSIZE_PARAMS p = (WindowsAPI.NCCALCSIZE_PARAMS)Marshal.PtrToStructure(m.LParam, typeof(WindowsAPI.NCCALCSIZE_PARAMS));
                            p.rgrc1 = p.rgrc0;
                            p.rgrc1.Bottom = p.rgrc1.Top + this.AeroBorderPadding.Top + this.AeroBorderCaptionHeight;
                            p.rgrc2.Bottom = p.rgrc2.Top + this.AeroBorderPadding.Top + this.AeroBorderCaptionHeight;
                            Marshal.StructureToPtr(p, m.LParam, false);
                            m.Result = (IntPtr)(WindowsAPI.WVR_REDRAW);
                        }
                        else
                        {
                            m.Result = (IntPtr)0;
                        }
                    }
                    break;
                case WindowsAPI.WM_NCHITTEST:
                    {
                        int point = (int)m.LParam;
                        int x = (point & 0xFFFF) - this.Left;
                        int y = (point >> 16) - this.Top;
                        int w = this.Width;
                        int h = this.Height;
                        int result = 0;

                        if (x < this.AeroBorderPadding.Left)
                        {
                            if (y < this.AeroBorderPadding.Top)
                            {
                                result = WindowsAPI.HTTOPLEFT;
                            }
                            else if (h - y <= this.AeroBorderPadding.Bottom)
                            {
                                result = WindowsAPI.HTBOTTOMLEFT;
                            }
                            else
                            {
                                result = WindowsAPI.HTLEFT;
                            }
                        }
                        else if (w - x <= this.AeroBorderPadding.Right)
                        {
                            if (y < this.AeroBorderPadding.Top)
                            {
                                result = WindowsAPI.HTTOPRIGHT;
                            }
                            else if (h - y <= this.AeroBorderPadding.Bottom)
                            {
                                result = WindowsAPI.HTBOTTOMRIGHT;
                            }
                            else
                            {
                                result = WindowsAPI.HTRIGHT;
                            }
                        }
                        else
                        {
                            if (y < this.AeroBorderPadding.Top)
                            {
                                result = WindowsAPI.HTTOP;
                            }
                            else if (h - y <= this.AeroBorderPadding.Bottom)
                            {
                                result = WindowsAPI.HTBOTTOM;
                            }
                            else if (y < this.AeroBorderPadding.Top + this.AeroBorderCaptionHeight)
                            {
                                result = WindowsAPI.HTCAPTION;
                            }
                            else
                            {
                                result = WindowsAPI.HTCLIENT;
                            }
                        }
                        m.Result = (IntPtr)result;
                    }
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }
    }
}
