using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class ReturnNode : ParserNode
    {
        public ParserNode Content { get; set; }
    }
}
