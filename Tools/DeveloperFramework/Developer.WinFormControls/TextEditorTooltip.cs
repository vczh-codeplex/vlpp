using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public partial class TextEditorTooltip : TextEditorPopupBase
    {
        private Control ownerControl = null;
        private RichContentViewer viewer = null;

        public TextEditorTooltip()
        {
            InitializeComponent();
            this.viewer = new RichContentViewer();
            this.Controls.Add(this.viewer);
        }

        public void Show(Control control, Point locationTop, Point locationBottom, XDocument content)
        {
            if (control == null) return;
            this.ownerControl = control;
            this.viewer.SetContent(content, Screen.FromControl(control).WorkingArea.Width);
            this.viewer.Location = new Point(0, 0);
            this.ClientSize = this.viewer.Size;
            Show(control, locationTop, locationBottom);
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            if (!this.Visible)
            {
                this.viewer.ClearContent();
            }
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);
            this.ownerControl.Focus();
        }
    }
}
