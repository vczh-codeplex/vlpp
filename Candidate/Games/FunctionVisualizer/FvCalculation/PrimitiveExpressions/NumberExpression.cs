using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FvCalculation.PrimitiveExpressions
{
    class NumberExpression : Expression
    {
        public double Number { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            return this.Number;
        }

        public override Expression Apply(Dictionary<string, double> variables)
        {
            return this;
        }

        public override Expression Different(string variable)
        {
            return new NumberExpression
            {
                Number = 0,
            };
        }

        public override bool ContainsVariable(string variable)
        {
            return false;
        }

        public override Expression SimplifyInternal()
        {
            return this;
        }

        public override string ToCode()
        {
            return this.Number.ToString();
        }
    }
}
