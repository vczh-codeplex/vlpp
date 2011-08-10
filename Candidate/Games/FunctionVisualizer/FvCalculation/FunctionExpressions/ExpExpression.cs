using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.OperatorExpressions;

namespace FvCalculation.FunctionExpressions
{
    [FunctionName("exp")]
    class ExpExpression : FunctionExpression
    {
        public override double Execute(Dictionary<string, double> variables)
        {
            return Math.Exp(this.Op.Execute(variables));
        }

        public override Expression Different(string variable)
        {
            return new MulExpression
            {
                Left = this,
                Right = this.Op.Different(variable),
            };
        }

        public override bool ContainsVariable(string variable)
        {
            return this.Op.ContainsVariable(variable);
        }
    }
}
