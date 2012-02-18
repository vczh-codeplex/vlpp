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
}
