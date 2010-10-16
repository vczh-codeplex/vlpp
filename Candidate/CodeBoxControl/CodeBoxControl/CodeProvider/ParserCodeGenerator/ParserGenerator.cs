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
                int labelCounter = 0;
                sb.AppendLine("        public static " + rule.NodeType.FullName + " Parse" + rule.RuleName + "(List<CodeToken> tokens, ref int currentToken)");
                sb.AppendLine("        {");
                sb.AppendLine("            " + GetTypeFullName(rule.NodeType) + " result = default(" + GetTypeFullName(rule.NodeType) + ");");
                sb.Append(CodeGenerator.GenerateCode(rule.NodeType, rule.Content, "            ", 0, 0, "result", "currentToken", ref labelCounter));
                if (typeof(CodeNode).IsAssignableFrom(rule.NodeType))
                {
                    sb.AppendLine("            if (result == null) result = CodeNode.Create<" + GetTypeFullName(rule.NodeType) + ">();");
                }
                foreach (MemberNode memberNode in MemberCollector.GetMembers(rule.Content))
                {
                    sb.AppendLine("            result." + memberNode.Member + " = " + memberNode.Member + "Member0;");
                }
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

        class NodeElementTypeRetriver : IParserNodeVisitor
        {
            private Type nodeType = null;

            public static Type GetNodeType(ParserNode node)
            {
                NodeElementTypeRetriver retriver = new NodeElementTypeRetriver();
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

        class NodeResultTypeRetriver : IParserNodeVisitor
        {
            private Type nodeType = null;

            public static Type GetNodeType(ParserNode node)
            {
                NodeResultTypeRetriver retriver = new NodeResultTypeRetriver();
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
                nodeType = typeof(CodeNodeList<>).MakeGenericType(node.NodeType);
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

        class SequenceNodeFlatter : IParserNodeVisitor
        {
            private List<ParserNode> nodes = new List<ParserNode>();

            public static List<ParserNode> Flat(SequenceNode node)
            {
                SequenceNodeFlatter flatter = new SequenceNodeFlatter();
                node.Accept(flatter);
                return flatter.nodes;
            }

            public void Visit(ChoiceNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(LeftRecursionNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ListNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(MemberNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ReturnNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(RuleNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(SequenceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(TokenNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(TokenContentNode node)
            {
                this.nodes.Add(node);
            }
        }

        class ChoiceNodeFlatter : IParserNodeVisitor
        {
            private List<ParserNode> nodes = new List<ParserNode>();

            public static List<ParserNode> Flat(ChoiceNode node)
            {
                ChoiceNodeFlatter flatter = new ChoiceNodeFlatter();
                node.Accept(flatter);
                return flatter.nodes;
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ListNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(MemberNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ReturnNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(RuleNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(SequenceNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(TokenNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(TokenContentNode node)
            {
                this.nodes.Add(node);
            }
        }

        class ReturnNodeDetector : IParserNodeVisitor
        {
            private ReturnNode discovered = null;

            public static ReturnNode Discover(ParserNode node)
            {
                ReturnNodeDetector detector = new ReturnNodeDetector();
                node.Accept(detector);
                return detector.discovered;
            }

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
            }

            public void Visit(MemberNode node)
            {
            }

            public void Visit(ReturnNode node)
            {
                this.discovered = node;
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
        }

        class CodeGenerator : IParserNodeVisitor
        {
            private string returnVariable = "";
            private string identation = "";
            private int level = 0;
            private string indexVariable = "";
            private Type nodeType = null;
            private int labelCounter = 0;
            private StringBuilder sb = new StringBuilder();

            public static string GenerateCode(Type nodeType, ParserNode node, string identation, int level, int previousLevel, string returnVariable, string indexVariable, ref int labelCounter)
            {
                CodeGenerator generator = new CodeGenerator();
                generator.identation = identation;
                generator.level = level;
                generator.returnVariable = returnVariable;
                generator.indexVariable = indexVariable;
                generator.nodeType = nodeType;
                generator.labelCounter = labelCounter;

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
                labelCounter = generator.labelCounter;
                return generator.sb.ToString();
            }

            private void GenerateAssignCode(Type nodeType, Type memberNodeType, string oldReturnVariable, string newReturnVariable)
            {
                if (oldReturnVariable != "")
                {
                    if (memberNodeType == typeof(CodeToken))
                    {
                        if (nodeType == typeof(string))
                        {
                            sb.AppendLine(identation + "        " + oldReturnVariable + " = " + newReturnVariable + ".Value;");
                        }
                        else
                        {
                            sb.AppendLine(identation + "        " + oldReturnVariable + " = " + GetTypeFullName(nodeType) + ".Parse(" + newReturnVariable + ".Value);");
                        }
                    }
                    else
                    {
                        sb.AppendLine(identation + "        " + oldReturnVariable + " = " + newReturnVariable + ";");
                    }
                }
            }

            public void Visit(ChoiceNode node)
            {
                List<ParserNode> branches = ChoiceNodeFlatter.Flat(node);
                ReturnNode returnNode = ReturnNodeDetector.Discover(node);
                Type returnElementNodeType = returnNode == null ? null : NodeElementTypeRetriver.GetNodeType(returnNode.Content);
                Type returnResultNodeType = returnNode == null ? null : NodeResultTypeRetriver.GetNodeType(returnNode.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = returnNode != null ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelSuccessName = "LABEL_SUCCESS_" + (this.labelCounter++).ToString();
                string labelFailName = "LABEL_FAIL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (returnNode != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnResultNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = -1;");
                foreach (ParserNode branch in branches)
                {
                    bool branchHasReturnNode = ReturnNodeDetector.Discover(branch) != null;
                    sb.AppendLine(identation + "    " + newIndexVariable + " = " + this.indexVariable + ";");
                    sb.Append(CodeGenerator.GenerateCode(this.nodeType, branch, identation + "    ", newLevel, this.level, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, ref this.labelCounter));
                    sb.AppendLine(identation + "    if (" + this.indexVariable + " != " + newIndexVariable + ")");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "        goto " + labelSuccessName + ";");
                    sb.AppendLine(identation + "    }");
                }
                sb.AppendLine(identation + "    goto " + labelFailName + ";");
                sb.AppendLine(identation + "    " + labelSuccessName + ":;");
                if (returnNode != null)
                {
                    GenerateAssignCode(null, returnResultNodeType, this.returnVariable, newReturnVariable);
                }
                sb.AppendLine(identation + "    " + labelFailName + ":;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(ListNode node)
            {
                Type listType = typeof(CodeNodeList<>).MakeGenericType(node.NodeType);
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + this.returnVariable + " = new " + GetTypeFullName(listType) + "();");
                }
                int newLevel = this.level + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string copiedIndexVariable = "currentIndexCopy" + newLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(node.NodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(node.NodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(node.NodeType, node.Item, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (" + this.indexVariable + " == " + newIndexVariable + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "        " + this.returnVariable + ".Add(" + newReturnVariable + ");");
                }
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "    while (true)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        int " + copiedIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(NodeResultTypeRetriver.GetNodeType(node.Separator), node.Separator, identation + "        ", newLevel, this.level, "", newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "        if (" + copiedIndexVariable + " != " + newIndexVariable + ")");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + copiedIndexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        else");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            goto " + labelName + ";");
                sb.AppendLine(identation + "        }");
                sb.Append(CodeGenerator.GenerateCode(node.NodeType, node.Item, identation + "        ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "        if (" + copiedIndexVariable + " != " + newIndexVariable + ")");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + copiedIndexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        else");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            goto " + labelName + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + copiedIndexVariable + ";");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "        " + this.returnVariable + ".Add(" + newReturnVariable + ");");
                }
                sb.AppendLine(identation + "    }");
                sb.Append(identation + "    " + labelName + ":;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(MemberNode node)
            {
                Type memberElementNodeType = NodeElementTypeRetriver.GetNodeType(node.Content);
                Type memberResultNodeType = NodeResultTypeRetriver.GetNodeType(node.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = "result" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                sb.AppendLine(identation + "{");
                sb.AppendLine(identation + "    " + GetTypeFullName(memberResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberResultNodeType) + ");");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(memberElementNodeType, node.Content, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (" + this.indexVariable + " != " + newIndexVariable + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                GenerateAssignCode(nodeType.GetProperty(node.Member).PropertyType, memberResultNodeType, node.Member + "Member" + this.level.ToString(), newReturnVariable);
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "}");
            }

            public void Visit(ReturnNode node)
            {
                Type memberElementNodeType = NodeElementTypeRetriver.GetNodeType(node.Content);
                Type memberResultNodeType = NodeResultTypeRetriver.GetNodeType(node.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = "result" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                sb.AppendLine(identation + "{");
                sb.AppendLine(identation + "    " + GetTypeFullName(memberResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberResultNodeType) + ");");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(memberElementNodeType, node.Content, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (" + this.indexVariable + " != " + newIndexVariable + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                GenerateAssignCode(null, memberResultNodeType, this.returnVariable, newReturnVariable);
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "}");
            }

            public void Visit(RuleNode node)
            {
                if (this.returnVariable == "")
                {
                    sb.AppendLine(identation + "Parse" + node.RuleName + "(tokens, ref " + this.indexVariable + ");");
                }
                else
                {
                    sb.AppendLine(identation + this.returnVariable + " = Parse" + node.RuleName + "(tokens, ref " + this.indexVariable + ");");
                }
            }

            public void Visit(SequenceNode node)
            {
                List<ParserNode> branches = SequenceNodeFlatter.Flat(node);
                ReturnNode returnNode = ReturnNodeDetector.Discover(node);
                Type returnElementNodeType = returnNode == null ? null : NodeElementTypeRetriver.GetNodeType(returnNode.Content);
                Type returnResultNodeType = returnNode == null ? null : NodeResultTypeRetriver.GetNodeType(returnNode.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = returnNode != null ? "result" + newLevel.ToString() : "";
                string copiedIndexVariable = "currentIndexCopy" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (returnNode != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnResultNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + copiedIndexVariable + " = " + this.indexVariable + ";");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                foreach (ParserNode branch in branches)
                {
                    bool branchHasReturnNode = ReturnNodeDetector.Discover(branch) != null;
                    sb.Append(CodeGenerator.GenerateCode(this.nodeType, branch, identation + "    ", newLevel, this.level, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, ref this.labelCounter));
                    sb.AppendLine(identation + "    if (" + copiedIndexVariable + " != " + newIndexVariable + ")");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + copiedIndexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "    }");
                    sb.AppendLine(identation + "    else");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        goto " + labelName + ";");
                    sb.AppendLine(identation + "    }");
                }
                if (returnNode != null)
                {
                    GenerateAssignCode(null, returnResultNodeType, this.returnVariable, newReturnVariable);
                }
                sb.AppendLine(identation + "    " + labelName + ":;");
                sb.AppendLine(identation + "}");
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
                sb.AppendLine(identation + "if (" + this.indexVariable + " < tokens.Count && tokens[" + this.indexVariable + "].Value == \"" + node.TokenValue + "\")");
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
