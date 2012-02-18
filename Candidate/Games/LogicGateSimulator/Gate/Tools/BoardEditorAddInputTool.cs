using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Gate.BoardComponents;

namespace Gate.Tools
{
    class BoardEditorAddInputTool : IBoardEditorTool
    {
        private bool inputValue;

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
                return (this.inputValue ? "1" : "0") + " Input";
            }
        }

        public BoardEditorAddInputTool(bool inputValue)
        {
            this.inputValue = inputValue;
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorComponentCommand(() => new LogicInput() { InputValue = this.inputValue });
        }
    }
}
