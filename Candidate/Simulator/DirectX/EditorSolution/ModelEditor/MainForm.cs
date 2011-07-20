using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ModelEditor
{
    public partial class MainForm : Form
    {
        private IntPtr editorWindow = IntPtr.Zero;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.editorWindow = ModelEditorCore.CreateEditorWindow(panelEditorWindow.Handle);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ModelEditorCore.DestroyEditorWindow(this.editorWindow);
            this.editorWindow = IntPtr.Zero;
        }
    }
}
