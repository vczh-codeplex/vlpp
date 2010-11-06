using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public static class ParserGenerator
    {
        public static string GenerateCSharpCode(string namespaceName, string className, params RuleNode[] startRules)
        {
            StringBuilder sb = new StringBuilder();
            List<RuleNode> rules = startRules.SelectMany(r => RuleNodeCollector.GetRules(r)).Distinct().ToList();

            sb.AppendLine("using System;");
            sb.AppendLine("using System.Collections.Generic;");
            sb.AppendLine("using System.Linq;");
            sb.AppendLine("using System.Text;");
            sb.AppendLine("using Developer.LanguageProvider;");
            sb.AppendLine();
            sb.AppendLine("namespace " + namespaceName);
            sb.AppendLine("{");
            sb.AppendLine("    public static partial class " + className);
            sb.AppendLine("    {");
            foreach (RuleNode rule in rules)
            {
                int labelCounter = 0;
                bool noReturn = MemberCollector.GetMembers(rule.Content).Count == 0 && ReturnTypeRetriver.GetNodeType(rule.Content) == null;
                sb.AppendLine("        public static " + rule.NodeType.FullName + " Parse" + rule.RuleName + "(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)");
                sb.AppendLine("        {");
                sb.AppendLine("            bool forceSuccess0 = false;");
                sb.AppendLine("            " + GetTypeFullName(typeof(TextPosition)) + " start = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetStartPosition(tokens, currentToken);");
                sb.AppendLine("            " + GetTypeFullName(rule.NodeType) + " result = default(" + GetTypeFullName(rule.NodeType) + ");");
                foreach (string member in MemberCollector.GetMembers(rule.Content))
                {
                    Type memberType = rule.NodeType.GetProperty(member).PropertyType;
                    sb.AppendLine("            " + GetTypeFullName(memberType) + " " + member + "Member0 = default(" + GetTypeFullName(memberType) + ");");
                }
                CodeGenerator.GenerateCode(sb, rule.NodeType, rule.Content, "            ", 0, 0, (noReturn ? "" : "result"), "currentToken", 0, ref labelCounter);
                sb.AppendLine("            if (parseSuccess || forceSuccess0)");
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
                sb.AppendLine("                parseSuccess = true;");
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

        #region Helper Visitors

        class RuleNodeCollector : IParserNodeVisitor
        {
            public List<RuleNode> rules = new List<RuleNode>();

            public static List<RuleNode> GetRules(RuleNode startRule)
            {
                RuleNodeCollector collector = new RuleNodeCollector();
                startRule.Accept(collector);
                return collector.rules;
            }

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(OptionalNode node)
            {
                node.Content.Accept(this);
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
                if (node.Separator != null)
                {
                    node.Separator.Accept(this);
                }
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

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(OptionalNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(LeftRecursionNode node)
            {
            }

            public void Visit(LeftRecursionGroupNode node)
            {
            }

            public void Visit(ListNode node)
            {
                if (node.Separator != null)
                {
                    node.Separator.Accept(this);
                }
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

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(OptionalNode node)
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

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ChoiceNode node)
            {
            }

            public void Visit(OptionalNode node)
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

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(OptionalNode node)
            {
                node.Content.Accept(this);
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

            public void Visit(KeyNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ChoiceNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(OptionalNode node)
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

            public void Visit(KeyNode node)
            {
                this.nodes.Add(node);
            }

            public void Visit(ChoiceNode node)
            {
                node.Left.Accept(this);
                node.Right.Accept(this);
            }

            public void Visit(OptionalNode node)
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

        #endregion

        class CodeGenerator : IParserNodeVisitor
        {
            private string identation = "";
            private int level = 0;
            private int memberLevel = 0;
            private string returnVariable = "";
            private string indexVariable = "";
            private int forceSuccessLevel = 0;
            private Type nodeType = null;
            private int labelCounter = 0;
            private StringBuilder sb = null;

            public static void GenerateCode(StringBuilder sb, Type nodeType, ParserNode node, string identation, int level, int memberLevel, string returnVariable, string indexVariable, int forceSuccessLevel, ref int labelCounter)
            {
                CodeGenerator generator = new CodeGenerator();
                generator.identation = identation;
                generator.level = level;
                generator.memberLevel = memberLevel;
                generator.returnVariable = returnVariable;
                generator.indexVariable = indexVariable;
                generator.forceSuccessLevel = forceSuccessLevel;
                generator.nodeType = nodeType;
                generator.labelCounter = labelCounter;
                generator.sb = sb;

                node.Accept(generator);
                labelCounter = generator.labelCounter;
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

            public void Visit(KeyNode node)
            {
                node.Content.Accept(this);
                sb.AppendLine(identation + "if (parseSuccess) forceSuccess" + this.forceSuccessLevel.ToString() + " = true;");
            }

            public void Visit(ChoiceNode node)
            {
                List<ParserNode> branches = ChoiceNodeFlatter.Flat(node);
                Type returnNodeType = ReturnTypeRetriver.GetNodeType(node);
                int newLevel = this.level + 1;
                int newForceSuccessLevel = this.forceSuccessLevel + 1;
                string newReturnVariable = returnNodeType != null ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string newForceSuccessVariable = "forceSuccess" + newForceSuccessLevel.ToString();
                string labelSuccessName = "LABEL_SUCCESS_" + (this.labelCounter++).ToString();
                string labelFailName = "LABEL_FAIL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (returnNodeType != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = -1;");
                sb.AppendLine(identation + "    bool " + newForceSuccessVariable + " = false;");
                foreach (ParserNode branch in branches)
                {
                    bool branchHasReturnNode = ReturnTypeRetriver.GetNodeType(branch) != null;
                    sb.AppendLine(identation + "    " + newIndexVariable + " = " + this.indexVariable + ";");
                    CodeGenerator.GenerateCode(this.sb, this.nodeType, branch, identation + "    ", newLevel, this.memberLevel, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "    if (parseSuccess || " + newForceSuccessVariable + ")");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "        forceSuccess" + this.forceSuccessLevel.ToString() + " = " + newForceSuccessVariable + ";");
                    sb.AppendLine(identation + "        goto " + labelSuccessName + ";");
                    sb.AppendLine(identation + "    }");

                    List<string> members = MemberCollector.GetMembers(branch);
                    if (members.Count > 0)
                    {
                        sb.AppendLine(identation + "    else");
                        sb.AppendLine(identation + "    {");
                        foreach (string member in members)
                        {
                            Type memberType = this.nodeType.GetProperty(member).PropertyType;
                            sb.AppendLine(identation + "        " + member + "Member" + this.memberLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                        }
                        sb.AppendLine(identation + "        " + newForceSuccessVariable + " = false;");
                        sb.AppendLine(identation + "    }");
                    }
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

            public void Visit(OptionalNode node)
            {
                Type returnNodeType = ReturnTypeRetriver.GetNodeType(node);
                int newLevel = this.level + 1;
                int newForceSuccessLevel = this.forceSuccessLevel + 1;
                string newReturnVariable = returnNodeType != null ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string newForceSuccessVariable = "forceSuccess" + newForceSuccessLevel.ToString();
                sb.AppendLine(identation + "{");
                if (returnNodeType != null)
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(returnNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(returnNodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = -1;");
                sb.AppendLine(identation + "    bool " + newForceSuccessVariable + " = false;");

                bool branchHasReturnNode = ReturnTypeRetriver.GetNodeType(node.Content) != null;
                sb.AppendLine(identation + "    " + newIndexVariable + " = " + this.indexVariable + ";");
                CodeGenerator.GenerateCode(this.sb, this.nodeType, node.Content, identation + "    ", newLevel, this.memberLevel, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "    if (parseSuccess || " + newForceSuccessVariable + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "    }");

                List<string> members = MemberCollector.GetMembers(node.Content);
                if (members.Count > 0)
                {
                    sb.AppendLine(identation + "    else");
                    sb.AppendLine(identation + "    {");
                    foreach (string member in members)
                    {
                        Type memberType = this.nodeType.GetProperty(member).PropertyType;
                        sb.AppendLine(identation + "        " + member + "Member" + this.memberLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                    }
                    sb.AppendLine(identation + "    }");
                }

                if (returnNodeType != null)
                {
                    GenerateAssignCode(null, returnNodeType, this.returnVariable, newReturnVariable);
                }
                sb.Append(identation + "    parseSuccess = true;");
                sb.AppendLine(identation + "}");
            }

            public void Visit(LeftRecursionNode node)
            {
                MemberNode first = (MemberNode)node.First;
                ParserNode next = node.Next;
                int newLevel = this.level + 1;
                int newMemberLevel = this.memberLevel + 1;
                int newForceSuccessLevel = this.forceSuccessLevel + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string newForceSuccessVariable = "forceSuccess" + newForceSuccessLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");

                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                CodeGenerator.GenerateCode(this.sb, ResultTypeRetriver.GetNodeType(first.Content), first.Content, identation + "    ", newLevel, this.memberLevel, this.returnVariable, newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "    }");
                sb.AppendLine(identation + "    else");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        goto " + labelName + ";");
                sb.AppendLine(identation + "    }");

                sb.AppendLine(identation + "    while (true)");
                sb.AppendLine(identation + "    {");
                sb.AppendLine(identation + "        bool " + newForceSuccessVariable + " = false;");
                foreach (string member in MemberCollector.GetMembers(next))
                {
                    Type memberType = node.NodeType.GetProperty(member).PropertyType;
                    sb.AppendLine(identation + "        " + GetTypeFullName(memberType) + " " + member + "Member" + newMemberLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                }
                CodeGenerator.GenerateCode(this.sb, node.NodeType, next, identation + "        ", newLevel + 1, newMemberLevel, "", newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "        if (parseSuccess || " + newForceSuccessVariable + ")");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + this.indexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "            " + GetTypeFullName(node.NodeType) + " " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(node.NodeType) + ">();");
                foreach (string member in MemberCollector.GetMembers(next))
                {
                    sb.AppendLine(identation + "            " + newReturnVariable + "." + member + " = " + member + "Member" + newMemberLevel.ToString() + ";");
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
                int newMemberLevel = this.memberLevel + 1;
                int newForceSuccessLevel = this.forceSuccessLevel + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string newForceSuccessVariable = "forceSuccess" + newForceSuccessLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");

                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                CodeGenerator.GenerateCode(this.sb, ResultTypeRetriver.GetNodeType(node.FirstNode), node.FirstNode, identation + "    ", newLevel, this.memberLevel, this.returnVariable, newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
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
                    sb.AppendLine(identation + "            bool " + newForceSuccessVariable + " = false;");
                    foreach (string member in MemberCollector.GetMembers(g.NextNode))
                    {
                        Type memberType = g.Type.GetProperty(member).PropertyType;
                        sb.AppendLine(identation + "            " + GetTypeFullName(memberType) + " " + member + "Member" + newMemberLevel.ToString() + " = default(" + GetTypeFullName(memberType) + ");");
                    }
                    CodeGenerator.GenerateCode(this.sb, g.Type, g.NextNode, identation + "        ", newLevel + 1, newMemberLevel, "", newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "            if (parseSuccess || " + newForceSuccessVariable + ")");
                    sb.AppendLine(identation + "            {");
                    sb.AppendLine(identation + "                " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "                " + GetTypeFullName(g.Type) + " " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(g.Type) + ">();");
                    foreach (string member in MemberCollector.GetMembers(g.NextNode))
                    {
                        sb.AppendLine(identation + "                " + newReturnVariable + "." + member + " = " + member + "Member" + newMemberLevel.ToString() + ";");
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
                int newForceSuccessLevel = this.forceSuccessLevel + 1;
                string newReturnVariable = this.returnVariable != "" ? "result" + newLevel.ToString() : "";
                string newIndexVariable = "currentIndex" + newLevel.ToString();
                string copiedIndexVariable = "currentIndexCopy" + newLevel.ToString();
                string newForceSuccessVariable = "forceSuccess" + newForceSuccessLevel.ToString();
                string labelName = "LABEL_" + (this.labelCounter++).ToString();
                sb.AppendLine(identation + "{");
                if (this.returnVariable != "")
                {
                    sb.AppendLine(identation + "    " + GetTypeFullName(node.NodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(node.NodeType) + ");");
                }
                sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                sb.AppendLine(identation + "    bool " + newForceSuccessVariable + " = false;");
                CodeGenerator.GenerateCode(this.sb, node.NodeType, node.Item, identation + "    ", newLevel, this.memberLevel, newReturnVariable, newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "    if (parseSuccess || " + newForceSuccessVariable + ")");
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
                sb.AppendLine(identation + "        " + newIndexVariable + " = " + this.indexVariable + ";");
                if (node.Separator != null)
                {
                    CodeGenerator.GenerateCode(this.sb, ResultTypeRetriver.GetNodeType(node.Separator), node.Separator, identation + "        ", newLevel, this.memberLevel, "", newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "        if (parseSuccess || " + newForceSuccessVariable + ")");
                    sb.AppendLine(identation + "        {");
                    sb.AppendLine(identation + "            " + copiedIndexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "        }");
                    sb.AppendLine(identation + "        else");
                    sb.AppendLine(identation + "        {");
                    sb.AppendLine(identation + "            goto " + labelName + ";");
                    sb.AppendLine(identation + "        }");
                }
                CodeGenerator.GenerateCode(this.sb, node.NodeType, node.Item, identation + "        ", newLevel, this.memberLevel, newReturnVariable, newIndexVariable, newForceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "        if (parseSuccess || " + newForceSuccessVariable + ")");
                sb.AppendLine(identation + "        {");
                sb.AppendLine(identation + "            " + copiedIndexVariable + " = " + newIndexVariable + ";");
                sb.AppendLine(identation + "        }");
                sb.AppendLine(identation + "        else");
                sb.AppendLine(identation + "        {");
                switch (node.ContinueType)
                {
                    case ListNodeContinueType.CreateNodeType:
                        {
                            if (this.returnVariable != "")
                            {
                                sb.AppendLine("            " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(node.NodeType) + ">();");
                                sb.AppendLine("            " + newReturnVariable + ".Start = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetStartPosition(tokens, " + newIndexVariable + ");");
                            }
                        }
                        break;
                    case ListNodeContinueType.KeepGoing:
                    case ListNodeContinueType.StopByFinishTokens:
                        {
                            bool continueParsing = true;
                            if (node.ContinueType == ListNodeContinueType.StopByFinishTokens)
                            {
                                if (node.FinishTokens.Length == 0)
                                {
                                    sb.AppendLine(identation + "            goto " + labelName + ";");
                                    continueParsing = false;
                                }
                                else
                                {
                                    string condition = node.FinishTokens.Select(v => ", \"" + v + "\"").Aggregate("", (a, b) => (a + b));
                                    string code = GetTypeFullName(typeof(CodeTokenizer)) + ".TestToken(tokens, " + this.indexVariable + condition + ")";
                                    sb.AppendLine(identation + "            if (" + code + ") goto " + labelName + ";");
                                }
                            }
                            if (continueParsing)
                            {
                                sb.AppendLine(identation + "            if (" + newIndexVariable + " < tokens.Count - 1)");
                                sb.AppendLine(identation + "            {");
                                sb.AppendLine(identation + "                " + copiedIndexVariable + " = " + newIndexVariable + " + 1;");
                                sb.AppendLine(identation + "            }");
                                sb.AppendLine(identation + "            else");
                                sb.AppendLine(identation + "            {");
                                sb.AppendLine(identation + "                goto " + labelName + ";");
                                sb.AppendLine(identation + "            }");
                                if (this.returnVariable != "")
                                {
                                    sb.AppendLine("            " + newReturnVariable + " = CodeNode.Create<" + GetTypeFullName(node.NodeType) + ">();");
                                    sb.AppendLine("            " + newReturnVariable + ".Start = " + GetTypeFullName(typeof(CodeTokenizer)) + ".GetStartPosition(tokens, " + newIndexVariable + ");");
                                }
                            }
                        }
                        break;
                }
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
                if (node.ValueCode != null)
                {
                    int newLevel = this.level + 1;
                    string newIndexVariable = "currentIndex" + newLevel.ToString();
                    sb.AppendLine(identation + "{");
                    sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                    CodeGenerator.GenerateCode(this.sb, null, node.Content, identation + "    ", newLevel, this.memberLevel, "", newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                    sb.AppendLine(identation + "        " + node.Member + "Member" + this.memberLevel.ToString() + " = " + node.ValueCode + ";");
                    sb.AppendLine(identation + "    }");
                    sb.AppendLine(identation + "}");
                }
                else
                {
                    Type memberElementNodeType = ElementTypeRetriver.GetNodeType(node.Content);
                    Type memberResultNodeType = ResultTypeRetriver.GetNodeType(node.Content);
                    int newLevel = this.level + 1;
                    string newReturnVariable = "result" + newLevel.ToString();
                    string newIndexVariable = "currentIndex" + newLevel.ToString();
                    sb.AppendLine(identation + "{");
                    sb.AppendLine(identation + "    " + GetTypeFullName(memberResultNodeType) + " " + newReturnVariable + " = default(" + GetTypeFullName(memberResultNodeType) + ");");
                    sb.AppendLine(identation + "    int " + newIndexVariable + " = " + this.indexVariable + ";");
                    CodeGenerator.GenerateCode(this.sb, memberElementNodeType, node.Content, identation + "    ", newLevel, this.memberLevel, newReturnVariable, newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
                    sb.AppendLine(identation + "    {");
                    sb.AppendLine(identation + "        " + this.indexVariable + " = " + newIndexVariable + ";");
                    GenerateAssignCode(nodeType.GetProperty(node.Member).PropertyType, memberResultNodeType, node.Member + "Member" + this.memberLevel.ToString(), newReturnVariable);
                    sb.AppendLine(identation + "    }");
                    sb.AppendLine(identation + "}");
                }
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
                CodeGenerator.GenerateCode(this.sb, memberElementNodeType, node.Content, identation + "    ", newLevel, this.memberLevel, newReturnVariable, newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
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
                    CodeGenerator.GenerateCode(this.sb, this.nodeType, branch, identation + "    ", newLevel, this.memberLevel, branchHasReturnNode ? newReturnVariable : "", newIndexVariable, this.forceSuccessLevel, ref this.labelCounter);
                    sb.AppendLine(identation + "    if (parseSuccess || forceSuccess" + this.forceSuccessLevel.ToString() + ")");
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
                string code = GetTypeFullName(typeof(CodeTokenizer)) + ".ParseToken(tokens, ref " + this.indexVariable + ", ref parseSuccess, " + node.Code + ");";
                if (this.returnVariable != "")
                {
                    sb.AppendLine(this.returnVariable + " = " + code);
                }
                else
                {
                    sb.AppendLine(identation + code);
                }
            }

            public void Visit(TokenContentNode node)
            {
                string condition = node.TokenValues.Select(v => ", \"" + v + "\"").Aggregate("", (a, b) => (a + b));
                string code = GetTypeFullName(typeof(CodeTokenizer)) + ".ParseToken(tokens, ref " + this.indexVariable + ", ref parseSuccess" + condition + ");";
                if (this.returnVariable != "")
                {
                    sb.AppendLine(this.returnVariable + " = " + code);
                }
                else
                {
                    sb.AppendLine(identation + code);
                }
            }
        }
    }
}
