using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using NodeService;
using NodeService.Protocols;
using System.Net.Sockets;
using System.Reflection;

namespace NodeServiceGuard.ServiceReflectoring
{
    public class ServiceReflector
    {
        private INodeEndpointProtocolFactory factory;
        private INodeEndpointProtocolFactory outestFactory;
        private string protocolAddress;
        private string endpointName;

        public ServiceReflector(XElement serviceDescription)
        {
            this.factory = BuildFactory(serviceDescription.Element("Protocol"), out this.outestFactory);
            this.protocolAddress = serviceDescription.Element("Address").Element("ProtocolAddress").Value;
            this.endpointName = serviceDescription.Element("Address").Element("EndpointName").Value;
        }

        public INodeEndpointProtocolFactory Factory
        {
            get
            {
                return this.factory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return this.protocolAddress;
            }
        }

        public string EndpointName
        {
            get
            {
                return this.endpointName;
            }
        }

        public string SuggestedAddress
        {
            get
            {
                if (this.outestFactory is NamedPipeProtocolFactory)
                {
                    return "localhost/" + this.protocolAddress;
                }
                else if (this.outestFactory is TcpProtocolFactory)
                {
                    return "127.0.0.1:" + this.protocolAddress;
                }
                else if (this.outestFactory is HttpProtocolFactory)
                {
                    int index = this.protocolAddress.IndexOfAny("+*".ToCharArray());
                    if (index == -1)
                    {
                        return this.protocolAddress;
                    }
                    else
                    {
                        string result = this.protocolAddress;
                        result.Remove(index, 1);
                        result.Insert(index, "localhost");
                        return result;
                    }
                }
                else
                {
                    return "";
                }
            }
        }

        private static INodeEndpointProtocolFactory BuildFactory(XElement protocolDescription, out INodeEndpointProtocolFactory outestFactory)
        {
            outestFactory = null;
            INodeEndpointProtocolFactory factory = null;
            foreach (var factoryDescription in protocolDescription.Elements().Reverse())
            {
                factory = BuildFactory(factoryDescription, factory);
                if (outestFactory == null)
                {
                    outestFactory = factory;
                }
            }
            return factory;
        }

        private static INodeEndpointProtocolFactory BuildFactory(XElement factoryDescription, INodeEndpointProtocolFactory previousFactory)
        {
            string factoryName = factoryDescription.Name.LocalName;
            switch (factoryName)
            {
                case "NamedPipeProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        return new NamedPipeProtocolFactory();
                    }
                case "TcpProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        TcpProtocolFactory factory = new TcpProtocolFactory();
                        factory.AddressFamily = (AddressFamily)typeof(AddressFamily)
                            .GetField(factoryDescription.Element("AddressFamily").Value, BindingFlags.Public | BindingFlags.Static)
                            .GetValue(null);
                        factory.ProtocolType = (ProtocolType)typeof(ProtocolType)
                            .GetField(factoryDescription.Element("ProtocolType").Value, BindingFlags.Public | BindingFlags.Static)
                            .GetValue(null);
                        return factory;
                    }
                case "HttpProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        return new HttpProtocolFactory();
                    }
                case "TranslatorProtocolFactory":
                    {
                        if (previousFactory == null)
                        {
                            throw new ArgumentException(factoryName + " cannot be the outest protocol.");
                        }
                        XElement handlerDescription = (XElement)factoryDescription.FirstNode;
                        string handlerName = handlerDescription.Name.LocalName;
                        switch (handlerName)
                        {
                            case "TranslatorProtocolHandlerFactorySimple":
                                {
                                    XElement handlerSimpleDescription = (XElement)handlerDescription.FirstNode;
                                    string handlerSimpleName = handlerSimpleDescription.Name.LocalName;
                                    switch (handlerSimpleName)
                                    {
                                        case "GzipProtocolHandler":
                                            return previousFactory.With(new GzipProtocolHandler());
                                        default:
                                            throw new ArgumentException("Don't know how to deal with " + handlerSimpleName + ".");
                                    }
                                }
                            default:
                                throw new ArgumentException("Don't know how to deal with " + handlerName + ".");
                        }
                    }
                default:
                    throw new ArgumentException("Don't know how to deal with " + factoryName + ".");
            }
        }
    }
}
