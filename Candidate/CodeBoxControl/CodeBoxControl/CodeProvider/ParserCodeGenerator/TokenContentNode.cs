using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class TokenContentNode : ParserNode
    {
        public string[] TokenValues { get; set; }

        internal TokenContentNode()
        {
        }

        public override void Accept(IParserNodeVisitor visitor)
        {
            visitor.Visit(this);
        }
    }
}
