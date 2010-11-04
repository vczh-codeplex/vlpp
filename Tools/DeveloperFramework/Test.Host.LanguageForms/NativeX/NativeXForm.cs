using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageServices.NativeX;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Test.Host.LanguageForms.NativeX
{
    public partial class NativeXForm : Test.Host.LanguageForms.LanguageForm
    {
        public NativeXForm()
        {
            InitializeComponent();
            this.Colorizer = new NativeXColorizer();
            this.ControlPanel = new NativeXControlPanel(this);
            InitializeLanguageForm();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            (this.ControlPanel as NativeXControlPanel).Dispose();
        }

        protected override string CodeFileName
        {
            get
            {
                return "NativeXCode.txt";
            }
        }
    }
}
