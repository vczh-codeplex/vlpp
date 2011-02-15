using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimulatorViewer
{
    public partial class ResultForm : Form
    {
        private Bitmap buffer = null;

        public ResultForm(Bitmap buffer)
        {
            this.buffer = buffer;
            InitializeComponent();
            this.ClientSize = buffer.Size;
        }

        private void ResultForm_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.buffer, 0, 0);
        }

        private void ResultForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.buffer.Dispose();
        }
    }
}
