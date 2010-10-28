using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Developer.RibbonFramework
{
    class ToolStripAeroFormClientPanel : Panel
    {
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WindowsAPI.WM_ERASEBKGND:
                    if (this.DesignMode)
                    {
                        base.WndProc(ref m);
                    }
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }
    }
}
