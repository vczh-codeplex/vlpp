using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FvCalculation
{
    public static class ExpressionExtensions
    {
        public static double Execute(this Expression e, string name, double value)
        {
            return e.Execute(new Dictionary<string, double> { { name, value } });
        }

        public static Expression Apply(this Expression e, string name, double value)
        {
            return e.Apply(new Dictionary<string, double> { { name, value } });
        }

        public static double Solve(this Expression e, string name, double start, int maxCount = 1000)
        {
            Expression f = e.Simplify();
            Expression df = f.Different(name).Simplify();
            return f.Solve(df, name, ref start, maxCount);
        }

        private static double Solve(this Expression f, Expression df, string name, ref double start, int maxCount = 1000)
        {
            for (int i = 0; i < maxCount; i++)
            {
                double result = f.Execute(name, start);
                if (-Expression.ZeroNumber <= result && result <= Expression.ZeroNumber)
                {
                    return start;
                }

                double d = df.Execute(name, start);
                if (-Expression.ZeroNumber <= d && d <= Expression.ZeroNumber)
                {
                    return double.NaN;
                }
                else
                {
                    start -= result / d;
                }
            }
            return double.NaN;
        }
    }
}
