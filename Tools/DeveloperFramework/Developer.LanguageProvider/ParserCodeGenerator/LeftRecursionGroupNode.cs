using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public class LeftRecursionGroupNode : ParserNode
    {
        public class Group
        {
            public string FirstMember { get; set; }
            public ParserNode NextNode { get; set; }
            public Type Type { get; set; }
        }

        public ParserNode FirstNode { get; set; }
        public List<LeftRecursionGroupNode.Group> Groups { get; set; }

        internal LeftRecursionGroupNode()
        {
        }

        public override void Accept(IParserNodeVisitor visitor)
        {
            visitor.Visit(this);
        }
    }
}
