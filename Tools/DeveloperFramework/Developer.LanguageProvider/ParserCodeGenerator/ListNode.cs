using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public class ListNode : ParserNode
    {
        public ParserNode Item { get; set; }
        public ParserNode Separator { get; set; }
        public Type NodeType { get; set; }
        public string[] FinishTokens { get; set; }
        public ListNodeContinueType ContinueType { get; set; }

        internal ListNode()
        {
        }

        public override void Accept(IParserNodeVisitor visitor)
        {
            visitor.Visit(this);
        }
    }

    public enum ListNodeContinueType
    {
        CreateNodeType,
        KeepGoing,
        StopByFinishTokens,
    }
}
