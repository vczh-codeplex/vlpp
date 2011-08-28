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
    public class ProtocolTest
    {
        [TestMethod]
        public void TestNamedPipeProtocol()
        {
            NodeEndpointTestCases.TestProtocol(new NamedPipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestNamedPipeProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(new NamedPipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestNamedPipeProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(new NamedPipeProtocolFactory(), "DuplexService", "localhost/DuplexService");
        }

        [TestMethod]
        public void TestTcpProtocol()
        {
            NodeEndpointTestCases.TestProtocol(new TcpProtocolFactory(), "8765", "127.0.0.1:8765");
        }

        [TestMethod]
        public void TestTcpProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(new TcpProtocolFactory(), "8765", "127.0.0.1:8765");
        }

        [TestMethod]
        public void TestTcpProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(new TcpProtocolFactory(), "8765", "127.0.0.1:8765");
        }
    }
}
