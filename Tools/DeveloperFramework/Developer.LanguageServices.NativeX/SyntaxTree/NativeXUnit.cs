﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXUnit : NativeXNode
    {
        public string Name { get; set; }
        public abstract CodeNodeList<NativeXUses> UsesUnits { get; set; }
        public abstract CodeNodeList<NativeXDeclaration> Declarations { get; set; }
    }

    public abstract class NativeXUses : NativeXNode
    {
        public string UnitName { get; set; }
    }
}
