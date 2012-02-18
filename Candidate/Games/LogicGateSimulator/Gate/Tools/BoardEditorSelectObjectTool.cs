using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate.Tools
{
    class BoardEditorSelectObjectTool : IBoardEditorTool
    {
        public Bitmap Icon
        {
            get
            {
                return ToolIcons.SelectObjects;
            }
        }

        public string Name
        {
            get
            {
                return "Select Objects";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            throw new NotImplementedException();
        }
    }
}
