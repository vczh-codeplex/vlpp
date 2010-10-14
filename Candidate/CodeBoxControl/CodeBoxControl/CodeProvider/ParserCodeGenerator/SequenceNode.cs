using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class SequenceNode : ParserNode
    {
        public ParserNode Left { get; set; }
        public ParserNode Right { get; set; }
    }
}
