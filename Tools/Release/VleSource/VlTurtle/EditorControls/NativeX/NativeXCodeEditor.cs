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
        private string[] headerFiles = { "Header.syscrnat.NativeX.txt", "Script\\Header.Turtle.NativeX.txt" };

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

            foreach (string header in this.headerFiles)
            {
                if (File.Exists(header))
                {
                    string text = File.ReadAllText(header);
                    controlPanel.AddPredefinedHeader(text);
                }
            }
        }
    }
}
