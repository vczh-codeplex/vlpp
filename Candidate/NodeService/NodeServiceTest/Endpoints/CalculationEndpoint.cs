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

        [NodeEndpointMethod]
        public int[] CopyArray(int[] value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public List<int> CopyList(List<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public HashSet<int> CopyHashSet(HashSet<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public LinkedList<int> CopyLinkedList(LinkedList<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public Queue<int> CopyQueue(Queue<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public SortedSet<int> CopySortedSet(SortedSet<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public Stack<int> CopyStack(Stack<int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public Dictionary<int, int> CopyDictionary(Dictionary<int, int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public SortedDictionary<int, int> CopySortedDictionary(SortedDictionary<int, int> value)
        {
            return value;
        }

        [NodeEndpointMethod]
        public SortedList<int, int> CopySortedList(SortedList<int, int> value)
        {
            return value;
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

        int[] CopyArray(int[] value);
        List<int> CopyList(List<int> value);
        HashSet<int> CopyHashSet(HashSet<int> value);
        LinkedList<int> CopyLinkedList(LinkedList<int> value);
        Queue<int> CopyQueue(Queue<int> value);
        SortedSet<int> CopySortedSet(SortedSet<int> value);
        Stack<int> CopyStack(Stack<int> value);
        Dictionary<int, int> CopyDictionary(Dictionary<int, int> value);
        SortedDictionary<int, int> CopySortedDictionary(SortedDictionary<int, int> value);
        SortedList<int, int> CopySortedList(SortedList<int, int> value);
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
