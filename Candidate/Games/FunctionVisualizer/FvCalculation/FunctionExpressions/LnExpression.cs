using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.OperatorExpressions;

namespace FvCalculation.FunctionExpressions
{
    [FunctionName("ln")]
    class LnExpression : FunctionExpression
    {
        public override double Execute(Dictionary<string, double> variables)
        {
            return Math.Log(this.Op.Execute(variables));
        }

        public override Expression Different(string variable)
        {
            return new DivExpression
            {
                Left = this.Op.Different(variable),
                Right = this.Op,
            };
        }

        public override bool ContainsVariable(string variable)
        {
            return this.Op.ContainsVariable(variable);
        }
    }
}
