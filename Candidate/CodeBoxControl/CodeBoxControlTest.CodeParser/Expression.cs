using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest.CodeParser
{
    public abstract class Expression : CodeNode
    {
    }

    public abstract class NumberExpression : Expression
    {
        public double Number { get; set; }
    }

    public abstract class BinaryExpression : Expression
    {
        public abstract Expression Left { get; set; }
        public abstract Expression Right { get; set; }
        public string Operator { get; set; }
    }

    public abstract class AddExpression : BinaryExpression
    {
        public AddExpression()
        {
            this.Operator = "+";
        }
    }

    public abstract class SubExpression : BinaryExpression
    {
        public SubExpression()
        {
            this.Operator = "-";
        }
    }

    public abstract class MulExpression : BinaryExpression
    {
        public MulExpression()
        {
            this.Operator = "*";
        }
    }

    public abstract class DivExpression : BinaryExpression
    {
        public DivExpression()
        {
            this.Operator = "/";
        }
    }

    public abstract class FunctionExpression : Expression
    {
        public string Name { get; set; }
        public abstract CodeNodeList<Expression> Parameters { get; set; }
    }
}
