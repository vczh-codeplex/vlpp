using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeServiceTest.Endpoints;
using NodeService.Endpoints;
using NodeService;
using NodeService.Protocols;
using System.Threading;

namespace NodeServiceTest
{
    [TestClass]
    public class DuplexNodeEndpointTest
    {
        [TestMethod]
        public void TestDuplexNodeEndpoint()
        {
            INodeEndpointProtocolFactory factory = new NamedPipeProtocolFactory();

            INodeEndpointProtocolServer server = null;
            IDuplexServer client = null;

            Thread serverThread = new Thread(() =>
            {
                INodeEndpointProtocolServerListener listener = factory.CreateServerListener();
                server = listener.WaitForServerAndCallback<DuplexServer, IDuplexCallback>(
                    "DuplexService",
                    new DuplexServer(),
                    (s, c) => s.Callback = c
                    );
            });
            serverThread.Start();

            client = factory.WaitForClientAndCallback<IDuplexServer, DuplexCallback>(
                "localhost/DuplexService",
                "DuplexServer",
                new DuplexCallback()
                );

            Assert.IsNotNull(client);
            Assert.AreEqual(15, client.Add(5));
        }
    }

    namespace Endpoints
    {
        [NodeEndpoint("DuplexServer")]
        public class DuplexServer : StrongTypedNodeEndpoint
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

        public interface IDuplexServer : INodeEndpointClient
        {
            int Add(int number);
        }

        public interface IDuplexCallback : INodeEndpointClient
        {
            int GetAdder();
        }
    }
}
