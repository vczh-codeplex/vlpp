﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class LeftRecursionNode : ParserNode
    {
        public ParserNode First { get; set; }
        public ParserNode Next { get; set; }
        public Type NodeType { get; set; }
    }
}
