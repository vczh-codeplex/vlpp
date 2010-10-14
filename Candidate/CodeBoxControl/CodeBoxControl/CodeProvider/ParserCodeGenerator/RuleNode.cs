using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public class RuleNode : ParserNode
    {
        public ParserNode Content { get; set; }
        public Type NodeType { get; set; }
        public string RuleName { get; set; }

        public void Infer(ParserNode node)
        {
            this.Content = node;
        }
    }
}
