using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public class RuleNode : ParserNode
    {
        public ParserNode Content { get; set; }
        public Type NodeType { get; set; }
        public string RuleName { get; set; }

        internal RuleNode()
        {
        }

        public void Infer(ParserNode node)
        {
            this.Content = node;
        }

        public override void Accept(IParserNodeVisitor visitor)
        {
            visitor.Visit(this);
        }
    }
}
