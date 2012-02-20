using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Gate.BoardComponents;
using System.Drawing;

namespace Gate.Tools
{
    class BoardEditorAddChipOutputTool : IBoardEditorTool
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
                return "Chip Output";
            }
        }

        public BoardEditorAddChipOutputTool()
        {
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicChipOutput());
        }
    }
}
