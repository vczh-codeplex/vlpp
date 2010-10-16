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
                sb.AppendLine("            " + GetTypeFullName(rule.NodeType) + " result = default(" + GetTypeFullName(rule.NodeType) + ");");
                sb.Append(RuleCodeGenerator.GenerateCode(rule.NodeType, rule.Content, "result", "            ", 0));
                sb.AppendLine("            return result;");
                sb.AppendLine("        }");
                sb.AppendLine();
            }
            sb.AppendLine("    }");
            sb.AppendLine("}");

            return sb.ToString();
        }

        private static string GetTypeFullName(Type type)
        {
            if (type.IsGenericTypeDefinition)
            {
                string name = type.FullName;
                return name.Substring(0, name.IndexOf('`'));
            }
            else if (type.IsGenericType)
            {
                string result = GetTypeFullName(type.GetGenericTypeDefinition()) + "<";
                Type[] arguments = type.GetGenericArguments();
                for (int i = 0; i < arguments.Length; i++)
                {
                    if (i > 0) result += ", ";
                    result += GetTypeFullName(arguments[i]);
                }
                result += ">";
                return result;
            }
            else if (type.IsArray)
            {
                return GetTypeFullName(type.GetElementType()) + "[]";
            }
            else
            {
                return type.FullName;
            }
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

        class RuleMemberCollector : IParserNodeVisitor
        {
            public List<MemberNode> members = new List<MemberNode>();

            public static List<MemberNode> GetMembers(ParserNode parserNode)
            {
                RuleMemberCollector collector = new RuleMemberCollector();
                parserNode.Accept(collector);
                return collector.members;
            }

            #region IParserNodeVisitor Members

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(ListNode node)
            {
                node.Separator.Accept(this);
                node.Item.Accept(this);
            }

            public void Visit(MemberNode node)
            {
                members.Add(node);
                node.Content.Accept(this);
            }

            public void Visit(ReturnNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(RuleNode node)
            {
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

        class RuleCodeGenerator : IParserNodeVisitor
        {
            public string returnVariable = "";
            public string identation = "";
            public int level = 0;
            public StringBuilder sb = new StringBuilder();

            public static string GenerateCode(Type nodeType, ParserNode node, string returnVariable, string identation, int level)
            {
                RuleCodeGenerator generator = new RuleCodeGenerator();
                generator.returnVariable = returnVariable;
                generator.identation = identation;
                generator.level = level;

                foreach (MemberNode memberNode in RuleMemberCollector.GetMembers(node))
                {
                    Type memberType = nodeType.GetProperty(memberNode.Member).PropertyType;
                    generator.sb.AppendLine(identation + GetTypeFullName(memberType) + " " + memberNode.Member + "Member" + level.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                }
                node.Accept(generator);
                return generator.sb.ToString();
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(ListNode node)
            {
            }

            public void Visit(MemberNode node)
            {
            }

            public void Visit(ReturnNode node)
            {
            }

            public void Visit(RuleNode node)
            {
            }

            public void Visit(SequenceNode node)
            {
            }

            public void Visit(TokenNode node)
            {
            }

            public void Visit(TokenContentNode node)
            {
            }
        }
    }
}
