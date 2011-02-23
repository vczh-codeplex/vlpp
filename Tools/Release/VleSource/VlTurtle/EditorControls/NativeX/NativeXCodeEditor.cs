using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageServices.NativeX;
using Developer.WinFormControls;

namespace VlTurtle.EditorControls.NativeX
{
    public partial class NativeXCodeEditor : TextEditorBox
    {
        public NativeXCodeEditor()
        {
            InitializeComponent();
            this.Colorizer = new NativeXColorizer();
            this.ControlPanel = new NativeXControlPanel();
            if (this.components == null)
            {
                this.components = new Container();
            }
            this.components.Add((NativeXControlPanel)this.ControlPanel);
        }
    }
}
