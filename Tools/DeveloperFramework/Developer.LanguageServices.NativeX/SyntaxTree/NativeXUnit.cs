using System;
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

        protected override bool ContainScope
        {
            get
            {
                return true;
            }
        }
    }

    public abstract class NativeXUses : NativeXNode
    {
        public string UnitName { get; set; }
    }

    public abstract class NativeXEditingStatement : NativeXNode
    {
        public abstract CodeNodeList<NativeXStatement> Statements { get; set; }
    }
}
