using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using System.Windows.Forms;

namespace AsHost
{
    public class TextFileEditor : TextBasedEditorWindowPane
    {
        private TextBox textBox = null;

        public TextFileEditor()
        {
            this.textBox = new TextBox();
            this.textBox.Multiline = true;
            this.textBox.WordWrap = false;
            this.textBox.ScrollBars = ScrollBars.Both;
            this.textBox.HideSelection = false;
        }

        public override TextBoxBase EditorControl
        {
            get
            {
                return textBox;
            }
        }
    }
}
