using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageServices.NativeX;
using Developer.WinFormControls;
using System.IO;

namespace VlTurtle.EditorControls.NativeX
{
    public partial class NativeXCodeEditor : TextEditorBox
    {
        public NativeXCodeEditor()
        {
            NativeXControlPanel controlPanel = new NativeXControlPanel();
            InitializeComponent();
            this.Colorizer = new NativeXColorizer();
            this.ControlPanel = controlPanel;
            if (this.components == null)
            {
                this.components = new Container();
            }
            this.components.Add(controlPanel);

            if (File.Exists("Header.syscrnat.NativeX.txt"))
            {
                string text = File.ReadAllText("Header.syscrnat.NativeX.txt");
                controlPanel.AddPredefinedHeader(text);
            }
        }
    }
}
