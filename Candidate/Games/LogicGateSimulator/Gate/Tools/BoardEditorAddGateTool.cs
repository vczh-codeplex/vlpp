using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;
using System.Windows.Forms;

namespace Gate.Tools
{
    class BoardEditorAddGateTool : IBoardEditorTool
    {
        private PrimitiveGates gate;

        public Bitmap Icon
        {
            get
            {
                return ToolIcons.Gate;
            }
        }

        public string Name
        {
            get
            {
                return this.gate.ToString().ToUpper() + " Gate";
            }
        }

        public BoardEditorAddGateTool(PrimitiveGates gate)
        {
            this.gate = gate;
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicGate() { GateType = this.gate });
        }
    }
}
