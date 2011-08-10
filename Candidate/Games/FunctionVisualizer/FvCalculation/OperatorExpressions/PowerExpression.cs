using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.PrimitiveExpressions;
using FvCalculation.FunctionExpressions;

namespace FvCalculation.OperatorExpressions
{
    class PowerExpression : Expression
    {
        public Expression Left { get; set; }
        public Expression Right { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            double a = this.Left.Execute(variables);
            double b = this.Right.Execute(variables);
            return Math.Exp(b * Math.Log(a));
        }

        public override Expression Different(string variable)
        {
            bool lf = this.Left.ContainsVariable(variable);
            bool rf = this.Right.ContainsVariable(variable);
            if (lf)
            {
                if (rf)
                {
                    return new ExpExpression
                    {
                        Op = new MulExpression
                        {
                            Left = this.Right,
                            Right = new LnExpression
                            {
                                Op = this.Left,
                            },
                        },
                    }.Different(variable);
                }
                else
                {
                    return new MulExpression
                    {
                        Left = this.Right,
                        Right = new PowerExpression
                        {
                            Left = this.Left,
                            Right = new SubExpression
                            {
                                Left = this.Right,
                                Right = new NumberExpression
                                {
                                    Number = 1,
                                },
                            },
                        },
                    };
                }
            }
            else
            {
                if (rf)
                {
                    return new MulExpression
                    {
                        Left = this,
                        Right = new LnExpression
                        {
                            Op = this.Left,
                        },
                    };
                }
                else
                {
                    return new NumberExpression
                    {
                        Number = 0,
                    };
                }
            }
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
                else if (nleft.Number == 1)
                {
                    return new NumberExpression
                    {
                        Number = 1,
                    };
                }
            }
            else if (nright != null)
            {
                if (nright.Number == 0)
                {
                    return new NumberExpression
                    {
                        Number = 1,
                    };
                }
                else if (nright.Number == 1)
                {
                    return sleft;
                }
            }
            return new PowerExpression
            {
                Left = sleft,
                Right = sright,
            };
        }

        public override string ToCode()
        {
            return "(" + this.Left.ToCode() + " ^ " + this.Right.ToCode() + ")";
        }
    }
}
