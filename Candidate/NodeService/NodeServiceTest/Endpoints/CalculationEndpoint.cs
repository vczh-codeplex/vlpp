using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;

namespace NodeServiceTest.Endpoints
{
    [NodeEndpoint("Calculation")]
    class CalculationEndpoint : StrongTypedNodeEndpoint
    {
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
    }

    interface ICalculationEndpoint
    {
        int Add(int a, int b);
        int Sub(int a, int b);
        int Mul(int a, int b);
        int Div(int a, int b);
    }
}
