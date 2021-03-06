﻿using System;
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
using System.Xml.Linq;

namespace NodeServiceTest
{
    [TestClass]
    public class ProtocolTest
    {
        #region Network Protocol Test Cases

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
            NodeEndpointTestCases.TestProtocolAsync(new TcpProtocolFactory(), "8766", "127.0.0.1:8766");
        }

        [TestMethod]
        public void TestTcpProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(new TcpProtocolFactory(), "8767", "127.0.0.1:8767");
        }

        [TestMethod]
        public void TestHttpProtocol()
        {
            NodeEndpointTestCases.TestProtocol(new HttpProtocolFactory(), "http://+:8768/CalculationService/", "http://localhost:8768/CalculationService/");
        }

        [TestMethod]
        public void TestHttpProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(new HttpProtocolFactory(), "http://+:8769/CalculationService/", "http://localhost:8769/CalculationService/");
        }

        #endregion

        #region Translator Protocol Test Cases

        private INodeEndpointProtocolFactory CreateReversedNamePipeProtocolFactory()
        {
            return
                new NamedPipeProtocolFactory()
                .With(new TranslatorHandlerSimple())
                ;
        }

        [TestMethod]
        public void TestTranslatorProtocol()
        {
            NodeEndpointTestCases.TestProtocol(CreateReversedNamePipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestTranslatorProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(CreateReversedNamePipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestTranslatorProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(CreateReversedNamePipeProtocolFactory(), "DuplexService", "localhost/DuplexService");
        }

        #endregion

        #region Gzip Test Cases

        private INodeEndpointProtocolFactory CreateGzipNamePipeProtocolFactory()
        {
            return
                new NamedPipeProtocolFactory()
                .With(new GzipProtocolHandler())
                ;
        }

        [TestMethod]
        public void TestGzipProtocol()
        {
            NodeEndpointTestCases.TestProtocol(CreateGzipNamePipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestGzipProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(CreateGzipNamePipeProtocolFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestGzipProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(CreateGzipNamePipeProtocolFactory(), "DuplexService", "localhost/DuplexService");
        }

        #endregion

        #region Auth Test Cases

        private class AuthProvider : IAuthenticationProvider
        {
            public byte[] GetUserNamePasswordMD5(string userName)
            {
                return userName == "username" ? CreateAuthPasswordMD5() : null;
            }
        }

        private static byte[] CreateAuthPasswordMD5()
        {
            return AuthenticationProtocolHandlerFactory.MD5String("password");
        }

        private INodeEndpointProtocolFactory CreateAuthNamePipeProtocolServerFactory()
        {
            return
                new NamedPipeProtocolFactory()
                .With(new AuthenticationProtocolHandlerFactory(new AuthProvider()))
                ;
        }

        private INodeEndpointProtocolFactory CreateAuthNamePipeProtocolClientFactory()
        {
            return
                new NamedPipeProtocolFactory()
                .With(new AuthenticationProtocolHandlerFactory("username", CreateAuthPasswordMD5()));
        }

        [TestMethod]
        public void TestAuthProtocol()
        {
            NodeEndpointTestCases.TestProtocol(CreateAuthNamePipeProtocolServerFactory(), CreateAuthNamePipeProtocolClientFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestAuthProtocolAsync()
        {
            NodeEndpointTestCases.TestProtocolAsync(CreateAuthNamePipeProtocolServerFactory(), CreateAuthNamePipeProtocolClientFactory(), "CalculationService", "localhost/CalculationService");
        }

        [TestMethod]
        public void TestAuthProtocolDuplex()
        {
            NodeEndpointTestCases.TestProtocolDuplex(CreateAuthNamePipeProtocolServerFactory(), CreateAuthNamePipeProtocolClientFactory(), "DuplexService", "localhost/DuplexService");
        }

        #endregion
    }

    class TranslatorHandlerSimple : ITranslatorProtocolHandlerSimple
    {
        public byte[] Decode(byte[] bytes)
        {
            return bytes.Reverse().ToArray();
        }

        public byte[] Encode(byte[] bytes)
        {
            return bytes.Reverse().ToArray();
        }


        public XElement[] GetHandlerDescription()
        {
            return new XElement[]
            {
                new XElement("NodeServiceTest.TranslatorHandlerSimple")
            };
        }
    }
}
