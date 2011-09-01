using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ExpressionContract;
using NodeService;
using FvCalculation;

namespace ExpressionClient
{
    class Program
    {
        static void Main(string[] args)
        {
            IExpressionServiceClient client = ExpressionServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<IExpressionServiceClient>(
                    "localhost/" + ExpressionServiceConfiguration.ProtocolName,
                    ExpressionServiceConfiguration.EndpointName
                    );

            string input = "x^2-(2+3)*x+2*3";
            Console.WriteLine("Input = " + input);

            RawExpression expression = client.Parse(input);
            Console.WriteLine("Parse() = " + client.ToCode(expression));

            RawExpression simplified = client.Simplify(expression);
            Console.WriteLine("Simplify() = " + client.ToCode(simplified));

            RawExpression differentiated = client.Simplify(client.Different(simplified, "x"));
            Console.WriteLine("Different(x) = " + client.ToCode(differentiated));

            double evaluated1 = client.Evaluate(client.Apply(expression, "x", 1));
            Console.WriteLine("Evaluate(x=1) = " + evaluated1.ToString());

            double evaluated2 = client.EvaluateWithArguments(expression, new Dictionary<string, double>() { { "x", 2 } });
            Console.WriteLine("Evaluate(x=2) = " + evaluated2.ToString());

            double solved = client.Solve(expression, "x", 0);
            Console.WriteLine("Solve(x) = " + solved.ToString());

            Console.ReadLine();
        }
    }
}
