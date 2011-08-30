using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Endpoints;
using System.Threading.Tasks;

namespace NodeServiceTest.Endpoints
{
    [NodeEndpoint("Calculation")]
    class CalculationEndpoint : StrongTypedNodeEndpoint
    {
        public string Message { get; private set; }

        public CalculationEndpoint(bool enableAsynchronization)
        {
            this.EnableAsynchronization = enableAsynchronization;
        }

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
        public string ReceiveMessage()
        {
            return this.Message;
        }

        [NodeEndpointMethod]
        public void ThrowException()
        {
            throw new InvalidOperationException("ThrowException");
        }

        [NodeEndpointMethod]
        public Point Swap(Point p)
        {
            return new Point
            {
                X = p.Y,
                Y = p.X,
            };
        }
    }

    [NodeEndpointDataType]
    public class Point
    {
        [NodeEndpointDataMember]
        public int X { get; set; }

        [NodeEndpointDataMember]
        public int Y { get; set; }
    }

    public interface ICalculationEndpoint : INodeEndpointClient
    {
        int Add(int a, int b);
        int Sub(int a, int b);
        int Mul(int a, int b);
        int Div(int a, int b);
        void SendMessage(string message);
        string ReceiveMessage();
        void ThrowException();
        Point Swap(Point p);
    }

    public interface ICalculationEndpointAsync : INodeEndpointClient
    {
        Task<int> Add(int a, int b);
        Task<int> Sub(int a, int b);
        Task<int> Mul(int a, int b);
        Task<int> Div(int a, int b);
        Task SendMessage(string message);
        Task<string> ReceiveMessage();
        Task ThrowException();
        Task<Point> Swap(Point p);
    }
}
