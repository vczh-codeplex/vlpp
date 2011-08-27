using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeServiceTest.Endpoints;
using NodeService.Protocols;
using NodeService;
using System.Threading;
using NodeService.Providers;
using NodeService.Endpoints;

namespace NodeServiceTest
{
    [TestClass]
    public class NamedPipeProtocolTest
    {
        [TestMethod]
        public void TestNamedPipeProtocol()
        {
            NamedPipeProtocolFactory protocolFactory = new NamedPipeProtocolFactory();
            INodeEndpointProtocolServer server = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener serverListener = protocolFactory.CreateServerListener();
                server = serverListener.WaitForServer("CalculationService", new CalculationEndpoint(true));
            });
            serverThread.Start();

            ICalculationEndpoint client = protocolFactory.WaitForClient<ICalculationEndpoint>("localhost/CalculationService", "Calculation");
            Assert.IsNotNull(client);
            Assert.IsNotNull(server);

            Assert.AreEqual(3, client.Add(2, 1));
            Assert.AreEqual(1, client.Sub(2, 1));
            Assert.AreEqual(2, client.Mul(2, 1));
            Assert.AreEqual(2, client.Div(2, 1));

            client.SendMessage("Vczh is a genius!");
            Assert.AreEqual("Vczh is a genius!", client.ReceiveMessage());
        }

        [TestMethod]
        public void TestNamedPipeProtocolAsync()
        {
            NamedPipeProtocolFactory protocolFactory = new NamedPipeProtocolFactory();
            INodeEndpointProtocolServer server = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener serverListener = protocolFactory.CreateServerListener();
                server = serverListener.WaitForServer("CalculationService", new CalculationEndpoint(true));
            });
            serverThread.Start();

            ICalculationEndpointAsync client = protocolFactory.WaitForClient<ICalculationEndpointAsync>("localhost/CalculationService", "Calculation");
            Assert.IsNotNull(client);
            Assert.IsNotNull(server);

            Assert.AreEqual(3, client.Add(2, 1).Result);
            Assert.AreEqual(1, client.Sub(2, 1).Result);
            Assert.AreEqual(2, client.Mul(2, 1).Result);
            Assert.AreEqual(2, client.Div(2, 1).Result);

            client.SendMessage("Vczh is a genius!").Wait();
            Assert.AreEqual("Vczh is a genius!", client.ReceiveMessage().Result);
        }
    }
}
