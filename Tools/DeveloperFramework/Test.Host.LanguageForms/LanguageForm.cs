using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Developer.WinFormControls;
using System.IO;
using System.ServiceModel;
using Developer.WinFormControls.Core;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms
{
    public partial class LanguageForm : Form
    {
        protected ServiceHost ServiceHost { get; private set; }
        protected ITextEditorColorizer Colorizer { get; set; }
        protected ITextEditorControlPanel ControlPanel { get; set; }

        public LanguageForm()
        {
            InitializeComponent();
        }

        protected void InitializeLanguageForm()
        {
            if (this.Colorizer != null) textEditorBox.Colorizer = this.Colorizer;
            if (this.ControlPanel != null) textEditorBox.ControlPanel = this.ControlPanel;
            TextEditorService.EditorControl = textEditorBox;
            this.ServiceHost = new ServiceHost(typeof(TextEditorService));
            this.ServiceHost.Open();
        }

        protected virtual string CodeFileName
        {
            get
            {
                return "";
            }
        }

        public string TreeText
        {
            get
            {
                return textTree.Text;
            }
            set
            {
                textTree.Text = value;
            }
        }

        public string StatusText
        {
            get
            {
                return textStatus.Text;
            }
            set
            {
                textStatus.Text = value;
            }
        }

        public string ContextText
        {
            get
            {
                return textContext.Text;
            }
            set
            {
                textContext.Text = value;
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            this.ServiceHost.Close();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            if (File.Exists(this.CodeFileName))
            {
                using (StreamReader reader = new StreamReader(this.CodeFileName))
                {
                    textEditorBox.Text = reader.ReadToEnd();
                }
            }
        }
    }
}
