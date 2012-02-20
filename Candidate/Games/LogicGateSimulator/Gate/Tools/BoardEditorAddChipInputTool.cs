using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;

namespace Gate.Tools
{
    class BoardEditorAddChipInputTool : IBoardEditorTool
    {
        public Bitmap Icon
        {
            get
            {
                return ToolIcons.Input;
            }
        }

        public string Name
        {
            get
            {
                return "Chip Input";
            }
        }

        public BoardEditorAddChipInputTool()
        {
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicChipInput());
        }
    }
}
