using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Endpoints;

namespace NodeServiceTest.Endpoints
{
    [NodeEndpoint("Calculation")]
    class CalculationEndpoint : StrongTypedNodeEndpoint
    {
        public string Message { get; private set; }

        [NodeEndpointMethod]
        public int Add(int a, int b)
        {
            return a + b;
        }

        [NodeEndpointMethod]
        public int Sub(int a, int b)
        {
            return a - b;
        }

        [NodeEndpointMethod]
        public void Mul(INodeEndpointRequest request, int a, int b)
        {
            Respond(request, a * b);
        }

        [NodeEndpointMethod]
        public void Div(INodeEndpointRequest request, int a, int b)
        {
            Respond(request, a / b);
        }

        [NodeEndpointMethod]
        public void SendMessage(string message)
        {
            this.Message = message;
        }

        [NodeEndpointMethod]
        public void ThrowException()
        {
            throw new InvalidOperationException("ThrowException");
        }
    }

    public interface ICalculationEndpoint : INodeEndpointClient
    {
        int Add(int a, int b);
        int Sub(int a, int b);
        int Mul(int a, int b);
        int Div(int a, int b);
        void SendMessage(string message);
        void ThrowException();
    }
}
