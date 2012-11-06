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
                string e = "(k1 *Cs1 + k2 * Co2) / (k1 * C1 + k2 * C2)";
                {
                    RawExpression exp = RawExpression.Parse(e);
                    RawExpression dk1 = exp.Different("k1").Simplify();
                    RawExpression dk2 = exp.Different("k2").Simplify();

                    Console.WriteLine("input:\t\t" + e);
                    Console.WriteLine("parse:\t\t" + exp.ToCode());
                    Console.WriteLine("dk1:\t\t" + dk1.ToCode());
                    Console.WriteLine("dk2:\t\t" + dk2.ToCode());
                }

                // cs1 = 128, Co2 = 1024, C1 = 256, C2 = 2048
                Dictionary<string, double> values = new Dictionary<string, double>
                {
                    {"Cs1", 128},
                    {"Co2", 1024},
                    {"C1", 256},
                    {"C2", 2048},
                };
                foreach (var p in values)
                {
                    Console.WriteLine("{0} => {1}", p.Value, p.Key);
                }
                {
                    RawExpression exp = RawExpression.Parse(e).Apply(values).Simplify();
                    RawExpression dk1 = exp.Different("k1").Simplify();
                    RawExpression dk2 = exp.Different("k2").Simplify();
                    Console.WriteLine("applied:\t" + exp.ToCode());
                    Console.WriteLine("dk1:\t\t" + dk1.ToCode());
                    Console.WriteLine("dk2:\t\t" + dk2.ToCode());
                }
            }
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
                    "exp(x)/ln(x)",
                    "sin(x)",
                    "cos(x)",
                    "tan(x)",
                    "cot(x)",
                    "sec(x)",
                    "csc(x)",
                };
                foreach (var e in expressions)
                {
                    RawExpression exp = RawExpression.Parse(e);
                    Console.WriteLine("input:\t\t" + e);
                    Console.WriteLine("parse:\t\t" + exp.ToCode());
                    Console.WriteLine("simplified:\t" + exp.Simplify().ToCode());
                    Console.WriteLine("dx:\t\t" + exp.Different("x").Simplify().ToCode());
                    Console.WriteLine("dy:\t\t" + exp.Different("y").Simplify().ToCode());
                    Console.WriteLine("no x:\t\t" + exp.Apply("x", 0).Simplify().ToCode());
                    Console.WriteLine("no y:\t\t" + exp.Apply("y", 0).Simplify().ToCode());
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
