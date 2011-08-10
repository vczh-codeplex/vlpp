using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.PrimitiveExpressions;

namespace FvCalculation.OperatorExpressions
{
    class NegExpression : Expression
    {
        public Expression Op { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            return -this.Op.Execute(variables);
        }

        public override Expression Different(string variable)
        {
            return new NegExpression
            {
                Op = this.Op.Different(variable),
            };
        }

        public override bool ContainsVariable(string variable)
        {
            return this.Op.ContainsVariable(variable);
        }

        public override Expression SimplifyInternal()
        {
            return new NegExpression
            {
                Op = this.Op.Simplify(),
            };
        }

        public override string ToCode()
        {
            return "-" + this.Op.ToCode();
        }
    }
}
