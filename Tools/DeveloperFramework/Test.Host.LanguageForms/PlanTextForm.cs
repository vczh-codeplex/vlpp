using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.ServiceModel;

namespace Test.Host.LanguageForms
{
    public partial class PlanTextForm : Form
    {
        protected ServiceHost ServiceHost { get; private set; }

        public PlanTextForm()
        {
            InitializeComponent();
            TextEditorService.EditorControl = textEditorBox;
            this.ServiceHost = new ServiceHost(typeof(TextEditorService));
            this.ServiceHost.Open();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            this.ServiceHost.Close();
        }
    }
}
