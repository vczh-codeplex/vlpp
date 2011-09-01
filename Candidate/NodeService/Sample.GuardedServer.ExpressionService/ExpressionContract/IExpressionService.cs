using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using FvCalculation;

namespace ExpressionContract
{
    public interface IExpressionService
    {
        RawExpression Parse(string input);
        RawExpression Simplify(RawExpression expression);
        RawExpression Different(RawExpression expression, string variable);
        RawExpression Apply(RawExpression expression, string variable, double value);
        double Evaluate(RawExpression expression);
        double Solve(RawExpression expression, string variable, double start);
    }

    public interface IExpressionServiceClient : IExpressionService, INodeEndpointClient
    {
    }
}
