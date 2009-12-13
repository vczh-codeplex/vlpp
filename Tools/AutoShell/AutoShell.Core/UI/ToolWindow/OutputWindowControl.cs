using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoShell.UI.ToolWindow
{
    internal partial class OutputWindowControl : UserControl
    {
        public OutputWindowControl()
        {
            InitializeComponent();
        }

        public TextBox OutputBox
        {
            get
            {
                return this.textOutput;
            }
        }
    }
}
