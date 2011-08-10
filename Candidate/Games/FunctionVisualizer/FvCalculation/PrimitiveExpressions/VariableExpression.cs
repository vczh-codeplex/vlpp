using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FvCalculation.PrimitiveExpressions
{
    class VariableExpression : Expression
    {
        public string Name { get; set; }

        public override double Execute(Dictionary<string, double> variables)
        {
            return variables[this.Name];
        }

        public override Expression Different(string variable)
        {
            if (this.Name == variable)
            {
                return new NumberExpression
                {
                    Number = 1,
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

        public override bool ContainsVariable(string variable)
        {
            return this.Name == variable;
        }

        public override Expression SimplifyInternal()
        {
            return this;
        }

        public override string ToCode()
        {
            return this.Name;
        }
    }
}
