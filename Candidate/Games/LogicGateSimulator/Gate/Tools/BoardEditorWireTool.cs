using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.Tools
{
    class BoardEditorWireTool : IBoardEditorTool
    {
        public Bitmap Icon
        {
            get
            {
                return ToolIcons.Wire;
            }
        }

        public string Name
        {
            get
            {
                return "Wire";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            throw new NotImplementedException();
        }
    }
}
