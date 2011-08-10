using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.PrimitiveExpressions;

namespace FvCalculation.OperatorExpressions
{
    class SubExpression : Expression
    {
        public Expression Left { get; set; }
        public Expression Right { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            return this.Left.Execute(variables) - this.Right.Execute(variables);
        }

        public override Expression Apply(Dictionary<string, double> variables)
        {
            return new SubExpression
            {
                Left = this.Left.Apply(variables),
                Right = this.Right.Apply(variables),
            };
        }

        public override Expression Different(string variable)
        {
            return new SubExpression
            {
                Left = this.Left.Different(variable),
                Right = this.Right.Different(variable),
            };
        }

        public override bool ContainsVariable(string variable)
        {
            return this.Left.ContainsVariable(variable) || this.Right.ContainsVariable(variable);
        }

        public override Expression SimplifyInternal()
        {
            Expression sleft = this.Left.Simplify();
            Expression sright = this.Right.Simplify();
            NumberExpression nleft = sleft as NumberExpression;
            NumberExpression nright = sright as NumberExpression;
            if (nleft != null && nleft.Number == 0)
            {
                return new NegExpression
                {
                    Op = sright,
                };
            }
            else if (nright != null && nright.Number == 0)
            {
                return sleft;
            }
            else
            {
                return new SubExpression
                {
                    Left = sleft,
                    Right = sright,
                };
            }
        }

        public override string ToCode()
        {
            return "(" + this.Left.ToCode() + " - " + this.Right.ToCode() + ")";
        }
    }
}
