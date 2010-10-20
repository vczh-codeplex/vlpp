using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider.ParserCodeGenerator
{
    public abstract class ParserNode
    {
        public static ParserNode operator |(ParserNode left, ParserNode right)
        {
            return new ChoiceNode()
            {
                Left = left,
                Right = right
            };
        }

        public static ParserNode operator +(ParserNode left, ParserNode right)
        {
            return new SequenceNode()
            {
                Left = left,
                Right = right
            };
        }

        public ParserNode this[string member]
        {
            get
            {
                if (this is RuleNode || this is TokenNode || this is TokenContentNode || this is LeftRecursionNode || this is LeftRecursionGroupNode || this is ListNode)
                {
                    return new MemberNode()
                    {
                        Content = this,
                        Member = member
                    };
                }
                else
                {
                    throw new ArgumentException("ParserNode.operator[] can only be applied to rule, token, list or leftrec.");
                }
            }
        }

        public abstract void Accept(IParserNodeVisitor visitor);
    }

    public interface IParserNodeVisitor
    {
        void Visit(ChoiceNode node);
        void Visit(LeftRecursionNode node);
        void Visit(LeftRecursionGroupNode leftRecursionGroupNode);
        void Visit(ListNode node);
        void Visit(MemberNode node);
        void Visit(ReturnNode node);
        void Visit(RuleNode node);
        void Visit(SequenceNode node);
        void Visit(TokenNode node);
        void Visit(TokenContentNode node);
    }

    public class Parser
    {
        public static RuleNode rule<T>(string name)
        {
            return new RuleNode()
            {
                NodeType = typeof(T),
                RuleName = name
            };
        }

        public static ParserNode id(string code)
        {
            return new TokenNode()
            {
                Code = code
            };
        }

        public static ParserNode tok(string id)
        {
            return new TokenContentNode()
            {
                TokenValues = new string[] { id }
            };
        }

        public static ParserNode toks(params string[] ids)
        {
            return new TokenContentNode()
            {
                TokenValues = ids
            };
        }

        public static ParserNode ret(ParserNode node)
        {
            if (node is RuleNode || node is TokenNode || node is TokenContentNode || node is LeftRecursionNode || node is LeftRecursionGroupNode || node is ListNode)
            {
                return new ReturnNode()
                {
                    Content = node
                };
            }
            else
            {
                throw new ArgumentException("Parser.ret can only be applied to rule, token, list or leftrec.");
            }
        }

        public static ParserNode list<T>(ParserNode item)
        {
            return new ListNode()
            {
                NodeType = typeof(T),
                Item = item
            };
        }

        public static ParserNode list<T>(ParserNode separator, ParserNode item)
        {
            return new ListNode()
            {
                NodeType = typeof(T),
                Separator = separator,
                Item = item
            };
        }

        public static ParserNode leftrec<T>(ParserNode first, ParserNode next)
        {
            if (first is MemberNode)
            {
                return new LeftRecursionNode()
                {
                    First = first,
                    Next = next,
                    NodeType = typeof(T)
                };
            }
            else
            {
                throw new ArgumentException("The first argument for Parser.leftrec should be MemberNode.");
            }
        }

        public static LeftRecursionGroupNode.Group g<T>(string member, ParserNode node)
            where T : CodeNode
        {
            return new LeftRecursionGroupNode.Group()
            {
                FirstMember = member,
                NextNode = node,
                Type = typeof(T)
            };
        }

        public static ParserNode leftrecg(ParserNode first, params LeftRecursionGroupNode.Group[] nexts)
        {
            return new LeftRecursionGroupNode()
            {
                FirstNode = first,
                Groups = nexts.ToList()
            };
        }
    }
}
