using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NodeService.Endpoints;
using NodeService;
using NodeServiceHost;
using NodeService.Protocols;
using System.Threading;

namespace NodeServiceTest
{
    [TestClass]
    public class NodeEndpointServerTest
    {
        [NodeEndpoint("EndpointService")]
        public class Endpoint : StrongTypedNodeEndpoint
        {
            [NodeEndpointMethod]
            public string Concat(string a, string b)
            {
                return a + b;
            }

            [NodeEndpointMethod]
            public Guid CopyGuid(Guid guid)
            {
                return guid;
            }
        }

        public interface IEndpoint : INodeEndpointClient
        {
            string Concat(string a, string b);
            Guid CopyGuid(Guid guid);
        }

        public class EndpointServerCallback : INodeEndpointServerCallback<Endpoint>
        {
            private INodeEndpointProtocolFactory protocolFactory = new NamedPipeProtocolFactory();

            public int endpointStartedCounter = 0;
            public int endpointStoppedCounter = 0;

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
                    return "ConcatStringEndpointServer";
                }
            }

            public string EndpointName
            {
                get
                {
                    return null;
                }
            }

            public Endpoint CreateEndpoint()
            {
                return new Endpoint();
            }

            public void OnEndpointStart(Endpoint endpoint, INodeEndpointProtocolServer protocolServer)
            {
                Interlocked.Increment(ref this.endpointStartedCounter);
            }

            public void OnEndpointStopped(Endpoint endpoint, INodeEndpointProtocolServer protocolServer)
            {
                Interlocked.Increment(ref this.endpointStoppedCounter);
            }
        }

        [TestMethod]
        public void TestNodeEndpointServerStartAndStop()
        {
            EndpointServerCallback callback = new EndpointServerCallback();
            NodeEndpointServer<Endpoint> server = new NodeEndpointServer<Endpoint>();
            Assert.AreEqual(NodeEndpointServerState.Ready, server.ServerState);
            server.Start(callback);
            Assert.AreEqual(NodeEndpointServerState.Running, server.ServerState);
            server.Stop();
            Assert.AreEqual(NodeEndpointServerState.Stopped, server.ServerState);
        }

        [TestMethod]
        public void TestNodeEndpointServer()
        {
            EndpointServerCallback callback = new EndpointServerCallback();
            NodeEndpointServer<Endpoint> server = new NodeEndpointServer<Endpoint>();
            server.Start(callback);

            for (int i = 0; i < 3; i++)
            {
                IEndpoint client = callback.ProtocolFactory.WaitForClient<IEndpoint>("localhost/" + callback.ProtocolAddress, "EndpointService");
                Assert.AreEqual("AB", client.Concat("A", "B"));
                Guid guid = Guid.NewGuid();
                Assert.AreEqual(guid, client.CopyGuid(guid));
                client.Dispose();
            }

            server.Stop();
            Assert.AreEqual(3, callback.endpointStartedCounter);
            Assert.AreEqual(3, callback.endpointStoppedCounter);
        }
    }
}
