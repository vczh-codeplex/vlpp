using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;

namespace Gate.Tools
{
    class BoardEditorAddWireNodeTool : IBoardEditorTool
    {
        public Bitmap Icon
        {
            get
            {
                return ToolIcons.WireNode;
            }
        }

        public string Name
        {
            get
            {
                return "Wire Node";
            }
        }

        public BoardEditorAddWireNodeTool()
        {
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicWireNode());
        }
    }
}
