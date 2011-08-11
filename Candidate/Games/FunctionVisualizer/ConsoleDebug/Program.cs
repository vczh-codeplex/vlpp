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
                    RawExpression exp = RawExpression.Parse(e);
                    Console.WriteLine("input:\t\t" + e);
                    Console.WriteLine("parse:\t\t" + exp.ToCode());
                    Console.WriteLine("reparse:\t" + RawExpression.Parse(exp.ToCode()).ToCode());
                    Console.WriteLine("simplified:\t" + exp.Simplify().ToCode());
                    Console.WriteLine("contains x:\t" + exp.ContainsVariable("x").ToString());
                    Console.WriteLine("contains y:\t" + exp.ContainsVariable("y").ToString());
                    Console.WriteLine("dx:\t\t" + exp.Different("x").ToCode());
                    Console.WriteLine("dy:\t\t" + exp.Different("y").ToCode());
                    Console.WriteLine("no x:\t\t" + exp.Apply("x", 0).ToCode());
                    Console.WriteLine("no y:\t\t" + exp.Apply("y", 0).ToCode());
                    Console.WriteLine("simplified dx:\t" + exp.Different("x").Simplify().ToCode());
                    Console.WriteLine("simplified dy:\t" + exp.Different("y").Simplify().ToCode());
                    Console.WriteLine("simplified no x:" + exp.Apply("x", 0).Simplify().ToCode());
                    Console.WriteLine("simplified no y:" + exp.Apply("y", 0).Simplify().ToCode());
                    Console.WriteLine();

                    Debug.Assert(exp.ToCode() == RawExpression.Parse(exp.ToCode()).ToCode());
                }
            }

            {
                string[] expressions =
                {
                    "(x-3)*(x-5)",
                    "ln(x)/ln(2)-3",
                    "(x-5)^2-4",
                };
                foreach (var e in expressions)
                {
                    RawExpression exp = RawExpression.Parse(e);
                    Console.WriteLine("input:\t\t" + e);
                    Console.WriteLine("parse:\t\t" + exp.ToCode());
                    for (int i = 0; i < 10; i++)
                    {
                        Console.WriteLine("solve:\t\t" + i.ToString() + " => " + exp.Solve("x", i).ToString());
                    }
                    Console.WriteLine();
                }
            }
        }
    }
}
