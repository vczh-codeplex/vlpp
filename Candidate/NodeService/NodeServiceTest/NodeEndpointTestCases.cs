using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeServiceTest.Endpoints;
using NodeService.Endpoints;
using NodeService;
using System.Threading;
using System.IO;

namespace NodeServiceTest
{
    public static class NodeEndpointTestCases
    {
        private static void AssertCollection(IEnumerable<int> enumerable)
        {
            int[] numbers = enumerable.ToArray();
            Assert.AreEqual(10, numbers.Length);
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i, numbers[i]);
            }
        }

        private static void AssertCollection(IDictionary<int, int> dictionary)
        {
            AssertCollection(dictionary.Keys);
            AssertCollection(dictionary.Values);
        }

        public static void TestProtocol(INodeEndpointProtocolFactory factory, string serverAddress, string clientAddress)
        {
            INodeEndpointProtocolServer server = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener serverListener = factory.CreateServerListener();
                server = serverListener.WaitForServer(serverAddress, new CalculationEndpoint(true));
            });
            serverThread.Start();

            ICalculationEndpoint client = factory.WaitForClient<ICalculationEndpoint>(clientAddress, "Calculation");
            Assert.IsNotNull(client);

            Assert.AreEqual(3, client.Add(2, 1));
            Assert.AreEqual(1, client.Sub(2, 1));
            Assert.AreEqual(2, client.Mul(2, 1));
            Assert.AreEqual(2, client.Div(2, 1));

            Point point = client.Swap(new Point { X = 1, Y = 2 });
            Assert.AreEqual(2, point.X);
            Assert.AreEqual(1, point.Y);

            Cat cat = (Cat)client.CopyAnimal(new Cat { name = "cat", catName = "bigcat" });
            Assert.AreEqual("cat", cat.name);
            Assert.AreEqual("bigcat", cat.catName);

            Dog dog = (Dog)client.CopyAnimal(new Dog { name = "dog", dogName = "bigdog" });
            Assert.AreEqual("dog", dog.name);
            Assert.AreEqual("bigdog", dog.dogName);

            client.SendMessage("Vczh is a genius!");
            Assert.AreEqual("Vczh is a genius!", client.ReceiveMessage());

            AssertCollection(client.CopyArray(Enumerable.Range(0, 10).ToArray()));
            AssertCollection(client.CopyList(new List<int>(Enumerable.Range(0, 10))));
            AssertCollection(client.CopyHashSet(new HashSet<int>(Enumerable.Range(0, 10))));
            AssertCollection(client.CopyLinkedList(new LinkedList<int>(Enumerable.Range(0, 10))));
            AssertCollection(client.CopyQueue(new Queue<int>(Enumerable.Range(0, 10))));
            AssertCollection(client.CopySortedSet(new SortedSet<int>(Enumerable.Range(0, 10))));
            AssertCollection(client.CopyStack(new Stack<int>(Enumerable.Range(0, 10).Reverse())));

            Dictionary<int, int> dictionary = Enumerable.Range(0, 10).ToDictionary(i => i);
            AssertCollection(client.CopyDictionary(dictionary));
            AssertCollection(client.CopySortedDictionary(new SortedDictionary<int, int>(dictionary)));
            AssertCollection(client.CopySortedList(new SortedList<int, int>(dictionary)));

            byte[] bytes = new byte[] { 1, 2, 3, 4, 5 };
            using (Stream stream = client.CopyStream(bytes))
            {
                byte[] copied = stream.ReadAllBytes();
                Assert.AreEqual("[1][2][3][4][5]", bytes.Select(b => "[" + b.ToString() + "]").Aggregate("", (a, b) => a + b));
            }
        }

        public static void TestProtocolAsync(INodeEndpointProtocolFactory factory, string serverAddress, string clientAddress)
        {
            INodeEndpointProtocolServer server = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener serverListener = factory.CreateServerListener();
                server = serverListener.WaitForServer(serverAddress, new CalculationEndpoint(true));
            });
            serverThread.Start();

            ICalculationEndpointAsync client = factory.WaitForClient<ICalculationEndpointAsync>(clientAddress, "Calculation");
            Assert.IsNotNull(client);

            Assert.AreEqual(3, client.Add(2, 1).Result);
            Assert.AreEqual(1, client.Sub(2, 1).Result);
            Assert.AreEqual(2, client.Mul(2, 1).Result);
            Assert.AreEqual(2, client.Div(2, 1).Result);

            Point point = client.Swap(new Point { X = 1, Y = 2 }).Result;
            Assert.AreEqual(2, point.X);
            Assert.AreEqual(1, point.Y);

            Cat cat = (Cat)client.CopyAnimal(new Cat { name = "cat", catName = "bigcat" }).Result;
            Assert.AreEqual("cat", cat.name);
            Assert.AreEqual("bigcat", cat.catName);

            Dog dog = (Dog)client.CopyAnimal(new Dog { name = "dog", dogName = "bigdog" }).Result;
            Assert.AreEqual("dog", dog.name);
            Assert.AreEqual("bigdog", dog.dogName);

            client.SendMessage("Vczh is a genius!").Wait();
            Assert.AreEqual("Vczh is a genius!", client.ReceiveMessage().Result);
        }

        public static void TestProtocolDuplex(INodeEndpointProtocolFactory factory, string serverAddress, string clientAddress)
        {
            INodeEndpointProtocolServer server = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener listener = factory.CreateServerListener();
                server = listener.WaitForServer(serverAddress, new DuplexServer());
            });
            serverThread.Start();

            IDuplexServer client = factory.WaitForClient<IDuplexServer, DuplexCallback>(clientAddress, "DuplexServer", new DuplexCallback());
            Assert.IsNotNull(client);

            Assert.AreEqual(15, client.Add(5));
        }
    }

    namespace Endpoints
    {
        [NodeEndpoint("DuplexServer")]
        public class DuplexServer : StrongTypedNodeEndpoint, IDuplexNodeEndpoint<IDuplexCallback>
        {
            public IDuplexCallback Callback { get; set; }

            public DuplexServer()
            {
                this.EnableAsynchronization = true;
            }

            [NodeEndpointMethod]
            public int Add(int number)
            {
                return number + Callback.GetAdder();
            }
        }

        [NodeEndpoint]
        public class DuplexCallback : StrongTypedNodeEndpoint
        {
            public DuplexCallback()
            {
                this.EnableAsynchronization = true;
            }

            [NodeEndpointMethod]
            public int GetAdder()
            {
                return 10;
            }
        }

        public interface IDuplexServer : IDuplexNodeEndpointClient<DuplexCallback>
        {
            int Add(int number);
        }

        public interface IDuplexCallback : INodeEndpointClient
        {
            int GetAdder();
        }
    }
}
