using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.DataModel;

namespace AutoShell.UI.Option
{
    [DataSerializable]
    internal class FindOption
    {
        public static readonly Guid OptionKey = new Guid("{53E351AD-9FEA-4358-9C38-234F872D1369}");

        [DataSerializerVisible]
        public bool EditorWindowMaintainFindResult { get; set; }

        [DataSerializerVisible]
        public bool CloseWindowAfterSearching { get; set; }

        public FindOption()
        {
            this.EditorWindowMaintainFindResult = true;
            this.CloseWindowAfterSearching = true;
        }
    }
}
