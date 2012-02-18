using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Gate
{
    interface IBoardEditorTool
    {
        Bitmap Icon { get; }
        string Name { get; }
        IBoardEditorCommand CreateCommand();
    }

    class BoardEditorDragViewTool : IBoardEditorTool
    {
        public Bitmap Icon
        {
            get
            {
                return ToolIcons.DragView;
            }
        }

        public string Name
        {
            get
            {
                return "Drag View";
            }
        }

        public IBoardEditorCommand CreateCommand()
        {
            return new BoardEditorDragViewCommand();
        }
    }

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
