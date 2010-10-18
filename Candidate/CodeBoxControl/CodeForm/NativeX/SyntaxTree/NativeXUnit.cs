using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeForm.NativeX.SyntaxTree
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
