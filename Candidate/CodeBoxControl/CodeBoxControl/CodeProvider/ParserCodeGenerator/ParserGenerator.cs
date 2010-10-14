using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
{
    public static class ParserGenerator
    {
        public static string GenerateCSharpCode(RuleNode startRule, string namespaceName, string className)
        {
            StringBuilder sb = new StringBuilder();
            List<RuleNode> rules = RuleNodeCollector.GetRules(startRule);

            sb.AppendLine("using System;");
            sb.AppendLine("using System.Collections.Generic;");
            sb.AppendLine("using System.Linq;");
            sb.AppendLine("using System.Text;");
            sb.AppendLine("using CodeBoxControl.CodeProvider;");
            sb.AppendLine();
            sb.AppendLine("namespace " + namespaceName);
            sb.AppendLine("{");
            sb.AppendLine("    public static class " + className);
            sb.AppendLine("    {");
            foreach (RuleNode rule in rules)
            {
                sb.AppendLine("        public static " + rule.NodeType.FullName + " Parse" + rule.RuleName + "(List<CodeToken> tokens, ref int currentToken)");
                sb.AppendLine("        {");
                sb.AppendLine("            throw new NotImplementedException();");
                sb.AppendLine("        }");
                sb.AppendLine();
            }
            sb.AppendLine("    }");
            sb.AppendLine("}");

            return sb.ToString();
        }

        class RuleNodeCollector : IParserNodeVisitor
        {
            public List<RuleNode> rules = new List<RuleNode>();

            public static List<RuleNode> GetRules(RuleNode startRule)
            {
                RuleNodeCollector collector = new RuleNodeCollector();
                startRule.Accept(collector);
                return collector.rules;
            }

            #region IParserNodeVisitor Members

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
                node.First.Accept(this);
                node.Next.Accept(this);
            }

            public void Visit(ListNode node)
            {
                node.Separator.Accept(this);
                node.Item.Accept(this);
            }

            public void Visit(MemberNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ReturnNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(RuleNode node)
            {
                if (!this.rules.Contains(node))
                {
                    this.rules.Add(node);
                    node.Content.Accept(this);
                }
            }

            public void Visit(SequenceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(TokenNode node)
            {
            }

            public void Visit(TokenContentNode node)
            {
            }

            #endregion
        }
    }
}
