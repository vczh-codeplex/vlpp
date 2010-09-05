using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace CodeForm
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "TextEditorService" in both code and config file together.
    public class TextEditorService : ITextEditorService
    {
        public string GetName()
        {
            return "TextEditorService";
        }
    }
}
