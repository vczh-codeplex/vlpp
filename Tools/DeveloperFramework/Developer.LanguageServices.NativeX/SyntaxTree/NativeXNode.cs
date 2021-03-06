﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXNode : CodeNode
    {
    }

    public abstract class NativeXConceptReference : NativeXNode
    {
        public string ReferenceName { get; set; }
    }
}
