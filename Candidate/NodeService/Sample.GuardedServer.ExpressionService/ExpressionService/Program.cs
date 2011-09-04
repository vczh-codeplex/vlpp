using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeServiceHost;
using NodeService;
using ExpressionContract;
using NodeServiceHost.GuardService;
using FvCalculation;

namespace ExpressionService
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = ExpressionServiceConfiguration.EndpointName;
            GuardServiceStarter<ExpressionService, ExpressionServiceEndpointServerCallback>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                ExpressionServiceConfiguration.EndpointName
                );
        }
    }

    [NodeEndpoint(ExpressionServiceConfiguration.EndpointName)]
    public class ExpressionService : StrongTypedNodeEndpoint, IExpressionService
    {
        [NodeEndpointMethod]
        public RawExpression Parse(string input)
        {
            Console.WriteLine("received request: Parse({0})", input);
            return RawExpression.Parse(input);
        }

        [NodeEndpointMethod]
        public RawExpression Simplify(RawExpression expression)
        {
            Console.WriteLine("received request: Simplify({0})", expression);
            return expression.Simplify();
        }

        [NodeEndpointMethod]
        public RawExpression Different(RawExpression expression, string variable)
        {
            Console.WriteLine("received request: Different({0}, {1})", expression, variable);
            return expression.Different(variable);
        }

        [NodeEndpointMethod]
        public RawExpression Apply(RawExpression expression, string variable, double value)
        {
            Console.WriteLine("received request: Apply({0}, {1}, {2})", expression, variable, value);
            return expression.Apply(variable, value);
        }

        [NodeEndpointMethod]
        public double Evaluate(RawExpression expression)
        {
            Console.WriteLine("received request: Evaluate({0})", expression);
            return expression.Execute(new Dictionary<string, double>());
        }

        [NodeEndpointMethod]
        public double EvaluateWithArguments(RawExpression expression, Dictionary<string, double> arguments)
        {
            Console.WriteLine("received request: EvaluateWithArguments({0})", expression);
            return expression.Execute(arguments);
        }

        [NodeEndpointMethod]
        public double Solve(RawExpression expression, string variable, double start)
        {
            Console.WriteLine("received request: Solve({0}, {1}, {2})", expression, variable, start);
            return expression.Solve(variable, start);
        }

        [NodeEndpointMethod]
        public string ToCode(RawExpression expression)
        {
            return expression.ToCode();
        }
    }

    public class ExpressionServiceEndpointServerCallback : INodeEndpointServerCallback<ExpressionService>
    {
        private INodeEndpointProtocolFactory protocolFactory = ExpressionServiceConfiguration.CreateProtocolFactory();

        public INodeEndpointProtocolFactory ProtocolFactory
        {
            get
            {
                return this.protocolFactory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return ExpressionServiceConfiguration.ProtocolName;
            }
        }

        public string EndpointName
        {
            get
            {
                return ExpressionServiceConfiguration.EndpointName;
            }
        }

        public ExpressionService CreateEndpoint()
        {
            return new ExpressionService();
        }

        public void OnEndpointStart(ExpressionService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }

        public void OnEndpointStopped(ExpressionService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }
    }
}
