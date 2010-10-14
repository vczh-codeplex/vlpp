using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class ListNode : ParserNode
    {
        public ParserNode Item { get; set; }
        public ParserNode Separator { get; set; }
        public Type NodeType { get; set; }
    }
}
