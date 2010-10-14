using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider.ParserCodeGenerator
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
                return new MemberNode()
                {
                    Content = this,
                    Member = member
                };
            }
        }

        public abstract void Accept(IParserNodeVisitor visitor);
    }

    public interface IParserNodeVisitor
    {
        void Visit(ChoiceNode node);
        void Visit(LeftRecursionNode node);
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

        public static ParserNode tok(int id)
        {
            return new TokenNode()
            {
                TokenId = id
            };
        }

        public static ParserNode tok(string id)
        {
            return new TokenContentNode()
            {
                TokenValue = id
            };
        }

        public static ParserNode ret(ParserNode node)
        {
            return new ReturnNode()
            {
                Content = node
            };
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
            return new LeftRecursionNode()
            {
                First = first,
                Next = next,
                NodeType = typeof(T)
            };
        }
    }
}
