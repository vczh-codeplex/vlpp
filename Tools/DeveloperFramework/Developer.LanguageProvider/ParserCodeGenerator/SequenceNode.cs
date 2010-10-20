using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public class SequenceNode : ParserNode
    {
        public ParserNode Left { get; set; }
        public ParserNode Right { get; set; }

        internal SequenceNode()
        {
        }

        public override void Accept(IParserNodeVisitor visitor)
        {
            visitor.Visit(this);
        }
    }
}
