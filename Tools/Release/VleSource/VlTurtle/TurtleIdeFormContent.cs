using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace VlTurtle
{
    public partial class TurtleIdeFormContent : UserControl
    {
        public TurtleIdeFormContent()
        {
            InitializeComponent();

            this.codeEditorNativeX.Text =
                "unit MyTurtleProgram;\r\n\r\n" +
                "uses syscrnat, Turtle;\r\n\r\n" +
                "function void main()\r\n" +
                "{\r\n" +
                "}\r\n";
        }
    }
}
