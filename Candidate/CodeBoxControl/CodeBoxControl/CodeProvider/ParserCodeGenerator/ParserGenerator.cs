using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.Core;

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
                sb.AppendLine("        public static " + rule.NodeType.FullName + " Parse" + rule.RuleName + "(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)");
                sb.AppendLine("        {");
                sb.AppendLine("            " + GetTypeFullName(typeof(TextPosition)) + " start = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetStartPosition(tokens, currentToken);");
                sb.AppendLine("            " + GetTypeFullName(rule.NodeType) + " result = default(" + GetTypeFullName(rule.NodeType) + ");");
                sb.Append(CodeGenerator.GenerateCode(rule.NodeType, rule.Content, "            ", 0, 0, "result", "currentToken", ref labelCounter));
                sb.AppendLine("            if (parseSuccess)");
                sb.AppendLine("            {");
                if (typeof(CodeNode).IsAssignableFrom(rule.NodeType))
                {
                    sb.AppendLine("                if (result == null) result = CodeNode.Create<" + GetTypeFullName(rule.NodeType) + ">();");
                }
                foreach (string member in MemberCollector.GetMembers(rule.Content))
                {
                    sb.AppendLine("                result." + member + " = " + member + "Member0;");
                }
                sb.AppendLine("                result.Start = start;");
                sb.AppendLine("                result.End = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetEndPosition(tokens, currentToken);");
                sb.AppendLine("            }");
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

            public void Visit(LeftRecursionGroupNode node)
            {
                node.FirstNode.Accept(this);
                foreach (var g in node.Groups)
                {
                    g.NextNode.Accept(this);
                }
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
        }

        class MemberCollector : IParserNodeVisitor
        {
            public List<string> members = new List<string>();

            public static List<string> GetMembers(ParserNode parserNode)
            {
                MemberCollector collector = new MemberCollector();
                parserNode.Accept(collector);
                return collector.members;
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(LeftRecursionGroupNode node)
            {
            }

            public void Visit(ListNode node)
            {
                node.Separator.Accept(this);
                node.Item.Accept(this);
            }

            public void Visit(MemberNode node)
            {
                if (!members.Contains(node.Member))
                {
                    members.Add(node.Member);
                }
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
        }

        class TypeRetriverBase
        {
            private Type nodeType = null;

            protected Type NodeType
            {
                get
                {
                    return this.nodeType;
                }
                set
                {
                    if (this.nodeType == null)
                    {
                        this.nodeType = value;
                    }
                    else
                    {
                        while (!this.nodeType.IsAssignableFrom(value))
                        {
                            this.nodeType = this.nodeType.BaseType;
                        }
                    }
                }
            }
        }

        class ElementTypeRetriver : TypeRetriverBase, IParserNodeVisitor
        {
            public static Type GetNodeType(ParserNode node)
            {
                ElementTypeRetriver retriver = new ElementTypeRetriver();
                node.Accept(retriver);
                return retriver.NodeType;
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(LeftRecursionNode node)
            {
                (node.First as MemberNode).Content.Accept(this);
            }

            public void Visit(LeftRecursionGroupNode node)
            {
                this.NodeType = ResultTypeRetriver.GetNodeType(node.FirstNode);
                foreach (var g in node.Groups)
                {
                    this.NodeType = g.Type;
                }
            }

            public void Visit(ListNode node)
            {
                NodeType = node.NodeType;
            }

            public void Visit(MemberNode node)
            {
            }

            public void Visit(ReturnNode node)
            {
            }

            public void Visit(RuleNode node)
            {
                NodeType = node.NodeType;
            }

            public void Visit(SequenceNode node)
            {
            }

            public void Visit(TokenNode node)
            {
                NodeType = typeof(CodeToken);
            }

            public void Visit(TokenContentNode node)
            {
                NodeType = typeof(CodeToken);
            }
        }

        class ResultTypeRetriver : TypeRetriverBase, IParserNodeVisitor
        {
            public static Type GetNodeType(ParserNode node)
            {
                ResultTypeRetriver retriver = new ResultTypeRetriver();
                node.Accept(retriver);
                return retriver.NodeType;
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(LeftRecursionNode node)
            {
                (node.First as MemberNode).Content.Accept(this);
            }

            public void Visit(LeftRecursionGroupNode node)
            {
                this.NodeType = ResultTypeRetriver.GetNodeType(node.FirstNode);
                foreach (var g in node.Groups)
                {
                    this.NodeType = g.Type;
                }
            }

            public void Visit(ListNode node)
            {
                NodeType = typeof(CodeNodeList<>).MakeGenericType(node.NodeType);
            }

            public void Visit(MemberNode node)
            {
            }

            public void Visit(ReturnNode node)
            {
            }

            public void Visit(RuleNode node)
            {
                NodeType = node.NodeType;
            }

            public void Visit(SequenceNode node)
            {
            }

            public void Visit(TokenNode node)
            {
                NodeType = typeof(CodeToken);
            }

            public void Visit(TokenContentNode node)
            {
                NodeType = typeof(CodeToken);
            }
        }

        class ReturnTypeRetriver : TypeRetriverBase, IParserNodeVisitor
        {
            public static Type GetNodeType(ParserNode node)
            {
                ReturnTypeRetriver retriver = new ReturnTypeRetriver();
                node.Accept(retriver);
                return retriver.NodeType;
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(LeftRecursionGroupNode node)
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
                this.NodeType = ResultTypeRetriver.GetNodeType(node.Content);
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

            public void Visit(LeftRecursionGroupNode node)
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

            public void Visit(LeftRecursionGroupNode node)
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

                if (level > 0)
                {
                    generator.sb.AppendLine(identation + "{");
                    generator.identation += "    ";
                }
                foreach (string member in MemberCollector.GetMembers(node))
                {
                    Type memberType = nodeType.GetProperty(member).PropertyType;
                    generator.sb.AppendLine(identation + GetTypeFullName(memberType) + " " + member + "Member" + level.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                }
                node.Accept(generator);
                if (previousLevel != level)
                {
                    foreach (string member in MemberCollector.GetMembers(node))
                    {
                        generator.sb.AppendLine(identation + member + "Member" + previousLevel.ToString() + " = " + member + "Member" + level.ToString() + ";");
                    }
                }
                if (level > 0)
                {
                    generator.sb.AppendLine(identation + "}");
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
                Type returnNodeType = ReturnTypeRetriver.GetNodeType(node);
                int newLevel = this.level + 1;
                string newReturnVariable = returnNodeType != null ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelSuccessName = "LABEL_SUCCESS_" + (this.labelCounter++).ToString();
                string labelFailName = "LABEL_FAIL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (returnNodeType != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = -1;");
                foreach (ParserNode branch in branches)
                {
                    bool branchHasReturnNode = ReturnTypeRetriver.GetNodeType(branch) != null;
                    sb.AppendLine(identation + "    " + newIndexVariable + " = " + this.indexVariable + ";");
                    sb.Append(CodeGenerator.GenerateCode(this.nodeType, branch, identation + "    ", newLevel, this.level, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, ref this.labelCounter));
                    sb.AppendLine(identation + "    if (parseSuccess)");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "        goto " + labelSuccessName + ";");
                    sb.AppendLine(identation + "    }");
                }
                sb.AppendLine(identation + "    goto " + labelFailName + ";");
                sb.AppendLine(identation + labelSuccessName + ":;");
                if (returnNodeType != null)
                {
                    GenerateAssignCode(null, returnNodeType, this.returnVariable, newReturnVariable);
                }
                sb.AppendLine(identation + labelFailName + ":;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(LeftRecursionNode node)
            {
                MemberNode first = (MemberNode)node.First;
                ParserNode next = node.Next;
                int newLevel = this.level + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");

                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(ResultTypeRetriver.GetNodeType(first.Content), first.Content, identation + "    ", newLevel, this.level, this.returnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (parseSuccess)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "    else");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        goto " + labelName + ";");
                sb.AppendLine(identation + "    }");

                sb.AppendLine(identation + "    while (true)");
                sb.AppendLine(identation + "    {");
                foreach (string member in MemberCollector.GetMembers(next))
                {
                    Type memberType = node.NodeType.GetProperty(member).PropertyType;
                    sb.AppendLine(identation + "        " + GetTypeFullName(memberType) + " " + member + "Member" + newLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                }
                sb.Append(CodeGenerator.GenerateCode(node.NodeType, next, identation + "        ", newLevel + 1, newLevel, "", newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "        if (parseSuccess)");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "            " + GetTypeFullName(node.NodeType) + " " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(node.NodeType) + ">();");
                foreach (string member in MemberCollector.GetMembers(next))
                {
                    sb.AppendLine(identation + "            " + newReturnVariable + "." + member + " = " + member + "Member" + newLevel.ToString() + ";");
                }
                sb.AppendLine(identation + "            " + newReturnVariable + "." + first.Member + " = " + this.returnVariable + ";");
                sb.AppendLine(identation + "            " + newReturnVariable + ".Start = " + this.returnVariable + ".Start;");
                sb.AppendLine(identation + "            " + newReturnVariable + ".End = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetEndPosition(tokens, " + this.indexVariable + ");");
                sb.AppendLine(identation + "            " + this.returnVariable + " = " + newReturnVariable + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        else");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            break;");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "    }");

                sb.AppendLine(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + labelName + ":;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(LeftRecursionGroupNode node)
            {
                int newLevel = this.level + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");

                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(ResultTypeRetriver.GetNodeType(node.FirstNode), node.FirstNode, identation + "    ", newLevel, this.level, this.returnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (parseSuccess)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "    else");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        goto " + labelName + ";");
                sb.AppendLine(identation + "    }");

                sb.AppendLine(identation + "    while (true)");
                sb.AppendLine(identation + "    {");
                foreach (var g in node.Groups)
                {
                    sb.AppendLine(identation + "        {");
                    foreach (string member in MemberCollector.GetMembers(g.NextNode))
                    {
                        Type memberType = g.Type.GetProperty(member).PropertyType;
                        sb.AppendLine(identation + "            " + GetTypeFullName(memberType) + " " + member + "Member" + newLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                    }
                    sb.Append(CodeGenerator.GenerateCode(g.Type, g.NextNode, identation + "        ", newLevel + 1, newLevel, "", newIndexVariable, ref this.labelCounter));
                    sb.AppendLine(identation + "            if (parseSuccess)");
                    sb.AppendLine(identation + "            {");
                    sb.AppendLine(identation + "                " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "                " + GetTypeFullName(g.Type) + " " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(g.Type) + ">();");
                    foreach (string member in MemberCollector.GetMembers(g.NextNode))
                    {
                        sb.AppendLine(identation + "                " + newReturnVariable + "." + member + " = " + member + "Member" + newLevel.ToString() + ";");
                    }
                    sb.AppendLine(identation + "                " + newReturnVariable + "." + g.FirstMember + " = " + this.returnVariable + ";");
                    sb.AppendLine(identation + "                " + newReturnVariable + ".Start = " + this.returnVariable + ".Start;");
                    sb.AppendLine(identation + "                " + newReturnVariable + ".End = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetEndPosition(tokens, " + this.indexVariable + ");");
                    sb.AppendLine(identation + "                " + this.returnVariable + " = " + newReturnVariable + ";");
                    sb.AppendLine(identation + "                continue;");
                    sb.AppendLine(identation + "            }");
                    sb.AppendLine(identation + "        }");
                }
                sb.AppendLine(identation + "        break;");
                sb.AppendLine(identation + "    }");

                sb.AppendLine(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + labelName + ":;");
                sb.AppendLine(identation + "}");
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
                sb.AppendLine(identation + "    if (parseSuccess)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "        " + this.returnVariable + ".Add(" + newReturnVariable + ");");
                    sb.AppendLine(identation + "        " + this.returnVariable + ".Start = " + newReturnVariable + ".Start;");
                    sb.AppendLine(identation + "        " + this.returnVariable + ".End = " + newReturnVariable + ".End;");
                }
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "    while (true)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        int " + copiedIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(ResultTypeRetriver.GetNodeType(node.Separator), node.Separator, identation + "        ", newLevel, this.level, "", newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "        if (parseSuccess)");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + copiedIndexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        else");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            goto " + labelName + ";");
                sb.AppendLine(identation + "        }");
                sb.Append(CodeGenerator.GenerateCode(node.NodeType, node.Item, identation + "        ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "        if (parseSuccess)");
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
                    sb.AppendLine(identation + "        " + this.returnVariable + ".End = " + newReturnVariable + ".End;");
                }
                sb.AppendLine(identation + "    }");
                sb.Append(identation + labelName + ":;");
                sb.Append(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(MemberNode node)
            {
                Type memberElementNodeType = ElementTypeRetriver.GetNodeType(node.Content);
                Type memberResultNodeType = ResultTypeRetriver.GetNodeType(node.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = "result" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                sb.AppendLine(identation + "{");
                sb.AppendLine(identation + "    " + GetTypeFullName(memberResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberResultNodeType) + ");");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(memberElementNodeType, node.Content, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (parseSuccess)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                GenerateAssignCode(nodeType.GetProperty(node.Member).PropertyType, memberResultNodeType, node.Member + "Member" + this.level.ToString(), newReturnVariable);
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "}");
            }

            public void Visit(ReturnNode node)
            {
                Type memberElementNodeType = ElementTypeRetriver.GetNodeType(node.Content);
                Type memberResultNodeType = ResultTypeRetriver.GetNodeType(node.Content);
                int newLevel = this.level + 1;
                string newReturnVariable = "result" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                sb.AppendLine(identation + "{");
                sb.AppendLine(identation + "    " + GetTypeFullName(memberResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberResultNodeType) + ");");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.Append(CodeGenerator.GenerateCode(memberElementNodeType, node.Content, identation + "    ", newLevel, this.level, newReturnVariable, newIndexVariable, ref this.labelCounter));
                sb.AppendLine(identation + "    if (parseSuccess)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                GenerateAssignCode(null, memberResultNodeType, this.returnVariable, newReturnVariable);
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "}");
            }

            public void Visit(RuleNode node)
            {
                sb.AppendLine(identation + "parseSuccess = false;");
                if (this.returnVariable == "")
                {
                    sb.AppendLine(identation + "Parse" + node.RuleName + "(tokens, ref " + this.indexVariable + ", ref parseSuccess);");
                }
                else
                {
                    sb.AppendLine(identation + this.returnVariable + " = Parse" + node.RuleName + "(tokens, ref " + this.indexVariable + ", ref parseSuccess);");
                }
            }

            public void Visit(SequenceNode node)
            {
                List<ParserNode> branches = SequenceNodeFlatter.Flat(node);
                Type returnNodeType = ReturnTypeRetriver.GetNodeType(node);
                int newLevel = this.level + 1;
                string newReturnVariable = returnNodeType != null ? "result" + newLevel.ToString() : "";
                string copiedIndexVariable = "currentIndexCopy" + newLevel.ToString();
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (returnNodeType != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + copiedIndexVariable + " = " + this.indexVariable + ";");
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                foreach (ParserNode branch in branches)
                {
                    bool branchHasReturnNode = ReturnTypeRetriver.GetNodeType(branch) != null;
                    sb.Append(CodeGenerator.GenerateCode(this.nodeType, branch, identation + "    ", newLevel, this.level, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, ref this.labelCounter));
                    sb.AppendLine(identation + "    if (parseSuccess)");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + copiedIndexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "    }");
                    sb.AppendLine(identation + "    else");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        goto " + labelName + ";");
                    sb.AppendLine(identation + "    }");
                }
                sb.AppendLine(identation + "    " + this.indexVariable + " = " + copiedIndexVariable + ";");
                if (returnNodeType != null)
                {
                    GenerateAssignCode(null, returnNodeType, this.returnVariable, newReturnVariable);
                }
                sb.AppendLine(identation + labelName + ":;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(TokenNode node)
            {
                sb.AppendLine(identation + "parseSuccess = false;");
                sb.AppendLine(identation + "if (" + this.indexVariable + " < tokens.Count && tokens[" + this.indexVariable + "].Id == " + node.Code + ")");
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + this.returnVariable + " = tokens[" + this.indexVariable + "];");
                }
                sb.AppendLine(identation + "    " + this.indexVariable + "++;");
                sb.AppendLine(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(TokenContentNode node)
            {
                string condition = node.TokenValues
                    .Select(v => "tokens[" + this.indexVariable + "].Value == \"" + v + "\"")
                    .Aggregate((a, b) => a + " || " + b);
                sb.AppendLine(identation + "parseSuccess = false;");
                sb.AppendLine(identation + "if (" + this.indexVariable + " < tokens.Count && (" + condition + "))");
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + this.returnVariable + " = tokens[" + this.indexVariable + "];");
                }
                sb.AppendLine(identation + "    " + this.indexVariable + "++;");
                sb.AppendLine(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + "}");
            }
        }
    }
}
