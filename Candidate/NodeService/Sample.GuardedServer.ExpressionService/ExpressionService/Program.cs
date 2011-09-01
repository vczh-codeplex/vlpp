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
                "pipe://./" + ExpressionServiceConfiguration.ProtocolName + "/" + ExpressionServiceConfiguration.EndpointName
                );
        }
    }

    [NodeEndpoint(ExpressionServiceConfiguration.EndpointName)]
    public class ExpressionService : StrongTypedNodeEndpoint, IExpressionService
    {
        [NodeEndpointMethod]
        public RawExpression Parse(string input)
        {
            return RawExpression.Parse(input);
        }

        [NodeEndpointMethod]
        public RawExpression Simplify(RawExpression expression)
        {
            return expression.Simplify();
        }

        [NodeEndpointMethod]
        public RawExpression Different(RawExpression expression, string variable)
        {
            return expression.Different(variable);
        }

        [NodeEndpointMethod]
        public RawExpression Apply(RawExpression expression, string variable, double value)
        {
            return expression.Apply(variable, value);
        }

        [NodeEndpointMethod]
        public double Evaluate(RawExpression expression)
        {
            return expression.Execute(new Dictionary<string, double>());
        }

        [NodeEndpointMethod]
        public double Solve(RawExpression expression, string variable, double start)
        {
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
