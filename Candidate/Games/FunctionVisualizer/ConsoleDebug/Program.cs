using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation;
using System.Diagnostics;

namespace ConsoleDebug
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] expressions =
            {
                "1",
                "1.2",
                "-3",
                "12+34",
                "56*78",
                "x^y",
                "x+2*y+4",
                "(x+y)*(3+4)",
                "exp(x+y)",
                "x+ln(y)",
                "x/y",
                "exp(x)/ln(x)"
            };

            foreach (var e in expressions)
            {
                Expression exp = Expression.Parse(e);
                Console.WriteLine("input:\t\t" + e);
                Console.WriteLine("parse:\t\t" + exp.ToCode());
                Console.WriteLine("reparse:\t" + Expression.Parse(exp.ToCode()).ToCode());
                Console.WriteLine("simplified:\t" + exp.Simplify().ToCode());
                Console.WriteLine("contains x:\t" + exp.ContainsVariable("x").ToString());
                Console.WriteLine("contains y:\t" + exp.ContainsVariable("y").ToString());
                Console.WriteLine("dx:\t\t" + exp.Different("x").ToCode());
                Console.WriteLine("dy:\t\t" + exp.Different("y").ToCode());
                Console.WriteLine("simplified dx:\t" + exp.Different("x").Simplify().ToCode());
                Console.WriteLine("simplified dy:\t" + exp.Different("y").Simplify().ToCode());
                Console.WriteLine();

                Debug.Assert(exp.ToCode() == Expression.Parse(exp.ToCode()).ToCode());
            }
        }
    }
}
