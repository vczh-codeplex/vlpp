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
                sb.Append(CodeGenerator.GenerateCode(rule.NodeType, rule.Content, "            ", 0, 0, "result", "currentToken"));
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

        class MemberCollector : IParserNodeVisitor
        {
            public List<MemberNode> members = new List<MemberNode>();

            public static List<MemberNode> GetMembers(ParserNode parserNode)
            {
                MemberCollector collector = new MemberCollector();
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

        class NodeTypeRetriver : IParserNodeVisitor
        {
            private Type nodeType = null;

            public static Type GetNodeType(ParserNode node)
            {
                NodeTypeRetriver retriver = new NodeTypeRetriver();
                node.Accept(retriver);
                return retriver.nodeType;
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(LeftRecursionNode node)
            {
                nodeType = node.NodeType;
            }

            public void Visit(ListNode node)
            {
                nodeType = node.NodeType;
            }

            public void Visit(MemberNode node)
            {
            }

            public void Visit(ReturnNode node)
            {
            }

            public void Visit(RuleNode node)
            {
                nodeType = node.NodeType;
            }

            public void Visit(SequenceNode node)
            {
            }

            public void Visit(TokenNode node)
            {
                nodeType = typeof(CodeToken);
            }

            public void Visit(TokenContentNode node)
            {
                nodeType = typeof(CodeToken);
            }
        }

        class CodeGenerator : IParserNodeVisitor
        {
            private string returnVariable = "";
            private string identation = "";
            private int level = 0;
            private string indexVariable = "";
            private Type nodeType = null;
            private StringBuilder sb = new StringBuilder();

            public static string GenerateCode(Type nodeType, ParserNode node, string identation, int level, int previousLevel, string returnVariable, string indexVariable)
            {
                CodeGenerator generator = new CodeGenerator();
                generator.identation = identation;
                generator.level = level;
                generator.returnVariable = returnVariable;
                generator.indexVariable = indexVariable;
                generator.nodeType = nodeType;

                foreach (MemberNode memberNode in MemberCollector.GetMembers(node))
                {
                    Type memberType = nodeType.GetProperty(memberNode.Member).PropertyType;
                    generator.sb.AppendLine(identation + GetTypeFullName(memberType) + " " + memberNode.Member + "Member" + level.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                }
                node.Accept(generator);
                if (previousLevel != level)
                {
                    foreach (MemberNode memberNode in MemberCollector.GetMembers(node))
                    {
                        generator.sb.AppendLine(identation + memberNode.Member + "Member" + previousLevel.ToString() + " = " + memberNode.Member + "Member" + level.ToString() + ";");
                    }
                }
                return generator.sb.ToString();
            }

            private void GenerateAssignCode(Type nodeType, Type memberNodeType, string oldReturnVariable, string newReturnVariable)
            {
                if (memberNodeType == typeof(CodeToken))
                {
                    if (nodeType == typeof(string))
                    {
                        sb.Append(identation + "        " + oldReturnVariable + " = " + newReturnVariable + ".Value;");
                    }
                    else
                    {
                        sb.Append(identation + "        " + oldReturnVariable + " = " + GetTypeFullName(nodeType) + ".Parse(" + newReturnVariable + ".Value);");
                    }
                }
                else
                {
                    sb.Append(identation + "        " + oldReturnVariable + " = " + newReturnVariable + ";");
                }
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
                Type memberNodeType = NodeTypeRetriver.GetNodeType(node.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = "result" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                sb.AppendLine(identation + "{");
                sb.AppendLine(identation + "    " + GetTypeFullName(memberNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberNodeType) + ");");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(memberNodeType, node.Content, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable));
                sb.AppendLine(identation + "    if (" + this.indexVariable + " != " + newIndexVariable + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                GenerateAssignCode(nodeType.GetProperty(node.Member).PropertyType, memberNodeType, node.Member + "Member" + this.level.ToString(), newReturnVariable);
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "}");
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
                sb.AppendLine(identation + "if (" + this.indexVariable + " < tokens.Count && tokens[" + this.indexVariable + "].Id == " + node.TokenId.ToString() + ")");
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + this.returnVariable + " = tokens[" + this.indexVariable + "];");
                }
                sb.AppendLine(identation + "    " + this.indexVariable + "++;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(TokenContentNode node)
            {
                sb.AppendLine(identation + "if (" + this.indexVariable + " < tokens.Count && tokens[" + this.indexVariable + "].Value == " + node.TokenValue + ")");
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + this.returnVariable + " = tokens[" + this.indexVariable + "];");
                }
                sb.AppendLine(identation + "    " + this.indexVariable + "++;");
                sb.AppendLine(identation + "}");
            }
        }
    }
}
