using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.PrimitiveExpressions;

namespace FvCalculation.OperatorExpressions
{
    class DivExpression : Expression
    {
        public Expression Left { get; set; }
        public Expression Right { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            return this.Left.Execute(variables) / this.Right.Execute(variables);
        }

        public override Expression Different(string variable)
        {
            return new DivExpression
            {
                Left = new SubExpression
                {
                    Left = new MulExpression
                    {
                        Left = this.Left.Different(variable),
                        Right = this.Right,
                    },
                    Right = new MulExpression
                    {
                        Left = this.Left,
                        Right = this.Right.Different(variable),
                    },
                },
                Right = new PowerExpression
                {
                    Left = this.Right,
                    Right = new NumberExpression
                    {
                        Number = 2,
                    },
                },
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
            if (nleft != null)
            {
                if (nleft.Number == 0)
                {
                    return new NumberExpression
                    {
                        Number = 0,
                    };
                }
            }
            else if (nright != null)
            {
                if (nright.Number == 1)
                {
                    return sleft;
                }
            }
            return new DivExpression
            {
                Left = sleft,
                Right = sright,
            };
        }

        public override string ToCode()
        {
            return "(" + this.Left.ToCode() + " / " + this.Right.ToCode() + ")";
        }
    }
}
