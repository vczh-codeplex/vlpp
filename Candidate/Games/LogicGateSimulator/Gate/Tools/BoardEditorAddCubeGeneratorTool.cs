using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;

namespace Gate.Tools
{
    class BoardEditorAddCubeGeneratorTool : IBoardEditorTool
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
                return "Cube Wave Input";
            }
        }

        public BoardEditorAddCubeGeneratorTool()
        {
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicCubeGenerator());
        }
    }
}
