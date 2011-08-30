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

        [NodeEndpointMethod]
        public Animal CopyAnimal(Animal animal)
        {
            return animal;
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

    [NodeEndpointDataType]
    [NodeEndpointKnownType(typeof(Cat))]
    [NodeEndpointKnownType(typeof(Dog))]
    public class Animal
    {
        [NodeEndpointDataMember]
        public string name;
    }

    [NodeEndpointDataType]
    public class Cat : Animal
    {
        [NodeEndpointDataMember]
        public string catName;
    }

    [NodeEndpointDataType]
    public class Dog : Animal
    {
        [NodeEndpointDataMember]
        public string dogName;
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
        Animal CopyAnimal(Animal animal);
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
        Task<Animal> CopyAnimal(Animal animal);
    }
}
