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
    }

    public abstract class AddExpression : BinaryExpression
    {
    }

    public abstract class SubExpression : BinaryExpression
    {
    }

    public abstract class MulExpression : BinaryExpression
    {
    }

    public abstract class DivExpression : BinaryExpression
    {
    }

    public abstract class FunctionExpression : Expression
    {
        public string Name { get; set; }
        public abstract CodeNodeList<Expression> Parameters { get; set; }
    }
}
