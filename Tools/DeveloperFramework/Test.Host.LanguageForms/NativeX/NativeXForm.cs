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
        class NativeXFormControlPanel : NativeXControlPanel
        {
            private NativeXForm form = null;
            private int counter = 0;

            public NativeXFormControlPanel(NativeXForm form)
            {
                this.form = form;
            }

            protected override void UpdateUnit(NativeXUnit unit)
            {
                base.UpdateUnit(unit);
                this.form.TreeText = unit == null ? "<NULL>" : unit.ToString();
                this.form.StatusText = (++this.counter).ToString();
            }
        }

        public NativeXForm()
        {
            InitializeComponent();
            this.Colorizer = new NativeXColorizer();
            this.ControlPanel = new NativeXFormControlPanel(this);
            InitializeLanguageForm();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            (this.ControlPanel as NativeXFormControlPanel).Dispose();
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
